#include <includes.h>

void DllStart()
{
    auto EncryptedSignature_Malloc = Encrypt("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 83 EC ? 4C 8B 25 ? ? ? ? 33 ED");
    auto EncryptedSignature_Free = Encrypt("48 85 C9 0F 84 ? ? ? ? 57 48 83 EC ? 4C 89 74 24");
    //auto EncryptedSignature_GetBoneMatrix = Encrypt("48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 90 ? ? ? ? 44 0F 29 98 ? ? ? ? 44 0F 29 A0 ? ? ? ? 44 0F 29 A8 ? ? ? ? 44 0F 29 B0 ? ? ? ? 44 0F 29 B8 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 33 FF 4D 63 E0");
    /*
        auto EncryptedSignature_StaticFindObject = Encrypt("48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8B EC 48 83 EC ? 4C 8B E9 48 8D 4D");
    auto EncryptedSignature_GetBoneMatrix = Encrypt("48 8B C4 48 89 58 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 0F 29 78 ? 44 0F 29 40 ? 44 0F 29 48 ? 44 0F 29 90 ? ? ? ? 44 0F 29 98 ? ? ? ? 44 0F 29 A0 ? ? ? ? 44 0F 29 A8 ? ? ? ? 44 0F 29 B0 ? ? ? ? 44 0F 29 B8 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 33 FF 4D 63 E0");
    auto EncryptedSignature_GWorld = Encrypt("48 89 05 ? ? ? ? 0F 28 D7");
    auto EncryptedSignature_AimRotation = Encrypt("0F 10 05 ? ? ? ? F2 0F 10 0D ? ? ? ? 0F 10 1D");

    */

    Malloc = SignatureScan(FortniteClientWin64Shipping, EncryptedSignature_Malloc.decrypt()); EncryptedSignature_Malloc.clear();
    Free = SignatureScan(FortniteClientWin64Shipping, EncryptedSignature_Free.decrypt()); EncryptedSignature_Free.clear();
    //fGetBoneMatrix = SignatureScan(FortniteClientWin64Shipping, EncryptedSignature_GetBoneMatrix.decrypt()); EncryptedSignature_GetBoneMatrix.clear();

    auto Enc_KismetMathLibrary = Encrypt(L"Engine.Default__KismetMathLibrary");
    auto Enc_GameplayStatics = Encrypt(L"Engine.Default__GameplayStatics");
    auto Enc_FortKismetLibrary = Encrypt(L"FortniteGame.Default__FortKismetLibrary");
    auto Enc_KismetSystemLibrary = Encrypt(L"Engine.Default__KismetSystemLibrary");
    auto Enc_KismetStringLibrary = Encrypt(L"Engine.Default__KismetStringLibrary");

    KismetMathLibrary = UObject::FindObjectSingle<UObject*>(Enc_KismetMathLibrary.decrypt()); Enc_KismetMathLibrary.clear();
    GameplayStatics = UObject::FindObjectSingle<UObject*>(Enc_GameplayStatics.decrypt()); Enc_GameplayStatics.clear();
    FortKismetLibrary = UObject::FindObjectSingle<UObject*>(Enc_FortKismetLibrary.decrypt()); Enc_FortKismetLibrary.clear();
    KismetSystemLibrary = UObject::FindObjectSingle<UObject*>(Enc_KismetSystemLibrary.decrypt()); Enc_KismetSystemLibrary.clear();
    KismetStringLibrary = UObject::FindObjectSingle<UObject*>(Enc_KismetStringLibrary.decrypt()); Enc_KismetStringLibrary.clear();

    auto Enc_LeftMouseButton = Encrypt(L"LeftMouseButton");
    auto Enc_RightMouseButton = Encrypt(L"RightMouseButton");
    auto Enc_F2 = Encrypt(L"F2");
    auto Enc_F8 = Encrypt(L"F8");
    auto Enc_W = Encrypt(L"W");
    auto Enc_A = Encrypt(L"A");
    auto Enc_S = Encrypt(L"S");
    auto Enc_D = Encrypt(L"D");
    auto Enc_SpaceBar = Encrypt(L"SpaceBar");
    auto Enc_LeftShift = Encrypt(L"LeftShift");
    auto Enc_Capslock = Encrypt(L"Capslock");
    auto Enc_GamepadLeftTrigger = Encrypt(L"Gamepad_LeftTrigger");

    Keys::LeftMouseButton = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(Enc_LeftMouseButton.decrypt()) }, 0 }; Enc_LeftMouseButton.clear();
    Keys::RightMouseButton = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(Enc_RightMouseButton.decrypt()) }, 0 }; Enc_RightMouseButton.clear();
    Keys::GamepadLeftTrigger = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(Enc_GamepadLeftTrigger.decrypt()) }, 0 }; Enc_GamepadLeftTrigger.clear();
    Keys::F2 = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(Enc_F2.decrypt()) }, 0 }; Enc_F2.clear();
    Keys::F8 = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(Enc_F8.decrypt()) }, 0 }; Enc_F8.clear();
    Keys::W = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(Enc_W.decrypt()) }, 0 }; Enc_W.clear();
    Keys::A = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(Enc_A.decrypt()) }, 0 }; Enc_A.clear();
    Keys::S = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(Enc_S.decrypt()) }, 0 }; Enc_S.clear();
    Keys::D = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(Enc_D.decrypt()) }, 0 }; Enc_D.clear();
    Keys::SpaceBar = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(Enc_SpaceBar.decrypt()) }, 0 }; Enc_SpaceBar.clear();
    Keys::LeftShift = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(Enc_LeftShift.decrypt()) }, 0 }; Enc_LeftShift.clear();
    Keys::Capslock = FKey{ FName{ UKismetStringLibrary::Conv_StringToName(Enc_Capslock.decrypt()) }, 0 }; Enc_Capslock.clear();

    auto Enc_PlayerClass = Encrypt(L"FortniteGame.FortPlayerPawnAthena");
    auto Enc_LocalPlayer = Encrypt(L"Engine.LocalPlayer");
    auto Enc_Material = Encrypt(L"Engine.Material");
    auto Enc_PlayerController = Encrypt(L"Engine.PlayerController");
    auto Enc_PlayerCameraManager = Encrypt(L"Engine.PlayerCameraManager");
    auto Enc_GameViewportClient = Encrypt(L"Engine.GameViewportClient");
    auto Enc_PickupClass = Encrypt(L"FortniteGame.FortPickup");
    auto Enc_ContainerClass = Encrypt(L"FortniteGame.BuildingContainer");
    auto Enc_ProjectileClass = Encrypt(L"FortniteGame.FortProjectileBase");
    auto Enc_WeakspotClass = Encrypt(L"FortniteGame.BuildingWeakSpot");
    auto Enc_AthenaSuperDingo = Encrypt(L"FortniteGame.AthenaSuperDingo");
    auto Enc_VehicleClass = Encrypt(L"FortniteGame.FortAthenaVehicle");
    auto Enc_MarkerClass = Encrypt(L"FortniteGame.FortPlayerMarkerBase");
    auto Enc_SupplyDropClass = Encrypt(L"FortniteGame.FortAthenaSupplyDrop");
    auto Enc_RangedWeaponClass = Encrypt(L"FortniteGame.FortWeaponRanged");
    auto Enc_FortWeaponClass = Encrypt(L"FortniteGame.FortWeapon");
    auto Enc_PickaxeClass = Encrypt(L"FortniteGame.FortWeaponPickaxeAthena");

    Classes::PlayerClass = UObject::FindObjectSingle<UObject*>(Enc_PlayerClass.decrypt()); Enc_PlayerClass.clear();
    Classes::LocalPlayer = UObject::FindObjectSingle<UObject*>(Enc_LocalPlayer.decrypt()); Enc_LocalPlayer.clear();
    Classes::PlayerController = UObject::FindObjectSingle<UObject*>(Enc_PlayerController.decrypt()); Enc_PlayerController.clear();
    Classes::PlayerCameraManager = UObject::FindObjectSingle<UObject*>(Enc_PlayerCameraManager.decrypt()); Enc_PlayerCameraManager.clear();
    Classes::GameViewportClient = UObject::FindObjectSingle<UObject*>(Enc_GameViewportClient.decrypt()); Enc_GameViewportClient.clear();
    Classes::PickupClass = UObject::FindObjectSingle<UObject*>(Enc_PickupClass.decrypt()); Enc_PickupClass.clear();
    Classes::ContainerClass = UObject::FindObjectSingle<UObject*>(Enc_ContainerClass.decrypt()); Enc_ContainerClass.clear();
    Classes::WeakspotClass = UObject::FindObjectSingle<UObject*>(Enc_WeakspotClass.decrypt()); Enc_WeakspotClass.clear();
    Classes::AthenaSuperDingoClass = UObject::FindObjectSingle<UObject*>(Enc_AthenaSuperDingo.decrypt()); Enc_AthenaSuperDingo.clear();
    Classes::VehicleClass = UObject::FindObjectSingle<UObject*>(Enc_VehicleClass.decrypt()); Enc_VehicleClass.clear();
    Classes::SupplyDropClass = UObject::FindObjectSingle<UObject*>(Enc_SupplyDropClass.decrypt()); Enc_SupplyDropClass.clear();
    Classes::RangedWeaponClass = UObject::FindObjectSingle<UObject*>(Enc_RangedWeaponClass.decrypt()); Enc_RangedWeaponClass.clear();
    Classes::FortWeaponClass = UObject::FindObjectSingle<UObject*>(Enc_FortWeaponClass.decrypt()); Enc_FortWeaponClass.clear();
    Classes::PickaxeClass = UObject::FindObjectSingle<UObject*>(Enc_PickaxeClass.decrypt()); Enc_PickaxeClass.clear();
    Classes::MaterialClass = UObject::FindObjectSingle<UObject*>(Enc_Material.decrypt()); Enc_Material.clear();

    auto Enc_HairTone = Encrypt(L"HairTone_Highlights");
    auto Enc_SkinExponent = Encrypt(L"Skin Boost Color And Exponent");
    auto Enc_HitGlow = Encrypt(L"HitGlow");
    auto Enc_Emissive = Encrypt(L"Emissive_Color");

    Variables::MaterialParameter = UKismetStringLibrary::Conv_StringToName(Enc_HairTone.decrypt()); Enc_HairTone.clear();
    Variables::MaterialParameter1 = UKismetStringLibrary::Conv_StringToName(Enc_SkinExponent.decrypt()); Enc_SkinExponent.clear();
    Variables::MaterialParameter2 = UKismetStringLibrary::Conv_StringToName(Enc_HitGlow.decrypt()); Enc_HitGlow.clear();
    Variables::MaterialParameter3 = UKismetStringLibrary::Conv_StringToName(Enc_Emissive.decrypt()); Enc_Emissive.clear();

    auto Enc_DefaultRegularFont = Encrypt(L"DefaultRegularFont");
    MediumFont = UObject::FindObjectSingle<UFont*>(Enc_DefaultRegularFont.decrypt(), reinterpret_cast<UObject*>(-1)); Enc_DefaultRegularFont.clear();

    ObjectArray = (TUObjectArray*)(FortniteClientWin64Shipping + 0x12143B20);

    printf("Object Array [%llx] Object Count [%d] \n", ObjectArray, ObjectArray->NumElements);

    Hook::Discord::DiscordHook((void*)(FortniteClientWin64Shipping + 0x2B1FA50), &DrawTransition, (void**)(&DrawTransitionOriginal));
}

bool DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        AllocConsole();
        freopen("conin$", "r", stdin);
        freopen("conout$", "w", stdout);
        freopen("conout$", "w", stderr);

        auto EncryptedModule_FortniteClientWin64Shipping = Encrypt(L"FortniteClient-Win64-Shipping.exe");
        FortniteClientWin64Shipping = FindModuleByName(EncryptedModule_FortniteClientWin64Shipping.decrypt()); EncryptedModule_FortniteClientWin64Shipping.clear();
        if (!FortniteClientWin64Shipping) return false;

        auto EncryptedModule_DiscordHook64 = Encrypt(L"DiscordHook64.dll");
        DiscordHook64 = FindModuleByName(EncryptedModule_DiscordHook64.decrypt()); EncryptedModule_DiscordHook64.clear();
        if (!DiscordHook64) return false;

        SpoofCall::Initialize(reinterpret_cast<unsigned char*>(FortniteClientWin64Shipping));

        reinterpret_cast<void(__cdecl*)(uintptr_t, void*)>(DoSpoofCall)(SpoofCode, &DllStart);
    }

    return true;
}