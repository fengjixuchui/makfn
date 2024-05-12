#pragma once
#include "../DrawMenu.h"

void(*DrawTransitionOriginal)(UGameViewportClient*, UCanvas*) = nullptr;
void DrawTransition(UGameViewportClient* ViewPortClient, UCanvas* Canvas)
{
	if (!UKismetSystemLibrary::IsValid(ViewPortClient))  return DrawTransitionOriginal(ViewPortClient, Canvas);
	Variables::Canvas = Canvas;
	Variables::ScreenSize = FVector2D(double(Canvas->ClipX()), double(Canvas->ClipY()));
	Variables::ScreenCenter = FVector2D(Variables::ScreenSize.X / 2.0, Variables::ScreenSize.Y / 2.0);

	bool Update_SpinningRightNow = false;
	static bool SpinningRightNow = false;
	bool IsCurrentlySickySilentAiming = false;

	bool Update__GetTargetingSourceLocation__bSilentAimActive = false;
	FVector Update__GetTargetingSourceLocation__SilentLocationTarget = FVector();

	bool Update__GetWeaponTargetingTransform__bSilentAimActive = false;
	FVector Update__GetWeaponTargetingTransform__SilentLocationTarget = FVector();

	bool GetPlayerViewPoint_bShouldStartAtLocationTarget = false;
	FVector GetPlayerViewPoint_SilentLocationTarget = FVector();

	FVector Update_LocalHead = FVector();
	char UpdateMyTeam = char(1337);

	UWorld* GWorld = UWorld::GetWorld();
	if (!UKismetSystemLibrary::IsValid(GWorld)) return DrawTransitionOriginal(ViewPortClient, Canvas);

	UGameInstance* Gameinstance = GWorld->OwningGameInstance();
	if (!UKismetSystemLibrary::IsValid(Gameinstance)) return DrawTransitionOriginal(ViewPortClient, Canvas);

	ULocalPlayer* LocalPlayer = Gameinstance->LocalPlayers()[0];
	if (!UKismetSystemLibrary::IsValid(LocalPlayer)) return DrawTransitionOriginal(ViewPortClient, Canvas);

	UGameViewportClient* ViewportClient = LocalPlayer->ViewportClient();
	if (!UKismetSystemLibrary::IsValid(ViewportClient)) return DrawTransitionOriginal(ViewPortClient, Canvas);

	APlayerController* PlayerController = LocalPlayer->PlayerController();
	if (!UKismetSystemLibrary::IsValid(PlayerController)) return DrawTransitionOriginal(ViewPortClient, Canvas);
	Variables::PlayerController = PlayerController;

	APlayerCameraManager* PlayerCameraManager = PlayerController->PlayerCameraManager();
	if (!UKismetSystemLibrary::IsValid(PlayerCameraManager)) return DrawTransitionOriginal(ViewPortClient, Canvas);

	Variables::FieldOfView = PlayerCameraManager->GetFOVAngle();
	Variables::CameraLocation = PlayerCameraManager->GetCameraLocation();
	Variables::CameraRotation = PlayerCameraManager->GetCameraRotation();

	APlayerPawn_Athena_C* AcknowledgedPawn = PlayerController->AcknowledgedPawn();
	Variables::AcknowledgedPawn = AcknowledgedPawn;

	if (UKismetSystemLibrary::IsValid(AcknowledgedPawn))
	{
		UpdateMyTeam = AcknowledgedPawn->GetTeam();

		//PlayerController->SetIgnoreLookInput(Variables::DrawMenu);
		//PlayerController->ClientIgnoreMoveInput(Variables::DrawMenu);

		USkeletalMeshComponent* Mesh = AcknowledgedPawn->Mesh();
		if (UKismetSystemLibrary::IsValid(Mesh))
		{
			Update_LocalHead = Mesh->GetBoneLocation(Bones::head);
		}

		FVector StickyLocation = AcknowledgedPawn->K2_GetActorLocation(); StickyLocation.Z += 100.0;
		Variables::GetCameraViewPoint::StickyLocation = StickyLocation;

		if (Settings::Misc::StormPrediction)
		{
			FVector WorldLocation = FVector();
			FVector2D ScreenLocation = FVector2D();
			int32_t SafeZonePhase = UFortKismetLibrary::GetCurrentSafeZonePhase(GWorld);
			if (UFortKismetLibrary::GetSafeZoneLocation(GWorld, SafeZonePhase, WorldLocation))
			{
				if (PlayerController->ProjectWorldLocationToScreen(WorldLocation, &ScreenLocation))
				{
					double Distance = UKismetMathLibrary::Vector_Distance(Update_LocalHead, WorldLocation) * 0.01;
					FString ConvertedText = UKismetStringLibrary::BuildString_Double(L"SafeZone", L" (", int(Distance), L"m)");
					Wrapper::Text(ConvertedText, ScreenLocation, FLinearColor(0.8f, 0.52f, 0.25f, 1.0f), true);
				}
			}
		}

		if (Settings::Exploits::CarFly)
		{
			AFortAthenaVehicle* CurrentVehicle = AcknowledgedPawn->CurrentVehicle();
			if (UKismetSystemLibrary::IsValid(CurrentVehicle))
			{
				FVector CurrentLocation = CurrentVehicle->K2_GetActorLocation();
				if (PlayerController->IsInputKeyDown(Keys::W)) {
					CurrentLocation.X = CurrentLocation.X - Settings::Exploits::CarSpeed;
					CurrentVehicle->K2_TeleportTo(CurrentLocation, Variables::CameraRotation);
				}
				if (PlayerController->IsInputKeyDown(Keys::A)) {
					CurrentLocation.Y = CurrentLocation.Y + Settings::Exploits::CarSpeed;
					CurrentVehicle->K2_TeleportTo(CurrentLocation, Variables::CameraRotation);
				}
				if (PlayerController->IsInputKeyDown(Keys::S)) {
					CurrentLocation.X = CurrentLocation.X + Settings::Exploits::CarSpeed;
					CurrentVehicle->K2_TeleportTo(CurrentLocation, Variables::CameraRotation);
				}
				if (PlayerController->IsInputKeyDown(Keys::D)) {
					CurrentLocation.Y = CurrentLocation.Y - Settings::Exploits::CarSpeed;
					CurrentVehicle->K2_TeleportTo(CurrentLocation, Variables::CameraRotation);
				}
				if (PlayerController->IsInputKeyDown(Keys::SpaceBar)) {
					CurrentLocation.Z = CurrentLocation.Z + Settings::Exploits::CarSpeed;
					CurrentVehicle->K2_TeleportTo(CurrentLocation, Variables::CameraRotation);
				}
			}
		}

		if (Settings::Exploits::InfiniteFuel)
		{
			AFortAthenaVehicle* CurrentVehicle = AcknowledgedPawn->CurrentVehicle();
			if (UKismetSystemLibrary::IsValid(CurrentVehicle))
			{
				UFortVehicleFuelComponent* CachedFuelComponent = CurrentVehicle->CachedFuelComponent();
				CachedFuelComponent->ServerFuel = FLT_MAX;
				CachedFuelComponent->InfiniteFuel = FLT_MAX;
			}
		}
		

		if (Settings::Environment::Pickup)
		{
			TArray<UObject*> PickupArray = UGameplayStatics::GetAllActorsOfClass(GWorld, Classes::PickupClass);
			for (int i = 0; i < PickupArray.Count; i++)
			{
				AFortPickup* PickupActor = reinterpret_cast<AFortPickup*>(PickupArray[i]);
				if (!UKismetSystemLibrary::IsValid(PickupActor)) continue;

				bool PickupIsA = UGameplayStatics::ObjectIsA(PickupActor, Classes::PickupClass);
				if (!PickupIsA) continue;

				FVector2D ScreenLocation = FVector2D();
				FVector WorldLocation = PickupActor->K2_GetActorLocation();
				if (PlayerController->ProjectWorldLocationToScreen(WorldLocation, &ScreenLocation))
				{
					double Distance = UKismetMathLibrary::Vector_Distance(Update_LocalHead, WorldLocation) * 0.01;
					if (ScreenLocation.X < 5.0 || ScreenLocation.X > Variables::ScreenSize.X - (5.0 * 2)) continue;
					if (ScreenLocation.Y < 5.0 || ScreenLocation.Y > Variables::ScreenSize.Y - (5.0 * 2)) continue;
					if (Distance > Settings::Environment::MaxDistance) continue;

					if (UFortItemDefinition* PickupItemDefinition = *(UFortItemDefinition**)(PickupActor->PrimaryPickupItemEntry() + 0x18))
					{
						FString DistplayName = PickupItemDefinition->ItemName().Get();
						FString ConvertedText = UKismetStringLibrary::BuildString_Double(DistplayName, L" (", int(Distance), L"m)");
						Wrapper::Text(ConvertedText, ScreenLocation, Custom::GetColorByTier(PickupItemDefinition->Tier()), true);
					}
				}
			}
		}

		if (Settings::Environment::Container)
		{
			TArray<UObject*> ContainerArray = UGameplayStatics::GetAllActorsOfClass(GWorld, Classes::ContainerClass);
			for (int i = 0; i < ContainerArray.Count; i++)
			{
				ABuildingContainer* ContainerActor = reinterpret_cast<ABuildingContainer*>(ContainerArray[i]);
				if (!UKismetSystemLibrary::IsValid(ContainerActor)) continue;

				bool ContainerIsA = UGameplayStatics::ObjectIsA(ContainerActor, Classes::ContainerClass);
				if (!ContainerIsA) continue;

				FVector2D ScreenLocation = FVector2D();
				FVector WorldLocation = ContainerActor->K2_GetActorLocation();
				if (PlayerController->ProjectWorldLocationToScreen(WorldLocation, &ScreenLocation))
				{
					double Distance = UKismetMathLibrary::Vector_Distance(Update_LocalHead, WorldLocation) * 0.01;
					if (ScreenLocation.X < 5.0 || ScreenLocation.X > Variables::ScreenSize.X - (5.0 * 2)) continue;
					if (ScreenLocation.Y < 5.0 || ScreenLocation.Y > Variables::ScreenSize.Y - (5.0 * 2)) continue;
					if (Distance > Settings::Environment::MaxDistance) continue;

					if (*(BYTE*)(ContainerActor + 0xd92) & (1 << 2)) continue; // bAlreadySearched

					if (Settings::Environment::Chams)
					{
						UMeshComponent* Mesh = ContainerActor->StaticMeshComponent();
						if (!Mesh) continue;

						Mesh->SetCustomDepthStencilValue(11);
						Mesh->SetRenderCustomDepth(true);
					}

					FString ObjectName = UKismetSystemLibrary::GetObjectName(ContainerActor);
					if (UKismetStringLibrary::Contains(ObjectName, FString(L"Tiered_Chest"), true, false))
					{
						FString ConvertedText = UKismetStringLibrary::BuildString_Double(L"Chest", L" (", int(Distance), L"m)");
						Wrapper::Text(ConvertedText, ScreenLocation, FLinearColor(1.f, 1.f, 0.f, 1.f), true);
					}
					else if (UKismetStringLibrary::Contains(ObjectName, FString(L"Tiered_Ammo"), true, false))
					{
						FString ConvertedText = UKismetStringLibrary::BuildString_Double(L"Ammo", L" (", int(Distance), L"m)");
						Wrapper::Text(ConvertedText, ScreenLocation, FLinearColor(0.5f, 0.5f, 0.5f, 1.f), true);
					}
				}
			}

			TArray<UObject*> SupplyDroneArray = UGameplayStatics::GetAllActorsOfClass(GWorld, Classes::AthenaSuperDingoClass);
			for (int i = 0; i < SupplyDroneArray.Count; i++)
			{
				AAthenaSuperDingo* SupplyDroneActor = reinterpret_cast<AAthenaSuperDingo*>(SupplyDroneArray[i]);
				if (!UKismetSystemLibrary::IsValid(SupplyDroneActor)) continue;

				bool SupplyDroneIsA = UGameplayStatics::ObjectIsA(SupplyDroneActor, Classes::AthenaSuperDingoClass);
				if (!SupplyDroneIsA) continue;

				FVector2D ScreenLocation = FVector2D();
				FVector WorldLocation = SupplyDroneActor->K2_GetActorLocation();
				if (PlayerController->ProjectWorldLocationToScreen(WorldLocation, &ScreenLocation))
				{
					double Distance = UKismetMathLibrary::Vector_Distance(Update_LocalHead, WorldLocation) * 0.01;
					if (ScreenLocation.X < 5.0 || ScreenLocation.X > Variables::ScreenSize.X - (5.0 * 2)) continue;
					if (ScreenLocation.Y < 5.0 || ScreenLocation.Y > Variables::ScreenSize.Y - (5.0 * 2)) continue;
					if (Distance > Settings::Environment::MaxDistance) continue;

					FString ConvertedText = UKismetStringLibrary::BuildString_Double(L"SupplyDrone", L" (", int(Distance), L"m)");
					Wrapper::Text(ConvertedText, ScreenLocation, FLinearColor(0.8f, 0.52f, 0.25f, 1.0f), true);
				}
			}
		}

		if (Settings::Exploits::PlayerFly)
		{
			UCharacterMovementComponent* CharacterMovement = AcknowledgedPawn->CharacterMovement();
			if (UKismetSystemLibrary::IsValid(CharacterMovement))
			{
				CharacterMovement->bWantsToSwing = (short)257;
				CharacterMovement->SwingAttachLocation = FVector(0.0, 0.0, 10000000.0);
				CharacterMovement->bSwingInstantVelocity = true;

				if (PlayerController->IsInputKeyDown(Keys::LeftShift))
				{
					double Angle;

					Angle = Variables::CameraRotation.Yaw * (M_PI / 180.0);
					double sy = UKismetMathLibrary::sin(Angle);
					double cy = UKismetMathLibrary::cos(Angle);

					Angle = -Variables::CameraRotation.Roll * (M_PI / 180.0);
					double sp = UKismetMathLibrary::sin(Angle);
					double cp = UKismetMathLibrary::cos(Angle);

					CharacterMovement->SwingLaunch = FVector(cp * cy * 500.0, cp * sy * 500.0, -sp * 500.0 * 5.0);
				}
				if (PlayerController->IsInputKeyDown(Keys::W))
				{
					CharacterMovement->SwingLaunch = FVector();
				}
				if (PlayerController->IsInputKeyDown(Keys::SpaceBar)) 
				{
					CharacterMovement->SwingLaunch = FVector(0.0, 0.0, 2500.0);
				}
				if (PlayerController->IsInputKeyDown(Keys::S)) 
				{
					CharacterMovement->SwingLaunch = FVector(0.0, 0.0, -2500.0);
				}
			}
		}

		if (Settings::Environment::SupplyDrop)
		{
			TArray<UObject*> SupplyDropArray = UGameplayStatics::GetAllActorsOfClass(GWorld, Classes::SupplyDropClass);
			for (int i = 0; i < SupplyDropArray.Count; i++)
			{
				AFortAthenaSupplyDrop* SupplyDropActor = reinterpret_cast<AFortAthenaSupplyDrop*>(SupplyDropArray[i]);
				if (!UKismetSystemLibrary::IsValid(SupplyDropActor)) continue;

				bool SupplyDropIsA = UGameplayStatics::ObjectIsA(SupplyDropActor, Classes::SupplyDropClass);
				if (!SupplyDropIsA) continue;

				FVector2D ScreenLocation = FVector2D();
				FVector WorldLocation = SupplyDropActor->K2_GetActorLocation();
				if (PlayerController->ProjectWorldLocationToScreen(WorldLocation, &ScreenLocation))
				{
					double Distance = UKismetMathLibrary::Vector_Distance(Update_LocalHead, WorldLocation) * 0.01;
					if (ScreenLocation.X < 5.0 || ScreenLocation.X > Variables::ScreenSize.X - (5.0 * 2)) continue;
					if (ScreenLocation.Y < 5.0 || ScreenLocation.Y > Variables::ScreenSize.Y - (5.0 * 2)) continue;
					if (Distance > Settings::Environment::MaxDistance) continue;

					FString ObjectName = UKismetSystemLibrary::GetObjectName(SupplyDropActor);
					if (UKismetStringLibrary::Contains(ObjectName, FString(L"AthenaSupplyDrop_Llama"), true, false))
					{
						FString ConvertedText = UKismetStringLibrary::BuildString_Double(L"Llama", L" (", int(Distance), L"m)");
						Wrapper::Text(ConvertedText, ScreenLocation, FLinearColor(0.5f, 0.0f, 1.f, 1.f), true);
					}
					else if (UKismetStringLibrary::Contains(ObjectName, FString(L"AthenaSupplyDrop_C"), true, false))
					{
						FString ConvertedText = UKismetStringLibrary::BuildString_Double(L"SupplyDrop", L" (", int(Distance), L"m)");
						Wrapper::Text(ConvertedText, ScreenLocation, FLinearColor(0.1f, 0.5f, 0.9f, 1.0f), true);
					}
				}
			}
		}

		if (Settings::Environment::Weakspot)
		{
			double ClosestDistanceToCenter = DBL_MAX;
			ABuildingWeakSpot* TargetWeakSpot = nullptr;

			TArray<UObject*> WeakspotArray = UGameplayStatics::GetAllActorsOfClass((UObject*)GWorld, Classes::WeakspotClass);
			for (int i = 0; i < WeakspotArray.Count; i++)
			{
				ABuildingWeakSpot* WeakspotActor = reinterpret_cast<ABuildingWeakSpot*>(WeakspotArray[i]);
				if (!UKismetSystemLibrary::IsValid(WeakspotActor)) continue;

				bool WeakspotIsA = UGameplayStatics::ObjectIsA(WeakspotActor, Classes::WeakspotClass);
				if (!WeakspotIsA) continue;

				FVector2D ScreenLocation = FVector2D();
				FVector WorldLocation = WeakspotActor->K2_GetActorLocation();
				if (PlayerController->ProjectWorldLocationToScreen(WorldLocation, &ScreenLocation))
				{
					double Distance = UKismetMathLibrary::Vector_Distance(Update_LocalHead, WorldLocation) * 0.01;
					if (ScreenLocation.X < 5.0 || ScreenLocation.X > Variables::ScreenSize.X - (5.0 * 2)) continue;
					if (ScreenLocation.Y < 5.0 || ScreenLocation.Y > Variables::ScreenSize.Y - (5.0 * 2)) continue;
					if (Distance > 10.0) continue;

					if (*(BYTE*)(WeakspotActor + 0x2c0) & (1 << 0)) continue; // bHit
					if (!(*(BYTE*)(WeakspotActor + 0x2c0) & (1 << 2))) continue; // bActive

					Wrapper::Text(L"WS", ScreenLocation, FLinearColor(1.f, 0.f, 0.f, 1.f), true);

					double DistanceToCenter = UKismetMathLibrary::Vector_Distance2D(Variables::ScreenCenter, ScreenLocation);
					if (DistanceToCenter < ClosestDistanceToCenter)
					{
						ClosestDistanceToCenter = DistanceToCenter;
						TargetWeakSpot = WeakspotActor;
					}
				}
			}

			if (UKismetSystemLibrary::IsValid(TargetWeakSpot))
			{
				AFortWeapon* CurrentWeapon = AcknowledgedPawn->CurrentWeapon();
				bool IsHoldingPickaxe = UGameplayStatics::ObjectIsA(CurrentWeapon, Classes::PickaxeClass);
				if (IsHoldingPickaxe)
				{
					FKey AimbotKey = Keys::RightMouseButton;

					if (Settings::Aimbot::KeyType == 1)
						AimbotKey = Keys::LeftMouseButton;
					else if (Settings::Aimbot::KeyType == 2)
						AimbotKey = Keys::LeftShift;

					FVector WorldLocation = TargetWeakSpot->K2_GetActorLocation();
					FVector2D ScreenLocation = FVector2D();
					if (PlayerController->ProjectWorldLocationToScreen(WorldLocation, &ScreenLocation))
					{
						if (PlayerController->IsInputKeyDown(AimbotKey) || PlayerController->IsInputKeyDown(Keys::GamepadLeftTrigger))
						{
							FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Variables::CameraLocation, WorldLocation);

							TargetRotation.Pitch = (TargetRotation.Pitch - Variables::CameraRotation.Pitch) / double(PlayerController->InputPitchScale());
							TargetRotation.Yaw = (TargetRotation.Yaw - Variables::CameraRotation.Yaw) / double(PlayerController->InputYawScale());

							PlayerController->AddYawInput((float)TargetRotation.Yaw);
							PlayerController->AddPitchInput((float)TargetRotation.Pitch);
						}
					}
				}
			}
		}

		if (Settings::Environment::Vehicle)
		{
			TArray<UObject*> VehicleArray = UGameplayStatics::GetAllActorsOfClass(GWorld, Classes::VehicleClass);
			for (int i = 0; i < VehicleArray.Count; i++)
			{
				AFortAthenaVehicle* VehicleActor = reinterpret_cast<AFortAthenaVehicle*>(VehicleArray[i]);
				if (!UKismetSystemLibrary::IsValid(VehicleActor)) continue;

				bool VehicleIsA = UGameplayStatics::ObjectIsA(VehicleActor, Classes::VehicleClass);
				if (!VehicleIsA) continue;

				FVector2D ScreenLocation = FVector2D();
				FVector WorldLocation = VehicleActor->K2_GetActorLocation();
				if (PlayerController->ProjectWorldLocationToScreen(WorldLocation, &ScreenLocation))
				{
					double Distance = UKismetMathLibrary::Vector_Distance(Update_LocalHead, WorldLocation) * 0.01;
					if (ScreenLocation.X < 5.0 || ScreenLocation.X > Variables::ScreenSize.X - (5.0 * 2)) continue;
					if (ScreenLocation.Y < 5.0 || ScreenLocation.Y > Variables::ScreenSize.Y - (5.0 * 2)) continue;
					if (Distance > Settings::Environment::MaxDistance) continue;

					FString ConvertedText = UKismetStringLibrary::BuildString_Double(L"Vehicle", L" (", int(Distance), L"m)");
					Wrapper::Text(ConvertedText, ScreenLocation, FLinearColor(0.2f, 0.3f, 0.7f, 1.f), true);
				}
			}
		}
	}

	double ClosestDistanceToMyself = DBL_MAX;
	double ClosestDistanceToCenter = DBL_MAX;
	APlayerPawn_Athena_C* TargetPlayer = nullptr;

	double AimbotFOV = (Settings::Aimbot::FOV * Variables::ScreenSize.X / Variables::FieldOfView) / 2.0;

	TArray<UObject*> PlayerList = UGameplayStatics::GetAllActorsOfClass(GWorld, Classes::PlayerClass);
	for (int32_t i = 0; i < PlayerList.Count; i++)
	{
		APlayerPawn_Athena_C* Player = reinterpret_cast<APlayerPawn_Athena_C*>(PlayerList[i]);
		if (!UKismetSystemLibrary::IsValid(Player)) 
			continue;

		bool PlayerCheck = UGameplayStatics::ObjectIsA(Player, Classes::PlayerClass);
		if (PlayerCheck)
		{
			if (Player == AcknowledgedPawn) continue;

			bool Wound = Player->IsDBNO();

			bool Dead = Player->IsDead();
			if (Dead) continue;

			char Team = Player->GetTeam();
			bool Teammate = UpdateMyTeam != char(1337) && UpdateMyTeam == Team;

			bool Visable = UFortKismetLibrary::CheckLineOfSightToActorWithChannel(Variables::CameraLocation, Player, ECollisionChannel::ECC_Visibility, AcknowledgedPawn);

			APlayerState* PlayerState = Player->PlayerState();
			if (!UKismetSystemLibrary::IsValid(PlayerState)) continue;

			USkeletalMeshComponent* Mesh = Player->Mesh();
			if (!UKismetSystemLibrary::IsValid(Mesh)) continue;

			if (Settings::Exploits::Chams)
			{
				if (!UKismetSystemLibrary::IsValid(Variables::Material))
				{
					Variables::Material = reinterpret_cast<UMaterial*>(ObjectArray->FindObject(UKismetStringLibrary::Conv_StringToName(L"Eyeshadow"), Classes::MaterialClass));
				}

				if (UKismetSystemLibrary::IsValid(Variables::Material))
				{
					if (Settings::Exploits::Glow)
					{
						*(BYTE*)(Variables::Material + 0x1a8) |= 1 << 0; // bDisableDepthTest
					}

					if (Settings::Exploits::Wireframe)
					{
						*(BYTE*)(Variables::Material + 0x1b8) |= 1 << 6; // TwoSided
						*(BYTE*)(Variables::Material + 0x170) |= 1 << 4; // Wireframe
					}

					for (int xi = 0; xi < 7; xi++)
					{
						USkeletalMeshComponent* CharacterPartSkeletalMeshComponents = Player->CharacterPartSkeletalMeshComponents[xi];
						if (!UKismetSystemLibrary::IsValid(CharacterPartSkeletalMeshComponents)) continue;

						TArray<UMaterialInterface*> Materials = CharacterPartSkeletalMeshComponents->GetMaterials();
						for (int mi = 0; mi < Materials.Count; mi++)
						{
							CharacterPartSkeletalMeshComponents->SetMaterial(mi, Variables::Material);
						}
					}
				}
			}

			FLinearColor BoxColor = Settings::Colors::BoxInVisible;
			FLinearColor SkeletonColor = Settings::Colors::SkeletonInVisible;
			FLinearColor SnaplineColor = Settings::Colors::SkeletonInVisible;

			if (Visable)
			{
				BoxColor = Settings::Colors::BoxVisible;
				SkeletonColor = Settings::Colors::SkeletonVisible;
				SnaplineColor = Settings::Colors::SnaplineVisible;
			}

			if (Teammate)
			{
				BoxColor = Settings::Colors::TeammateColor;
				SkeletonColor = Settings::Colors::TeammateColor;
				SnaplineColor = Settings::Colors::TeammateColor;
			}

			FVector Root = Mesh->GetBoneLocation(Bones::Root);
			double Distance = UKismetMathLibrary::Vector_Distance(Variables::CameraLocation, Root) * 0.01;

			if (Settings::Player::Radar)
			{
				UFortClientSettingsRecord* ClientSettingsRecord = LocalPlayer->ClientSettingsRecord();
				if (UKismetSystemLibrary::IsValid(ClientSettingsRecord))
				{
					FVector2D RadarPosition = FVector2D(Settings::Player::RadarX, Settings::Player::RadarY);

					double RadarSize = (15.625 * Variables::ScreenCenter.X * double(ClientSettingsRecord->HUDScale()) / 100.0) * 2;

					double RadarPositionOffset = RadarSize / 30.0;
					RadarPosition = Settings::Player::Minimap ? FVector2D(Variables::ScreenSize.X - RadarSize - RadarPositionOffset, RadarPositionOffset) : RadarPosition;

					Wrapper::Rect(RadarPosition, FVector2D(RadarSize, RadarSize), FLinearColor(0.f, 0.f, 0.f, 1.f), 1.f);
					if (!Settings::Player::Minimap)
						Wrapper::RectFilled(RadarPosition, FVector2D(RadarSize, RadarSize), FLinearColor(0.024f, 0.024f, 0.025f, 0.0f));
					Radar::Add(Root, Variables::CameraLocation, BoxColor, RadarPosition, FVector2D(RadarSize, RadarSize), 5.0);
				}
			}

			if (Distance > Settings::Player::MaxDistance) continue;

			FVector Head = Mesh->GetBoneLocation(Bones::head);
			FVector2D HeadScreen = FVector2D();
			if (!PlayerController->ProjectWorldLocationToScreen(Head, &HeadScreen)) continue;

			if (HeadScreen.X < 5.0 || HeadScreen.X > Variables::ScreenSize.X - (5.0 * 2)) continue;
			if (HeadScreen.Y < 5.0 || HeadScreen.Y > Variables::ScreenSize.Y - (5.0 * 2)) continue;

			bool AimbotVisable = (Settings::Aimbot::VisableOnly && Visable) || !Settings::Aimbot::VisableOnly;
			bool AimbotKnocked = (Settings::Aimbot::SkipKnocked && !Wound) || !Settings::Aimbot::SkipKnocked;
			bool AimbotValid = (AimbotVisable && AimbotKnocked && !Teammate) && Distance < Settings::Aimbot::MaxDistance;
			if (AimbotValid) {
				double DistanceToCenter = UKismetMathLibrary::Vector_Distance2D(Variables::ScreenCenter, HeadScreen);
				if (DistanceToCenter < ClosestDistanceToCenter) {
					ClosestDistanceToCenter = DistanceToCenter;
					TargetPlayer = Player;
				}
			}

			FVector2D RootScreen;
			if (!PlayerController->ProjectWorldLocationToScreen(Root, &RootScreen)) continue;

			FVector Head2 = FVector(Head.X, Head.Y, Head.Z + 18.0);
			FVector2D HeadScreen2;
			if (!PlayerController->ProjectWorldLocationToScreen(Head2, &HeadScreen2)) continue;

			FVector Neck = Mesh->GetBoneLocation(Bones::neck_02);
			FVector2D NeckScreen = FVector2D();
			if (!PlayerController->ProjectWorldLocationToScreen(Neck, &NeckScreen)) continue;

			FVector Chest = Mesh->GetBoneLocation(Bones::spine_05);
			FVector2D ChestScreen = FVector2D();
			if (!PlayerController->ProjectWorldLocationToScreen(Chest, &ChestScreen)) continue;

			FVector Pelvis = Mesh->GetBoneLocation(Bones::pelvis);
			FVector2D PelvisScreen = FVector2D();
			if (!PlayerController->ProjectWorldLocationToScreen(Pelvis, &PelvisScreen)) continue;

			FVector RShoulder = Mesh->GetBoneLocation(Bones::upperarm_r);
			FVector2D RShoulderScreen = FVector2D();
			if (!PlayerController->ProjectWorldLocationToScreen(RShoulder, &RShoulderScreen)) continue;

			FVector RElbow = Mesh->GetBoneLocation(Bones::lowerarm_r);
			FVector2D RElbowScreen = FVector2D();
			if (!PlayerController->ProjectWorldLocationToScreen(RElbow, &RElbowScreen)) continue;

			FVector RHand = Mesh->GetBoneLocation(Bones::lowerarm_twist_01_r);
			FVector2D RHandScreen = FVector2D();
			if (!PlayerController->ProjectWorldLocationToScreen(RHand, &RHandScreen)) continue;

			FVector RThigh = Mesh->GetBoneLocation(Bones::thigh_r);
			FVector2D RThighScreen = FVector2D();
			if (!PlayerController->ProjectWorldLocationToScreen(RThigh, &RThighScreen)) continue;

			FVector RCalf = Mesh->GetBoneLocation(Bones::calf_r);
			FVector2D RCalfScreen = FVector2D();
			if (!PlayerController->ProjectWorldLocationToScreen(RCalf, &RCalfScreen)) continue;

			FVector RKnee = Mesh->GetBoneLocation(Bones::calf_twist_01_r);
			FVector2D RKneeScreen = FVector2D();
			if (!PlayerController->ProjectWorldLocationToScreen(RKnee, &RKneeScreen)) continue;

			FVector RFoot = Mesh->GetBoneLocation(Bones::foot_r);
			FVector2D RFootScreen = FVector2D();
			if (!PlayerController->ProjectWorldLocationToScreen(RFoot, &RFootScreen)) continue;

			FVector LShoulder = Mesh->GetBoneLocation(Bones::upperarm_l);
			FVector2D LShoulderScreen = FVector2D();
			if (!PlayerController->ProjectWorldLocationToScreen(LShoulder, &LShoulderScreen)) continue;

			FVector LElbow = Mesh->GetBoneLocation(Bones::lowerarm_l);
			FVector2D LElbowScreen = FVector2D();
			if (!PlayerController->ProjectWorldLocationToScreen(LElbow, &LElbowScreen)) continue;

			FVector LHand = Mesh->GetBoneLocation(Bones::lowerarm_twist_01_l);
			FVector2D LHandScreen = FVector2D();
			if (!PlayerController->ProjectWorldLocationToScreen(LHand, &LHandScreen)) continue;

			FVector LThigh = Mesh->GetBoneLocation(Bones::thigh_l);
			FVector2D LThighScreen = FVector2D();
			if (!PlayerController->ProjectWorldLocationToScreen(LThigh, &LThighScreen)) continue;

			FVector LCalf = Mesh->GetBoneLocation(Bones::calf_l);
			FVector2D LCalfScreen = FVector2D();
			if (!PlayerController->ProjectWorldLocationToScreen(LCalf, &LCalfScreen)) continue;

			FVector LKnee = Mesh->GetBoneLocation(Bones::calf_twist_01_l);
			FVector2D LKneeScreen = FVector2D();
			if (!PlayerController->ProjectWorldLocationToScreen(LKnee, &LKneeScreen)) continue;

			FVector LFoot = Mesh->GetBoneLocation(Bones::foot_l);
			FVector2D LFootScreen = FVector2D();
			if (!PlayerController->ProjectWorldLocationToScreen(LFoot, &LFootScreen)) continue;

			FVector2D BonesToCheck[] = { RootScreen, HeadScreen2, NeckScreen, ChestScreen, PelvisScreen, RShoulderScreen, RElbowScreen, RHandScreen, RCalfScreen, RKneeScreen, RFootScreen, LShoulderScreen, LElbowScreen, LHandScreen, LCalfScreen, LKneeScreen, LFootScreen };

			double MostLeft = DBL_MAX;
			double MostRight = DBL_MIN;
			double MostTop = DBL_MAX;
			double MostBottom = DBL_MIN;

			for (int ci = 0; ci < sizeof(BonesToCheck) / sizeof(FVector2D); ci++)
			{
				FVector2D CurrentBone = BonesToCheck[ci];

				if (CurrentBone.X < MostLeft)
					MostLeft = CurrentBone.X;

				if (CurrentBone.X > MostRight)
					MostRight = CurrentBone.X;

				if (CurrentBone.Y < MostTop)
					MostTop = CurrentBone.Y;

				if (CurrentBone.Y > MostBottom)
					MostBottom = CurrentBone.Y;
			}

			double ActorHeight = MostBottom - MostTop;
			double ActorWidth = MostRight - MostLeft;

			double DistanceDifference = 225.0 - Distance;
			double DistanceOffset = DistanceDifference * 0.045; // 0.035
			double CornerWidth = ActorWidth / 4;
			double CornerHeight = ActorHeight / 3;

			double TopTextOffset = 25.0;
			double BottomTextOffset = 14.0;

			FVector2D BottomMiddle = FVector2D(MostLeft + (ActorWidth / 2.0), MostBottom);

			float BoxThickness = (Distance < 30.0) ? 2.0f : 1.5f;
			float CornerThickness = (Distance < 30.0) ? 2.5f : 2.0f;

			if (Settings::Player::Skeleton)
			{
				Wrapper::Line(NeckScreen, ChestScreen, SkeletonColor, 2.0f);
				Wrapper::Line(ChestScreen, PelvisScreen, SkeletonColor, 2.0f);

				Wrapper::Line(ChestScreen, RShoulderScreen, SkeletonColor, 2.0f);
				Wrapper::Line(RShoulderScreen, RElbowScreen, SkeletonColor, 2.0f);
				Wrapper::Line(RElbowScreen, RHandScreen, SkeletonColor, 2.0f);

				Wrapper::Line(ChestScreen, LShoulderScreen, SkeletonColor, 2.0f);
				Wrapper::Line(LShoulderScreen, LElbowScreen, SkeletonColor, 2.0f);
				Wrapper::Line(LElbowScreen, LHandScreen, SkeletonColor, 2.0f);

				Wrapper::Line(PelvisScreen, RThighScreen, SkeletonColor, 2.0f);
				Wrapper::Line(RThighScreen, RCalfScreen, SkeletonColor, 2.0f);
				Wrapper::Line(RCalfScreen, RKneeScreen, SkeletonColor, 2.0f);
				Wrapper::Line(RKneeScreen, RFootScreen, SkeletonColor, 2.0f);

				Wrapper::Line(PelvisScreen, LThighScreen, SkeletonColor, 2.0f);
				Wrapper::Line(LThighScreen, LCalfScreen, SkeletonColor, 2.0f);
				Wrapper::Line(LCalfScreen, LKneeScreen, SkeletonColor, 2.0f);
				Wrapper::Line(LKneeScreen, LFootScreen, SkeletonColor, 2.0f);
			}

			//if (Settings::Player::Head)
			//{
			//	FVector Delta = Head - Variables::CameraLocation;
			//	double Distance = UKismetMathLibrary::sqrt(Delta.X * Delta.X + Delta.Y * Delta.Y + Delta.Z * Delta.Z);

			//	double Radius = 8.0 * (Variables::ScreenSize.X / (2.0 * Distance * UKismetMathLibrary::tan(Variables::FieldOfView * M_PI / 360.0)));

			//	Wrapper::Circle(HeadScreen, SkeletonColor, Radius, 138.0, 2.0f);
			//}

			if (Settings::Player::BoxType == 1)
			{
				Wrapper::Line(FVector2D(MostLeft - DistanceOffset, MostTop - DistanceOffset), FVector2D(MostRight + DistanceOffset, MostTop - DistanceOffset), BoxColor, BoxThickness);
				Wrapper::Line(FVector2D(MostLeft - DistanceOffset, MostBottom + DistanceOffset), FVector2D(MostRight + DistanceOffset, MostBottom + DistanceOffset), BoxColor, BoxThickness);
				Wrapper::Line(FVector2D(MostLeft - DistanceOffset, MostBottom + DistanceOffset), FVector2D(MostLeft - DistanceOffset, MostTop - DistanceOffset), BoxColor, BoxThickness);
				Wrapper::Line(FVector2D(MostRight + DistanceOffset, MostTop - DistanceOffset), FVector2D(MostRight + DistanceOffset, MostBottom + DistanceOffset), BoxColor, BoxThickness);
			}
			else if (Settings::Player::BoxType == 2)
			{
				Wrapper::Line(FVector2D(MostLeft - DistanceOffset, MostTop - DistanceOffset), FVector2D(MostLeft - DistanceOffset + CornerWidth, MostTop - DistanceOffset), BoxColor, CornerThickness);
				Wrapper::Line(FVector2D(MostLeft - DistanceOffset, MostTop - DistanceOffset), FVector2D(MostLeft - DistanceOffset, MostTop - DistanceOffset + CornerHeight), BoxColor, CornerThickness);

				Wrapper::Line(FVector2D(MostRight + DistanceOffset, MostTop - DistanceOffset), FVector2D(MostRight + DistanceOffset - CornerWidth, MostTop - DistanceOffset), BoxColor, CornerThickness);
				Wrapper::Line(FVector2D(MostRight + DistanceOffset, MostTop - DistanceOffset), FVector2D(MostRight + DistanceOffset, MostTop - DistanceOffset + CornerHeight), BoxColor, CornerThickness);

				Wrapper::Line(FVector2D(MostLeft - DistanceOffset, MostBottom + DistanceOffset), FVector2D(MostLeft - DistanceOffset + CornerWidth, MostBottom + DistanceOffset), BoxColor, CornerThickness);
				Wrapper::Line(FVector2D(MostLeft - DistanceOffset, MostBottom + DistanceOffset), FVector2D(MostLeft - DistanceOffset, MostBottom + DistanceOffset - CornerHeight), BoxColor, CornerThickness);

				Wrapper::Line(FVector2D(MostRight + DistanceOffset, MostBottom + DistanceOffset), FVector2D(MostRight + DistanceOffset - CornerWidth, MostBottom + DistanceOffset), BoxColor, CornerThickness);
				Wrapper::Line(FVector2D(MostRight + DistanceOffset, MostBottom + DistanceOffset), FVector2D(MostRight + DistanceOffset, MostBottom + DistanceOffset - CornerHeight), BoxColor, CornerThickness);
			}

			if (Settings::Player::LineType == 1)
				Wrapper::Line(BottomMiddle, FVector2D(Variables::ScreenCenter.X, Variables::ScreenCenter.Y * 2), BoxColor, 1.f);
			else if (Settings::Player::LineType == 2)
				Wrapper::Line(BottomMiddle, Variables::ScreenCenter, BoxColor, 1.f);
			else if (Settings::Player::LineType == 3)
				Wrapper::Line(HeadScreen, FVector2D(Variables::ScreenCenter.X, 0.0), BoxColor, 1.f);

			if (Settings::Player::Rank)
			{
				if (UKismetSystemLibrary::IsValid(PlayerState))
				{
					UFortPlayerStateComponent_Habanero* HabaneroComponent = PlayerState->HabaneroComponent();
					if (UKismetSystemLibrary::IsValid(HabaneroComponent))
					{
						int32_t RankProgress = HabaneroComponent->GetRankedProgress().Rank;
						Wrapper::Text(Custom::GetRankByProgress(RankProgress), FVector2D(BottomMiddle.X, MostTop - TopTextOffset), Custom::GetColorByRank(RankProgress), true);
						TopTextOffset += 14.0;
					}
				}
			}

			if (Settings::Player::Platform)
			{
				if (UKismetSystemLibrary::IsValid(PlayerState))
				{
					FString Platform = PlayerState->Platform();
					Wrapper::Text(Custom::GetNameByPlatform(Platform), FVector2D(BottomMiddle.X, MostTop - TopTextOffset), Custom::GetColorByPlatform(Platform), true);
					TopTextOffset += 14.0;
				}
			}

			if (Settings::Player::Name)
			{
				if (UKismetSystemLibrary::IsValid(PlayerState))
				{
					FString Username = PlayerState->GetPlayerName();
					FString ConvertedText = UKismetStringLibrary::BuildString_Int(Username, L" (", int(PlayerState->SeasonLevelUIDisplay()), L")");
					ConvertedText = PlayerState->IsABot() ? FString(L"BOT/NPC") : ConvertedText;
					Wrapper::Text(ConvertedText, FVector2D(BottomMiddle.X, MostTop - TopTextOffset), BoxColor, true, Settings::Misc::TextOutline, false);
					TopTextOffset += 14.0;
				}
			}

			if (Settings::Player::Distance)
			{
				FString ConvertedText = UKismetStringLibrary::BuildString_Double(L"", L"(", int(Distance), L"m)");
				Wrapper::Text(ConvertedText, FVector2D(BottomMiddle.X, MostBottom + BottomTextOffset), FLinearColor(0.8f, 0.8f, 0.f, 1.f), true);
				BottomTextOffset += 14.0;
			}

			if (Settings::Player::Weapon)
			{
				AFortWeapon* CurrentWeapon = Player->CurrentWeapon();
				if (UKismetSystemLibrary::IsValid(CurrentWeapon))
				{
					UFortItemDefinition* WeaponData = CurrentWeapon->WeaponData();
					if (UKismetSystemLibrary::IsValid(WeaponData))
					{
						if (UGameplayStatics::ObjectIsA(CurrentWeapon, Classes::FortWeaponClass))
						{
							bool IsHoldingPickaxe = UGameplayStatics::ObjectIsA(CurrentWeapon, Classes::PickaxeClass);
							if (IsHoldingPickaxe)
							{
								Wrapper::Text(L"Pickaxe", FVector2D(BottomMiddle.X, MostBottom + BottomTextOffset), FLinearColor(1.f, 1.f, 1.f, 1.f), true);
							}
							else
							{
								int CurrentAmmo = CurrentWeapon->GetMagazineAmmoCount();
								int MaxAmmo = CurrentWeapon->GetBulletsPerClip();

								FString DisplayName = WeaponData->ItemName().Get();
								FString ConvertedText = UKismetStringLibrary::BuildString_Int(DisplayName, L" (", CurrentAmmo, L"/");
								ConvertedText = UKismetStringLibrary::BuildString_Int(ConvertedText, L"", MaxAmmo, L")");

								Wrapper::Text(ConvertedText, FVector2D(BottomMiddle.X, MostBottom + BottomTextOffset), Custom::GetColorByTier(WeaponData->Tier()), true);
							}
						}
					}
				}
			}
		}
	}

	if (Settings::Aimbot::Aimbot)
	{
		if (UKismetSystemLibrary::IsValid(AcknowledgedPawn))
		{
			AFortWeapon* CurrentWeapon = AcknowledgedPawn->CurrentWeapon();
			if (UKismetSystemLibrary::IsValid(CurrentWeapon))
			{
				bool IsHoldingWeapon = UGameplayStatics::ObjectIsA(CurrentWeapon, Classes::RangedWeaponClass);
				if (IsHoldingWeapon)
				{
					bool IsHoldingPickaxe = UGameplayStatics::ObjectIsA(CurrentWeapon, Classes::PickaxeClass);
					if (!IsHoldingPickaxe)
					{
						if (Settings::Aimbot::DrawFOV)
						{
							Wrapper::Circle(Variables::ScreenCenter, Settings::Colors::FieldOfView, AimbotFOV, 8.0);
						}

						FKey AimbotKey = Keys::RightMouseButton;

						if (Settings::Aimbot::KeyType == 1)
							AimbotKey = Keys::LeftMouseButton;
						else if (Settings::Aimbot::KeyType == 2)
							AimbotKey = Keys::LeftShift;

						bool AimbotKeyDown = PlayerController->IsInputKeyDown(AimbotKey) || PlayerController->IsInputKeyDown(Keys::GamepadLeftTrigger);

						APlayerPawn_Athena_C* Target = TargetPlayer;
						if (UKismetSystemLibrary::IsValid(Target))
						{
							USkeletalMeshComponent* Mesh = Target->Mesh();
							if (UKismetSystemLibrary::IsValid(Mesh))
							{
								static Bones TargetBoneIndex = Bones::head;

								if (Settings::Aimbot::BoneType == 1)
									TargetBoneIndex = Bones::neck_02;
								else if (Settings::Aimbot::BoneType == 2)
									TargetBoneIndex = Bones::spine_05;
								else if (Settings::Aimbot::BoneType == 3) {
									static double LastBoneChangeTime = 0.00;
									const Bones BoneArray[] = {
										Bones::neck_01,
										Bones::neck_02,
										Bones::spine_01,
										Bones::spine_02,
										Bones::spine_03,
										Bones::clavicle_r,
										Bones::clavicle_l
									};

									const int NumBones = sizeof(BoneArray) / sizeof(Bones);

									if (UKismetSystemLibrary::GetGameTimeInSeconds(GWorld) - LastBoneChangeTime >= 1.00)
									{
										int32_t RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, NumBones - 1);
										TargetBoneIndex = BoneArray[RandomIndex];

										LastBoneChangeTime = UKismetSystemLibrary::GetGameTimeInSeconds(GWorld);
									}
								}

								FVector2D TargetScreen = FVector2D();
								FVector TargetWorld = Mesh->GetBoneLocation(TargetBoneIndex);
								if (PlayerController->ProjectWorldLocationToScreen(TargetWorld, &TargetScreen))
								{
									if (Settings::Aimbot::TargetLine)
									{
										FVector2D MuzzleScreen = FVector2D();
										FVector Muzzle = CurrentWeapon->GetMuzzleLocation();
										if (PlayerController->ProjectWorldLocationToScreen(Muzzle, &MuzzleScreen))
										{
											Wrapper::Line(MuzzleScreen, TargetScreen, Settings::Colors::TargetLine, 1.f);
										}
									}

									if (Settings::Aimbot::Triggerbot)
									{
										bool HoldingShotgun = CurrentWeapon->WeaponCoreAnimation() == EFortWeaponCoreAnimation::Shotgun;
										bool OnlyShotgun = (Settings::Aimbot::OnlyShotgun && HoldingShotgun) || (!Settings::Aimbot::OnlyShotgun && AimbotKeyDown);
										if (CurrentWeapon->CanFire() && CurrentWeapon->GetMagazineAmmoCount() > 0 && PlayerController->TargetedFortPawn() && OnlyShotgun)
										{
											AcknowledgedPawn->PawnStartFire();
											AcknowledgedPawn->PawnStopFire();
										}
									}

									double Distance = UKismetMathLibrary::Vector_Distance(Variables::CameraLocation, TargetWorld) * 0.01;

									if (Settings::Aimbot::PredictProjectiles && (!Settings::Aimbot::SilentAimbot && !Settings::Exploits::BulletTeleport))
									{
										FVector CurrentVelocity = Target->GetVelocity();
										UFortItemDefinition* WeaponData = CurrentWeapon->WeaponData();

										FString DisplayName = WeaponData->ItemName().Get();
										double ProjectileSpeed = Custom::GetProjectileSpeed(DisplayName);

										double ProjectileGravity = Custom::GetProjectileGravity(DisplayName);
										double TimeToTarget = (Distance / ProjectileSpeed) * 120.0;

										TargetWorld += FVector(CurrentVelocity.X * TimeToTarget, CurrentVelocity.Y * TimeToTarget, CurrentVelocity.Z * TimeToTarget - UKismetMathLibrary::abs(ProjectileGravity * -890.0) * (TimeToTarget * TimeToTarget) * 0.5);
									}

									if (Settings::Exploits::BulletTeleport) // no clip
									{
										Update__GetTargetingSourceLocation__bSilentAimActive = true;
										Update__GetTargetingSourceLocation__SilentLocationTarget = TargetWorld;
									}
									else if (Settings::Aimbot::SilentAimbot)
									{
										Update__GetWeaponTargetingTransform__bSilentAimActive = true;
										Update__GetWeaponTargetingTransform__SilentLocationTarget = TargetWorld;
									}
									else if (Settings::Aimbot::StickySilentAim && AimbotKeyDown)
									{
										IsCurrentlySickySilentAiming = true;

										GetPlayerViewPoint_bShouldStartAtLocationTarget = true;
										GetPlayerViewPoint_SilentLocationTarget = TargetWorld;
									}
									else if (Settings::Aimbot::Aimbot && AimbotKeyDown)
									{
										FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Variables::CameraLocation, TargetWorld);
										Variables::TargetRotation = TargetRotation;

										if (Settings::Aimbot::ShakeSpeed >= 1.0)
										{
											double CurrentTime = UGameplayStatics::GetTimeSeconds(GWorld);

											int32_t RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, (int32_t)Settings::Aimbot::ShakeSpeed);
											double ShakeFactor = UKismetMathLibrary::sin(CurrentTime * RandomIndex);
											double DistanceFactor = 1.0 / UKismetMathLibrary::FClamp(Distance / 100.0, 1.0, 7.0);

											TargetRotation.Pitch += ShakeFactor * DistanceFactor;
											TargetRotation.Yaw -= ShakeFactor * DistanceFactor;
										}

										if (Settings::Aimbot::Smoothing >= 1.0)
										{
											double WorldDeltaSeconds = UGameplayStatics::GetWorldDeltaSeconds(GWorld);

											if (Settings::Aimbot::SmoothingType == 2)
												TargetRotation = UKismetMathLibrary::RInterpTo(Variables::CameraRotation, TargetRotation, WorldDeltaSeconds, Settings::Aimbot::Smoothing);
											else if (Settings::Aimbot::SmoothingType == 3)
												TargetRotation = UKismetMathLibrary::RInterpTo_Constant(Variables::CameraRotation, TargetRotation, WorldDeltaSeconds, Settings::Aimbot::Smoothing);

											TargetRotation += (TargetRotation - Variables::CameraRotation);
										}

										if (Settings::Aimbot::SmoothingType == 1)
										{
											FVector2D CameraScreen = FVector2D();
											PlayerController->ProjectWorldLocationToScreen(Variables::CameraLocation, &CameraScreen);

											FVector2D PreviousPoint = CameraScreen;
											FVector2D ControlPoint = Custom::CalculateControlPoint(CameraScreen, TargetScreen, Settings::Aimbot::Smoothing);

											for (int i = 1; i <= 20; ++i)
											{
												float t = i / float(20);

												double CurrentPointX = (1 - t) * (1 - t) * CameraScreen.X + 2 * (1 - t) * t * ControlPoint.X + t * t * TargetScreen.X;
												double CurrentPointY = (1 - t) * (1 - t) * CameraScreen.Y + 2 * (1 - t) * t * ControlPoint.Y + t * t * TargetScreen.Y;
												FVector2D CurrrntPointScreen = FVector2D(CurrentPointX, CurrentPointY);

												FVector CurrentPoint = FVector();
												PlayerController->DeprojectScreenPositionToWorld(CurrrntPointScreen, &CurrentPoint);
												FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Variables::CameraLocation, CurrentPoint);
												Custom::MemoryAim(CurrentWeapon, TargetRotation);

												PreviousPoint = CurrrntPointScreen;
											}
										}
										else
											Custom::MemoryAim(CurrentWeapon, TargetRotation);
									}
								}
							}
						}

						if (Settings::Exploits::Spinbot && AimbotKeyDown)
						{
							Update_SpinningRightNow = true;

							PlayerController->AddYawInput((float)Settings::Exploits::SpinSpeed);
						}

						if (SpinningRightNow)
						{
							SpinningRightNow = false;

							float YawAngle = (float)(Variables::TargetRotation.Yaw - Variables::CameraRotation.Yaw) / double(PlayerController->InputYawScale());
							PlayerController->AddYawInput(YawAngle);
						}

						SpinningRightNow = Update_SpinningRightNow;

						Variables::GetTargetingSourceLocation::bSilentAimActive = Update__GetTargetingSourceLocation__bSilentAimActive;
						Variables::GetTargetingSourceLocation::SilentLocationTarget = Update__GetTargetingSourceLocation__SilentLocationTarget;

						Variables::GetWeaponTargetingTransform::bSilentAimActive = Update__GetWeaponTargetingTransform__bSilentAimActive;
						Variables::GetWeaponTargetingTransform::SilentLocationTarget = Update__GetWeaponTargetingTransform__SilentLocationTarget;

						Variables::GetPlayerViewPoint::bShouldStartAtLocationTarget = GetPlayerViewPoint_bShouldStartAtLocationTarget;
						Variables::GetPlayerViewPoint::SilentLocationTarget = GetPlayerViewPoint_SilentLocationTarget;

						if (Settings::Misc::CrosshairMode)
						{
							if (Settings::Misc::CrosshairMode == 1)
							{
								Wrapper::Line(FVector2D(Variables::ScreenCenter.X - 10, Variables::ScreenCenter.Y), FVector2D(Variables::ScreenCenter.X + 10, Variables::ScreenCenter.Y), FLinearColor(0.6f, 0.6f, 0.6f, 1.f), 1.f);
								Wrapper::Line(FVector2D(Variables::ScreenCenter.X, Variables::ScreenCenter.Y - 10), FVector2D(Variables::ScreenCenter.X, Variables::ScreenCenter.Y + 10), FLinearColor(0.6f, 0.6f, 0.6f, 1.f), 1.f);
							}
							else if (Settings::Misc::CrosshairMode == 2)
							{
								double CrosshairOuter = double(14);
								double CrosshairInner = double(8);

								Wrapper::Line(FVector2D(Variables::ScreenCenter.X - CrosshairOuter, Variables::ScreenCenter.Y), FVector2D(Variables::ScreenCenter.X - CrosshairInner, Variables::ScreenCenter.Y), FLinearColor(0.6f, 0.6f, 0.6f, 1.f), 2.f);
								Wrapper::Line(FVector2D(Variables::ScreenCenter.X + CrosshairInner, Variables::ScreenCenter.Y), FVector2D(Variables::ScreenCenter.X + CrosshairOuter, Variables::ScreenCenter.Y), FLinearColor(0.6f, 0.6f, 0.6f, 1.f), 2.f);
								Wrapper::Line(FVector2D(Variables::ScreenCenter.X, Variables::ScreenCenter.Y - CrosshairOuter), FVector2D(Variables::ScreenCenter.X, Variables::ScreenCenter.Y - CrosshairInner), FLinearColor(0.6f, 0.6f, 0.6f, 1.f), 2.f);
								Wrapper::Line(FVector2D(Variables::ScreenCenter.X, Variables::ScreenCenter.Y + CrosshairInner), FVector2D(Variables::ScreenCenter.X, Variables::ScreenCenter.Y + CrosshairOuter), FLinearColor(0.6f, 0.6f, 0.6f, 1.f), 2.f);
							}
							else if (Settings::Misc::CrosshairMode == 3)
							{
								Wrapper::Circle(Variables::ScreenCenter, FLinearColor(0.6f, 0.6f, 0.6f, 1.f), 5.0, 38.0, true);
							}
							else if (Settings::Misc::CrosshairMode == 4)
							{
								Wrapper::Line(FVector2D(Variables::ScreenCenter.X - 10, Variables::ScreenCenter.Y - 10), FVector2D(Variables::ScreenCenter.X + 10, Variables::ScreenCenter.Y + 10), FLinearColor(0.6f, 0.6f, 0.6f, 1.f), 1.f);
								Wrapper::Line(FVector2D(Variables::ScreenCenter.X - 10, Variables::ScreenCenter.Y + 10), FVector2D(Variables::ScreenCenter.X + 10, Variables::ScreenCenter.Y - 10), FLinearColor(0.6f, 0.6f, 0.6f, 1.f), 1.f);

								Wrapper::Line(FVector2D(Variables::ScreenCenter.X - 10, Variables::ScreenCenter.Y - 10), FVector2D(Variables::ScreenCenter.X - 5, Variables::ScreenCenter.Y - 5), FLinearColor(0.f, 0.f, 0.f, 1.f), 2.f);
								Wrapper::Line(FVector2D(Variables::ScreenCenter.X - 10, Variables::ScreenCenter.Y + 10), FVector2D(Variables::ScreenCenter.X - 5, Variables::ScreenCenter.Y + 5), FLinearColor(0.f, 0.f, 0.f, 1.f), 2.f);
								Wrapper::Line(FVector2D(Variables::ScreenCenter.X + 10, Variables::ScreenCenter.Y + 10), FVector2D(Variables::ScreenCenter.X + 5, Variables::ScreenCenter.Y + 5), FLinearColor(0.f, 0.f, 0.f, 1.f), 2.f);
								Wrapper::Line(FVector2D(Variables::ScreenCenter.X + 10, Variables::ScreenCenter.Y - 10), FVector2D(Variables::ScreenCenter.X + 5, Variables::ScreenCenter.Y - 5), FLinearColor(0.f, 0.f, 0.f, 1.f), 2.f);
							}
						}
					}
				}
			}
		}
	}

	bool ViewportIsA = UGameplayStatics::ObjectIsA(ViewportClient, Classes::GameViewportClient);
	if (ViewportIsA)
	{
		static bool DisableRMU = false;

		if (Settings::Environment::DisableRMU)
		{
			*(uintptr_t*)(ViewportClient + 0xB0) = 1;
			DisableRMU = true;
		}
		else if (DisableRMU)
		{
			*(uintptr_t*)(ViewportClient + 0xB0) = 0;
			DisableRMU = false;
		}
	}

	if (Settings::Exploits::Hook)
	{
		Rehook(LocalPlayer);
	}

	Wrapper::Text(FString(L"MakFN Internal"), FVector2D(50, 50), FLinearColor(0.906f, 0.000f, 0.486f, 1.0f), false, true, true, GameFont);

	if (PlayerController->WasInputKeyJustPressed(Keys::F8)) Variables::DrawMenu = !Variables::DrawMenu;
	if (Variables::DrawMenu) Framework::DrawMenu();

	Wrapper::Render();
	Wrapper::ClearTargets();

	return DrawTransitionOriginal(ViewPortClient, Canvas);
}