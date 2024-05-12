#pragma once

//uintptr_t SavedPointer = 0;
//
//__int64 (*AddProjectileOriginal)(__int64, uintptr_t, char);
//__int64 AddProjectile(__int64 a1, uintptr_t a2, char a3)
//{
//	__int64 ReturnValue = reinterpret_cast<__int64(__cdecl*)(__int64, uintptr_t, char, uintptr_t, void*)>(DoSpoofCall)(a1, a2, a3, SpoofCode, AddProjectileOriginal);
//
//	__int64 ProjectileConfig = reinterpret_cast<__int64(__cdecl*)(__int64, uintptr_t, void*)>(DoSpoofCall)(a1, SpoofCode, (void*)(FortniteClientWin64Shipping + 0x1513F80));
//	SavedPointer = ProjectileConfig;
//
//	float WorldGravity = *(float*)(ProjectileConfig + 0xE8); // -980
//
//	return ReturnValue;
//}

//__int64 (*sub_A1E5274Original)(__int64, __int64, __int64, __int64);
//__int64 sub_A1E5274(__int64 a1, __int64 a2, __int64 a3, __int64 a4)
//{
//	__int64 ReturnValue = reinterpret_cast<__int64(__cdecl*)(__int64, __int64, __int64, __int64, uintptr_t, void*)>(DoSpoofCall)(a1, a2, a3, a4, SpoofCode, sub_A1E5274Original);
//
//	auto v9 = a4 + 8;
//	auto v10 = (__int64*)(a4 + 0x18);
//	if (!*v10)
//		v10 = (__int64*)v9;
//
//	int index = 2 * *(int*)(v9 + 0x18);
//	auto v11 = &(v10[index]);
//
//	if (v10 != v11)
//	{
//		__int64 ProjectileInstance = 0;
//
//		if (auto VTable = *(void***)*v10)
//		{
//			ProjectileInstance = reinterpret_cast<__int64(__cdecl*)(__int64, uintptr_t, void*)>(DoSpoofCall)(*v10, SpoofCode, VTable[0x30 / 0x8]); // creates the Lightweight Projectile Instance
//		}
//
//		__int64 ProjectileConfig = reinterpret_cast<__int64(__cdecl*)(__int64, uintptr_t, void*)>(DoSpoofCall)(ProjectileInstance, SpoofCode, (void*)(FortniteClientWin64Shipping + 0x1513F80));
//
//		float WorldGravity = *(float*)(ProjectileConfig + 0xE8);
//
//		printf("Projectile WorldGravity: %d\n", WorldGravity);
//	}
//
//	return ReturnValue;
//}

void Rehook(ULocalPlayer* LocalPlayer)
{
	APlayerController* PlayerController = LocalPlayer->PlayerController();
	if (UKismetSystemLibrary::IsValid(PlayerController))
	{
		static bool Update_FieldOfView = false;

		if (Settings::Exploits::CameraFOVChanger)
		{
			PlayerController->FOV((float)Settings::Exploits::CameraFOV);
			Update_FieldOfView = true;
		}
		else if (Update_FieldOfView)
		{
			PlayerController->FOV(0.f);
			Update_FieldOfView = false;
		}

		static APlayerController* OldPlayerController = nullptr;

		if (PlayerController != OldPlayerController)
		{
			bool PlayerControllerIsA = UGameplayStatics::ObjectIsA(PlayerController, Classes::PlayerController);
			if (PlayerControllerIsA)
			{
				static Hook::NewHook PlayerControllerHook;
				if (PlayerControllerHook.Initialize(PlayerController))
				{
					PlayerControllerHook.Insert(&GetPlayerViewPoint, Offsets::GetPlayerViewPoint, &GetPlayerViewPointOriginal);

					PlayerControllerHook.SwapContext();
				}

				OldPlayerController = PlayerController;
			}
		}

		APlayerCameraManager* PlayerCameraManager = PlayerController->PlayerCameraManager();
		if (UKismetSystemLibrary::IsValid(PlayerCameraManager))
		{
			static APlayerCameraManager* OldPlayerCameraManager = nullptr;

			if (PlayerCameraManager != OldPlayerCameraManager)
			{
				bool PlayerCameraManagerIsA = UGameplayStatics::ObjectIsA(PlayerCameraManager, Classes::PlayerCameraManager);
				if (PlayerCameraManagerIsA)
				{
					static Hook::NewHook PlayerCameraManagerHook;
					if (PlayerCameraManagerHook.Initialize(PlayerCameraManager))
					{
						PlayerCameraManagerHook.Insert(&GetCameraViewPoint, Offsets::GetCameraViewPoint, &GetCameraViewPointOriginal);

						PlayerCameraManagerHook.SwapContext();
					}

					OldPlayerCameraManager = PlayerCameraManager;
				}
			}
		}

		APlayerPawn_Athena_C* AcknowledgedPawn = PlayerController->AcknowledgedPawn();
		if (UKismetSystemLibrary::IsValid(AcknowledgedPawn))
		{
			AFortWeapon* CurrentWeapon = AcknowledgedPawn->CurrentWeapon();
			if (UKismetSystemLibrary::IsValid(CurrentWeapon))
			{
				static AFortWeapon* OldCurrentWeapon = nullptr;

				if (CurrentWeapon != OldCurrentWeapon)
				{
					bool CurrentWeaponIsARanged = UGameplayStatics::ObjectIsA(CurrentWeapon, Classes::RangedWeaponClass);
					if (CurrentWeaponIsARanged)
					{
						if (Settings::Exploits::NoRecoil)
						{
							*(int*)(FortniteClientWin64Shipping + 0x11F1D214) = 0;
						}

						static Hook::NewHook CurrentWeaponHook;
						if (CurrentWeaponHook.Initialize(CurrentWeapon))
						{
							CurrentWeaponHook.Insert(&GetTargetingSourceLocation, Offsets::GetTargetingSourceLocation, &GetTargetingSourceLocationOriginal);
							CurrentWeaponHook.Insert(&GetWeaponTargetingTransform, Offsets::GetWeaponTargetingTransform, &GetWeaponTargetingTransformOriginal);
							CurrentWeaponHook.Insert(&DetermineTraceChannel, Offsets::DetermineTraceChannel, &DetermineTraceChannelOriginal);
							Hook::Discord::DiscordHook((void*)(FortniteClientWin64Shipping + 0x18C8A30), &GetCalculatedReticleSpread, (void**)(&GetCalculatedReticleSpreadOriginal));

							CurrentWeaponHook.SwapContext();
						}

						OldCurrentWeapon = CurrentWeapon;
					}
				}
			}
		}
	}
}