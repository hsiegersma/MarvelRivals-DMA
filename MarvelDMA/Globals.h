#pragma once

#include <string>
#include <singleton.h>
#include <shared_mutex>
#include <vector>
#include <Utils.h>

inline namespace MarvelDMA {
	class Globals : public Singleton<Globals> {

	public:
		int screenWidth = 1920;
		int screenHeight = 1080;

		std::string processName = "Marvel-Win64-Shipping.exe";
		HANDLE scatterHandle;

		std::shared_mutex playerMutex;
		std::vector<PlayerRender> renderPlayers;

		int readFPS = 0;

		AimType aimType;

		// Offsets removed - please do not update this just to use it to cheat
		struct Offsets {
			uintptr_t gworld = 0xE5E7000;
			uintptr_t uworld; // this will hold mem.Read<uintptr_t>(gworld) later

			uintptr_t gameInstance = 0x270;
			uintptr_t playersArray = 0x40;
			uintptr_t playerController = 0x38;
			uintptr_t playerCameraManager = 0x6c8;
			uintptr_t playerCameraCache = 0x1bb0;
			uintptr_t playerCameraPOV = 0x10;
			uintptr_t localPawn = 0x6b8;
			uintptr_t gameState = 0x210;
			uintptr_t playerArray = 0x610;
			uintptr_t pawnPrivate = 0x648;
			uintptr_t childActorComponent = 0x450;
			uintptr_t childActor = 0x478;
			uintptr_t mesh = 0x5f8;
			uintptr_t marvelCharacter = 0x950;
			uintptr_t defaultFov = 0x614;
			uintptr_t teamState = 0x630;
			uintptr_t heroID = 0xa60;

			uintptr_t lastSubmitTime = 0x380;
			uintptr_t lastRenderTime = 0x384;

			uintptr_t componentToWorld = 0x1c0;
			uintptr_t boneArray = 0xec0;
		} offsets;

		struct Settings {
			float smoothing = 1.25;
			int fov = 200;
		} settings;

		struct Aimbot {
			float closestDistance = FLT_MAX;
			Vector2 closestTarget = { 0, 0 };
			uintptr_t targetMesh = 0;
		} aimbot;
	};
#define Globals MarvelDMA::Globals::Get()
}
