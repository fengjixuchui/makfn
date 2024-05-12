#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <Helper/Safety/Imports.h>

// Modules
uintptr_t FortniteClientWin64Shipping, DiscordHook64 = 0;

// Functions
uintptr_t fGetBoneMatrix, fStaticFindObject, Malloc, Free = 0;

namespace Offsets
{
	// Global Arrays
	int GWorld = 0x1216B7B8;

	// Functions
	int StaticFindObject = 0x1520080;
	int GetBoneMatrix = 0x1B6D060;

	// Virtual Indexs
	int DrawTransition = 0x388 / 0x8; //  
	int ProcessEvent = 0x268 / 0x8;
	int GetMaterial = 0x2D0 / 0x8;
	int DetermineTraceChannel = 0xC18 / 0x8;
	int GetTargetingSourceLocation = 0xA28 / 0x8;
	int GetWeaponTargetingTransform = 0x7D8 / 0x8;
	int GetPlayerViewPoint = 0x550 / 0x8;
	int GetCameraViewPoint = 0x800 / 0x8;
	int GetFiringRate = 0x9E0 / 0x8;

	// E8 ? ? ? ? 48 8D 05 ? ? ? ? 45 33 D2 48 89 03 48 8D 8B ? ? ? ? 48 8D 05 ? ? ? ? 
	int AimRotation = 0x18D8; // 0x18D8 0x18F8
}

#define SpoofCode 0x13041052

#include <Helper/Safety/SKCrypter.h>
#include <Helper/Custom/NoCRT.h>

#include <Helper/Custom/Map.h>
#include <Helper/Custom/Vector.h>
#include <Helper/PEB.h>
#include <Helper/SysCall.h>
#include <Helper/SpoofCall.h>

#include <Helper/Custom/Module.h>
#include <Helper/Hooking/VTableHook.h>
#include <Helper/Hooking/DiscordHook.h>

#include <Fortnite/SDK/Structs.h>
#include <Fortnite/SDK/Classes.h>
#include <Fortnite/Settings.h>

TUObjectArray* ObjectArray;

namespace Variables
{
	UCanvas* Canvas = nullptr;
	UMaterial* Material = nullptr;
	APlayerController* PlayerController = nullptr;
	APlayerPawn_Athena_C* AcknowledgedPawn = nullptr;
	FName MaterialParameter = FName(0);
	FName MaterialParameter1 = FName(0);
	FName MaterialParameter2 = FName(0);
	FName MaterialParameter3 = FName(0);

	// Exploits
	FRotator TargetRotation = FRotator();

	//Variables
	static bool DrawMenu = true;
	static bool DragRadar = false;
	static FVector2D ScreenCenter = FVector2D();
	static FVector2D ScreenSize = FVector2D();

	//Camera
	static FVector CameraLocation = FVector();
	static FRotator CameraRotation = FRotator();
	static float FieldOfView = 0.f;

	//Bullet tracer
	static int LastBulletIndexPushed = 0;
	static FVector Last5BulletImpacts[5];
	static FVector Last5BulletImpactsMuzzle[5];

	//Exploits
	namespace GetTargetingSourceLocation
	{
		static bool bSilentAimActive = false;
		static FVector SilentLocationTarget = FVector();
	}

	namespace GetWeaponTargetingTransform
	{
		static bool bSilentAimActive = false;
		static FVector SilentLocationTarget = FVector();
	}

	namespace GetPlayerViewPoint
	{
		static bool bShouldStartAtLocationTarget = false;
		static FVector SilentLocationTarget = FVector();
		static FRotator LastTargetRotation = FRotator();
		static FVector OriginalLocation = FVector();
		static FRotator OriginalRotation = FRotator();
	}

	namespace GetCameraViewPoint
	{
		static FRotator LastStickyRotation = FRotator();
		static FVector StickyLocation = FVector();
	}
}

namespace Keys
{
	FKey LeftMouseButton;
	FKey RightMouseButton;
	FKey GamepadLeftTrigger;
	FKey F2;
	FKey F8;
	FKey W;
	FKey A;
	FKey S;
	FKey D;
	FKey Right;
	FKey SpaceBar;
	FKey LeftShift;
	FKey Capslock;
}

namespace Classes
{
	UObject* PlayerClass = nullptr;
	UObject* PickupClass = nullptr;
	UObject* ContainerClass = nullptr;
	UObject* WeakspotClass = nullptr;
	UObject* AthenaSuperDingoClass = nullptr;
	UObject* VehicleClass = nullptr;
	UObject* SupplyDropClass = nullptr;
	UObject* RangedWeaponClass = nullptr;
	UObject* FortWeaponClass = nullptr;
	UObject* PickaxeClass = nullptr;
	UObject* MaterialClass = nullptr;
	UObject* LocalPlayer = nullptr;
	UObject* PlayerController = nullptr;
	UObject* GameViewportClient = nullptr;
	UObject* PlayerCameraManager = nullptr;
}

enum Bones : int {
	Root = 0,
	pelvis = 2,
	spine_01 = 3,
	spine_02 = 4,
	spine_03 = 5,
	spine_04 = 6,
	spine_05 = 7,
	clavicle_l = 8,
	upperarm_l = 9,
	lowerarm_l = 10,
	lowerarm_twist_01_l = 33,
	hand_l = 11,
	clavicle_r = 37,
	upperarm_r = 38,
	lowerarm_r = 39,
	lowerarm_twist_01_r = 62,
	hand_r = 40,
	neck_01 = 66,
	neck_02 = 67,
	head = 68,
	thigh_l = 71,
	calf_l = 72,
	calf_twist_01_l = 73,
	calf_twist_02_l = 74,
	foot_l = 75,
	thigh_r = 78,
	calf_r = 79,
	calf_twist_01_r = 80,
	calf_twist_02_r = 81,
	foot_r = 82,
	camera_root = 92,
	Camera = 93,
	attach_fp = 94,
};

uintptr_t SignatureScan(uintptr_t ModuleBase, const char* Signature, bool Relative = false)
{
	auto pattern_to_byte = [](const char* pattern) {
		auto bytes = vector<int>{};
		const auto start = const_cast<char*>(pattern);
		const auto end = const_cast<char*>(pattern) + __strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;
				bytes.push_back(-1);
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
		};

	const auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(ModuleBase);
	const auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t*>(ModuleBase) + dos_header->e_lfanew);

	const auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	const auto pattern_bytes = pattern_to_byte(Signature);
	const auto scan_bytes = reinterpret_cast<uint8_t*>(ModuleBase);

	const auto s = pattern_bytes.size();
	const auto d = pattern_bytes.data();

	for (auto i = 0ul; i < size_of_image - s; ++i) {
		bool found = true;
		for (auto j = 0ul; j < s; ++j) {
			if (scan_bytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}

		if (found) {
			const auto offset = *reinterpret_cast<int*>(&scan_bytes[i] + 3);

			return Relative ? reinterpret_cast<uintptr_t>(&scan_bytes[i]) + offset + 7 : reinterpret_cast<uintptr_t>(&scan_bytes[i]);
		}
	}

	return 0;
}

#include <Fortnite/Hooks/DetermineTraceChannel.h>
#include <Fortnite/Hooks/GetCalculatedReticleSpread.h>
#include <Fortnite/Hooks/GetWeaponTargetingTransform.h>
#include <Fortnite/Hooks/GetTargetingSourceLocation.h>
#include <Fortnite/Hooks/GetPlayerViewPoint.h>
#include <Fortnite/Hooks/GetCameraViewPoint.h>

#include <Fortnite/Hook.h>
#include <Fortnite/Hooks/DrawTransition.h>