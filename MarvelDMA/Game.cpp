#include <Game.h>
#include <shared_mutex>
#include <Utils.h>
#include <DMALibrary/Memory/Memory.h>
#include <Globals.h>
#include <iostream>
#include <Aimbot.h>

namespace cache {
	std::shared_mutex cacheMutex;

	uintptr_t uworld;
	uintptr_t gameInstance;
	uintptr_t gameState;

	int playerNum;
	std::vector<Player> players;

	namespace local {
		uintptr_t pawn;
		uintptr_t playerState;
		int teamState;
		uintptr_t playerController;
		uintptr_t cameraManager;
		Camera camera;
	}
}

void Game::Loop() {
    manualCache();
	std::thread(Game::slowCacheLoop).detach();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	auto startTime = std::chrono::high_resolution_clock::now();
	int frameCount = 0;

	while (true) {
		fastCache();

		std::vector<Player> tempPlayers = cache::players;
		std::vector<PlayerRender> renderPlayers;

		for (Player& player : tempPlayers) {
			if (player.pawnPrivate == cache::local::pawn) continue;
			if (player.mesh == 0) continue;
			if (player.boneArray.Num() <= 0) continue;
			if (player.teamState == cache::local::teamState) continue;

			player.headW2S = doMatrix(player.head3DF, player.componentToWorld, cache::local::camera, Globals.screenWidth, Globals.screenHeight);
			player.bottomW2S = doMatrix(player.bottom3DF, player.componentToWorld, cache::local::camera, Globals.screenWidth, Globals.screenHeight);

			renderPlayers.push_back({ player.headW2S, player.bottomW2S, player.visible });
		}

		std::unique_lock lock(Globals.playerMutex);
		Globals.renderPlayers = renderPlayers;
		lock.unlock();

		for (Player& player : tempPlayers) {
			float dist = GetCrossDistance(player.headW2S.x, player.headW2S.y, Globals.screenWidth / 2, Globals.screenHeight / 2);

			if (dist < Globals.settings.fov && dist < Globals.aimbot.closestDistance) {
				Globals.aimbot.closestDistance = dist;
				Globals.aimbot.closestTarget = player.headW2S;
				Globals.aimbot.targetMesh = player.mesh;
			}
		}

		if (Globals.aimbot.targetMesh) {
			Aimbot::aimbot(Globals.aimbot.closestTarget);
		}

		Globals.aimbot.targetMesh = 0;
		Globals.aimbot.closestDistance = FLT_MAX;
		Globals.aimbot.closestTarget = { 0, 0 };

		// Measure FPS
		frameCount++;
		auto now = std::chrono::high_resolution_clock::now();
		double elapsed = std::chrono::duration<double>(now - startTime).count();

		if (elapsed >= 1.0) {
			Globals.readFPS = frameCount;
			frameCount = 0;
			startTime = std::chrono::high_resolution_clock::now();
		}
	}
}


void Game::manualCache() {
	Globals.renderPlayers.clear();
	bool result = false;
	while (!result) {
		uintptr_t newUWorld = mem.Read<uintptr_t>(mem.baseAddress + Globals.offsets.uworld);
		if (!newUWorld) continue;

		uintptr_t newGameInstance, newGameState;
		mem.AddScatterReadRequest(Globals.scatterHandle, newUWorld + Globals.offsets.gameInstance, &newGameInstance, sizeof(uintptr_t));
		mem.AddScatterReadRequest(Globals.scatterHandle, newUWorld + Globals.offsets.gameState, &newGameState, sizeof(uintptr_t));
		mem.ExecuteReadScatter(Globals.scatterHandle);

		if (!newGameInstance || !newGameState) continue;

		uintptr_t newLocalPlayers = mem.Read<uintptr_t>(mem.Read<uintptr_t>(newGameInstance + Globals.offsets.playersArray));
		if (!newLocalPlayers) continue;

		uintptr_t newPlayerController = mem.Read<uintptr_t>(newLocalPlayers + Globals.offsets.playerController);
		if (!newPlayerController) continue;

		uintptr_t newPawn = mem.Read<uintptr_t>(newPlayerController + Globals.offsets.localPawn);
		if (!newPawn) continue;

		uintptr_t newCameraManager = mem.Read<uintptr_t>(newPlayerController + Globals.offsets.playerCameraManager);
		if (!newCameraManager) continue;

		std::unique_lock lock(cache::cacheMutex);
		cache::uworld = newUWorld;
		cache::gameInstance = newGameInstance;
		cache::gameState = newGameState;
		cache::local::playerController = newPlayerController;
		cache::local::pawn = newPawn;
		cache::local::cameraManager = newCameraManager;
		cache::players.clear();
		cache::playerNum = 0;

		result = true;
	}

	slowCache();
}

void Game::slowCacheLoop() {
	while (true) {
		Game::slowCache();
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	}
}

