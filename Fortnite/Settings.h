#pragma once

namespace Settings
{
	namespace Aimbot
	{
		bool Aimbot = true;
		bool Triggerbot = false;
		double Smoothing = 8.0;
		bool HumanError = true;
		bool OnlyShotgun = false;
		bool TargetLine = true;
		double Speed = 10.0;
		bool PredictProjectiles = false;
		double FOV = 50.0;
		bool DrawFOV = true;
		double MaxDistance = 100.0;
		bool StickySilentAim = false;
		bool SilentAimbot = false;
		int BoneType = 0;
		int KeyType = 0;
		int SmoothingType = 1;
		bool SkipKnocked = false;
		bool VisableOnly = false;
		double ShakeSpeed = 2.0;

		namespace StickySilent
		{
			bool StickyCameraSpin = false;
		}
	}

	namespace Player
	{
		int BoxType = 1;
		bool Skeleton = false;
		int LineType = 0;
		bool Name = false;
		bool Rank = false;
		bool Weapon = false;
		bool Distance = false;
		bool Radar = false;
		bool Minimap = false;
		bool RadarLines = false;
		bool Platform = false;
		bool Head = false;

		double MaxDistance = 350.0;

		double RadarX = 50.0;
		double RadarY = 80.0;
	}

	namespace Environment
	{
		bool Weakspot = false;
		bool Projectile = false;
		bool Pickup = false;
		bool Container = false;
		bool SupplyDrone = false;
		bool Vehicle = false;
		bool SupplyDrop = false;
		bool WeakspotAim = false;
		bool DisableRMU = false;
		bool Chams = false;
		bool ChestChams = false;
		double MaxDistance = 100.0;
	}

	namespace Exploits
	{
		bool Hook = false;
		bool CarFly = false;
		bool InfiniteFuel = false;
		bool PlayerFly = false;
		double CarSpeed = 15.0;
		bool CameraFOVChanger = false;
		double CameraFOV = 80.0;
		double Spread = 10.0;
		bool UnlockFireRate = false;
		bool NoSpread = false;
		bool NoRecoil = false;
		bool InstantADS = false;
		bool Chams = false;
		bool Wireframe = false;
		bool Glow = false;
		bool PerfectBHop = false;
		bool InfiniteStamina = false;
		bool Spinbot = false;
		double SpinSpeed = 5.0;
		bool MagicWeapon = false;
		bool BulletTeleport = false;
	}

	namespace Misc
	{
		int CrosshairMode = 1;
		bool StormPrediction = false;
		bool TextOutline = false;
		bool DebugObjects = false;
	}

	namespace Colors
	{
		FLinearColor TargetLine = FLinearColor(1.f, 0.f, 0.f, 1.f);
		FLinearColor FieldOfView = FLinearColor(1.f, 1.f, 1.f, 1.f);
		FLinearColor TeammateColor = FLinearColor(0.8f, 0.5f, 0.f, 1.f);
		FLinearColor BoxVisible = FLinearColor(0.631f, 0.431f, 0.8f, 1.0f);
		FLinearColor BoxInVisible = FLinearColor(0.906f, 0.000f, 0.486f, 1.0f);
		FLinearColor SkeletonVisible = FLinearColor(0.631f, 0.431f, 0.8f, 1.0f);
		FLinearColor SkeletonInVisible = FLinearColor(0.906f, 0.000f, 0.486f, 1.0f);
		FLinearColor SnaplineVisible = FLinearColor(0.631f, 0.431f, 0.8f, 1.0f);
		FLinearColor SnaplineInVisible = FLinearColor(0.906f, 0.000f, 0.486f, 1.0f);
		FLinearColor SkinColor = FLinearColor(1.f, 1.f, 0.f, 1.f);
	}
}