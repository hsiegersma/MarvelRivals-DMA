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

		struct Offsets {
			uintptr_t uworld = 0xdee9d60; // https://dumpspace.spuckwaffel.com/Games/?hash=04842ac7&type=offsets
			uintptr_t gameInstance = 0x270; // https://dumpspace.spuckwaffel.com/Games/?hash=04842ac7&type=classes&idx=UWorld&member=OwningGameInstance
			uintptr_t playersArray = 0x40; // https://dumpspace.spuckwaffel.com/Games/?hash=04842ac7&type=classes&idx=UGameInstance&member=LocalPlayers
			uintptr_t playerController = 0x38; // https://dumpspace.spuckwaffel.com/Games/?hash=04842ac7&type=classes&idx=UPlayer&member=PlayerController
			uintptr_t playerCameraManager = 0x560; // https://dumpspace.spuckwaffel.com/Games/?hash=04842ac7&type=classes&idx=APlayerController&member=PlayerCameraManager
			uintptr_t playerCameraCache = 0x1A50; // https://dumpspace.spuckwaffel.com/Games/?hash=04842ac7&type=classes&idx=APlayerCameraManager&member=CameraCachePrivate
			uintptr_t playerCameraPOV = 0x10; // https://dumpspace.spuckwaffel.com/Games/?hash=04842ac7&type=structs&idx=FCameraCacheEntry&member=POV
			uintptr_t localPawn = 0x550; // https://dumpspace.spuckwaffel.com/Games/?hash=04842ac7&type=classes&idx=APlayerController&member=AcknowledgedPawn
			uintptr_t gameState = 0x210; // https://dumpspace.spuckwaffel.com/Games/?hash=04842ac7&type=classes&idx=UWorld&member=GameState
			uintptr_t playerArray = 0x4a8; // https://dumpspace.spuckwaffel.com/Games/?hash=04842ac7&type=classes&idx=AGameStateBase&member=PlayerArray
			uintptr_t pawnPrivate = 0x518; // https://dumpspace.spuckwaffel.com/Games/?hash=04842ac7&type=classes&idx=APlayerState&member=PawnPrivate
			uintptr_t childActorComponent = 0x530; // https://dumpspace.spuckwaffel.com/Games/?hash=04842ac7&type=classes&idx=ACharacter&member=ChildActorComponent
			uintptr_t childActor = 0x478; // https://dumpspace.spuckwaffel.com/Games/?hash=04842ac7&type=classes&idx=UChildActorComponent&member=ChildActor
			uintptr_t mesh = 0x490; // https://dumpspace.spuckwaffel.com/Games/?hash=04842ac7&type=classes&idx=ACharacterChildActorBase&member=Mesh
			uintptr_t marvelCharacter = 0x598; // https://dumpspace.spuckwaffel.com/Games/?hash=04842ac7&type=classes&idx=AMarvelPlayerState&member=MarvelCharacter
			uintptr_t defaultFov = 0x4ac; // https://dumpspace.spuckwaffel.com/Games/?hash=04842ac7&type=classes&idx=APlayerCameraManager&member=DefaultFOV
			uintptr_t teamState = 0xa30; // https://dumpspace.spuckwaffel.com/Games/?hash=04842ac7&type=classes&idx=AMarvelPlayerState&member=TeamState
			uintptr_t heroID = 0x13b0; // https://dumpspace.spuckwaffel.com/Games/?hash=04842ac7&type=classes&idx=AMarvelBaseCharacter&member=HeroID

			uintptr_t lastSubmitTime = 0x5A0;
			uintptr_t lastRenderTime = 0x5A4;

			uintptr_t componentToWorld = 0x2E0;
			uintptr_t boneArray = 0x998;
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