void Game::slowCache() {
	TArray<uintptr_t> playerArray = mem.Read<TArray<uintptr_t>>(cache::gameState + Globals.offsets.playerArray);

	if (!playerArray.isValid() || playerArray.Num() <= 0) {
		std::unique_lock lock(cache::cacheMutex);
		cache::players.clear();
		manualCache();
		return;
	}

	int numPlayers = playerArray.Num();
	std::vector<Player> players(numPlayers);
	std::vector<uintptr_t> playerPointers(numPlayers);

	mem.Read(playerArray.getAddress(), playerPointers.data(), numPlayers * sizeof(uintptr_t));

	for (int i = 0; i < numPlayers; i++) {
		players[i].playerState = playerPointers[i];
		mem.AddScatterReadRequest(Globals.scatterHandle, players[i].playerState + Globals.offsets.pawnPrivate, &players[i].pawnPrivate, sizeof(uintptr_t));
		mem.AddScatterReadRequest(Globals.scatterHandle, players[i].playerState + Globals.offsets.heroID, &players[i].heroID, sizeof(int));
		mem.AddScatterReadRequest(Globals.scatterHandle, players[i].playerState + Globals.offsets.teamState, &players[i].teamState, sizeof(int));
	}
	mem.ExecuteReadScatter(Globals.scatterHandle);

	for (int i = 0; i < numPlayers; i++) {
		if (players[i].pawnPrivate == cache::local::pawn) {
			cache::local::playerState = players[i].playerState;
		}
	}

	for (Player& player : players) {
		if (player.pawnPrivate == 0) continue;
		mem.AddScatterReadRequest(Globals.scatterHandle, player.pawnPrivate + Globals.offsets.childActorComponent, &player.childActorComponent, sizeof(uintptr_t));
	}
	mem.ExecuteReadScatter(Globals.scatterHandle);

	for (Player& player : players) {
		if (player.childActorComponent == 0) continue;
		mem.AddScatterReadRequest(Globals.scatterHandle, player.childActorComponent + Globals.offsets.childActor, &player.childActor, sizeof(uintptr_t));
	}
	mem.ExecuteReadScatter(Globals.scatterHandle);

	for (Player& player : players) {
		if (player.childActor == 0) continue;
		mem.AddScatterReadRequest(Globals.scatterHandle, player.childActor + Globals.offsets.mesh, &player.mesh, sizeof(uintptr_t));
	}
	mem.ExecuteReadScatter(Globals.scatterHandle);

	for (Player& player : players) {
		if (player.mesh == 0) continue;
		mem.AddScatterReadRequest(Globals.scatterHandle, player.mesh + Globals.offsets.boneArray, &player.boneArray, sizeof(TArray<FTransform>));
		mem.AddScatterReadRequest(Globals.scatterHandle, player.mesh + Globals.offsets.boneArray + 0x10, &player.boneArrayCache, sizeof(TArray<FTransform>));
	}

	int tempLocalTeamState;
	mem.AddScatterReadRequest(Globals.scatterHandle, cache::local::playerState + Globals.offsets.teamState, &tempLocalTeamState, sizeof(int));

	mem.ExecuteReadScatter(Globals.scatterHandle);

	std::unique_lock lock(cache::cacheMutex);
	cache::playerNum = numPlayers;
	cache::players = players;
	cache::local::teamState = tempLocalTeamState;
	lock.unlock();
}

void Game::fastCache() {
	std::vector<Player> tempPlayers = cache::players;

	for (Player& player : tempPlayers) {
		if (!player.boneArray.Num()) player.boneArray = player.boneArrayCache;
		if (player.boneArray.Num() <= 0) continue;

		player.bones.resize(player.boneArray.Num());

		mem.AddScatterReadRequest(Globals.scatterHandle, player.boneArray.getAddress(), player.bones.data(), player.boneArray.Num() * sizeof(FTransform));
		mem.AddScatterReadRequest(Globals.scatterHandle, player.mesh + Globals.offsets.componentToWorld, &player.componentToWorld, sizeof(FTransform));

		mem.AddScatterReadRequest(Globals.scatterHandle, player.mesh + Globals.offsets.lastRenderTime, &player.lastRenderTime, sizeof(float));
		mem.AddScatterReadRequest(Globals.scatterHandle, player.mesh + Globals.offsets.lastSubmitTime, &player.lastSubmitTime, sizeof(float));
	}

	uintptr_t uworldCheck;
	mem.AddScatterReadRequest(Globals.scatterHandle, mem.baseAddress + Globals.offsets.uworld, &uworldCheck, sizeof(uintptr_t));
	
	Camera tempCamera;
	mem.AddScatterReadRequest(Globals.scatterHandle, cache::local::cameraManager + Globals.offsets.playerCameraCache + Globals.offsets.playerCameraPOV, &tempCamera, sizeof(Camera));

	TArray<uintptr_t> playerArray;
	mem.AddScatterReadRequest(Globals.scatterHandle, cache::gameState + Globals.offsets.playerArray, &playerArray, sizeof(TArray<uintptr_t>));

	mem.ExecuteReadScatter(Globals.scatterHandle);

	if (!uworldCheck || uworldCheck != cache::uworld) {
		manualCache();
		return;
	}

	if (playerArray.Num() != cache::playerNum) {
		slowCache();
		return;
	}

	for (Player& player : tempPlayers) {
		if (!player.boneArray.Num()) continue;
		if (player.bones.empty()) continue;

		int headBone = getHeadBone(player.heroID);
		if (headBone < 0 || headBone >= player.bones.size()) continue;

		player.head3DF = player.bones[getHeadBone(player.heroID)];
		player.bottom3DF = player.bones[0];

		if (player.lastSubmitTime - player.lastRenderTime <= 0.06f) {
			player.visible = true;
		}
		else {
			player.visible = false;
		}

	}

	std::unique_lock<std::shared_mutex> lock(cache::cacheMutex);
	cache::players = std::move(tempPlayers);
	cache::local::camera = tempCamera;
	lock.unlock();
}

