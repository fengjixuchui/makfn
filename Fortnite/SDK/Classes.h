#pragma once

class UClass {
public:
	BYTE _padding_0[0x40];
	UClass* SuperClass;
};

class UObject1
{
public:
	void** VFTable;
	uint32_t ObjectFlags;
	uint32_t InternalIndex;
	UObject1* ClassPrivate;
	FName NamePrivate;
	UObject1* OuterPrivate;

	FName GetName()
	{
		return NamePrivate;
	}
};

class UObject
{
public:
	UObject* GetOuterObject()
	{
		return *(UObject**)(this + 0x20);
	}

	void ProcessEvent(UObject* UFunction, void* Params)
	{
		if (auto VTable = *(void***)this)
		{
			reinterpret_cast<void(__cdecl*)(UObject*, UObject*, void*, uintptr_t, void*)>(DoSpoofCall)(this, UFunction, Params, SpoofCode, VTable[Offsets::ProcessEvent]);
		}
	}

	static UObject* StaticFindObject(UObject* Class, UObject* Outer, const wchar_t* Name, bool ExactClass)
	{
		return reinterpret_cast<UObject* (__cdecl*)(UObject*, UObject*, const wchar_t*, bool, uintptr_t, void*)>(DoSpoofCall)(Class, Outer, Name, ExactClass, SpoofCode, (void*)(FortniteClientWin64Shipping + Offsets::StaticFindObject));
	}

	template <class Type>
	static Type FindObjectSingle(const wchar_t* Name, UObject* Outer = nullptr)
	{
		return reinterpret_cast<Type>(UObject::StaticFindObject(nullptr, Outer, Name, false));
	}
};

void* TUObjectArray::GetObjectPtr(uint32_t id) const
{
	if (id >= NumElements) return nullptr;
	uint64_t chunkIndex = id / 65536;
	if (chunkIndex >= NumChunks) return nullptr;
	auto chunk = Objects[chunkIndex];
	if (!chunk) return nullptr;
	uint32_t withinChunkIndex = id % 65536 * 24;
	UObject* item = *reinterpret_cast<UObject**>(chunk + withinChunkIndex);
	return item;
}

namespace StaticClasses
{
	static UObject* FortKismetLibrary = nullptr;
	static UObject* KismetSystemLibrary = nullptr;
	static UObject* KismetMathLibrary = nullptr;
	static UObject* GameplayStatics = nullptr;
	static UObject* KismetStringLibrary = nullptr;
}

class UFont : public UObject
{
public:
	FName LegacyFontName()
	{
		return *(FName*)(this + 0x140);
	}
};

UObject* KismetMathLibrary = nullptr;
UObject* GameplayStatics = nullptr;
UObject* FortKismetLibrary = nullptr;
UObject* KismetSystemLibrary = nullptr;
UObject* KismetStringLibrary = nullptr;
UFont* MediumFont = 0;
UFont* GameFont = 0;

class UTexture : public UObject
{
public:

};

class GenericApplication : public UObject
{
public:

};

class UEngine : public UObject
{
public:
	UFont* MediumFont()
	{
		return *(UFont**)(this + 0x70);
	}

	TArray<UFont*> AdditionalFonts()
	{
		return *(TArray<UFont*>*)(this + 0x60);
	}
};

class AHUD : public UObject
{
public:

};

class UCanvas : public UObject
{
public:
	float ClipX()
	{
		return *(float*)(this + 0x30);
	}

	float ClipY()
	{
		return *(float*)(this + 0x34);
	}

	void K2_DrawLine(FVector2D ScreenPositionA, FVector2D ScreenPositionB, float Thickness, FLinearColor RenderColor)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"Canvas.K2_DrawLine");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FVector2D ScreenPositionA;
			FVector2D ScreenPositionB;
			float Thickness;
			FLinearColor RenderColor;
		} Params = { ScreenPositionA, ScreenPositionB, Thickness, RenderColor };

		this->ProcessEvent(Function, &Params);
	}

	void K2_DrawText(UFont* Font, FString RenderText, FVector2D ScreenPosition, FVector2D Scale, FLinearColor RenderColor, float Kerning, bool bCentreX, bool bCentreY, bool bOutlined)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"Canvas.K2_DrawText");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			UObject* RenderFont;
			FString RenderText;
			FVector2D ScreenPosition;
			FVector2D Scale;
			FLinearColor RenderColor;
			float Kerning;
			FLinearColor ShadowColor;
			FVector2D ShadowOffset;
			bool bCentreX;
			bool bCentreY;
			bool bOutlined;
			FLinearColor OutlineColor;
		} Params = { MediumFont, RenderText, ScreenPosition, Scale, RenderColor, Kerning, FLinearColor(), FVector2D(), bCentreX, bCentreY, bOutlined, FLinearColor(0.f, 0.f, 0.f, 1.f) };

		this->ProcessEvent(Function, &Params);
	}

	void K2_DrawPolygon(UTexture* RenderTexture, FVector2D ScreenPosition, FVector2D Radius, int32_t NumberOfSides, FLinearColor RenderColor)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"Canvas.K2_DrawPolygon");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			UTexture* RenderTexture;
			FVector2D ScreenPosition;
			FVector2D Radius;
			int32_t NumberOfSides;
			FLinearColor RenderColor;
		} Params = { RenderTexture, ScreenPosition, Radius, NumberOfSides, RenderColor };

		this->ProcessEvent(Function, &Params);
	}
};

class UActorComponent : public UObject
{
public:

};

class UCameraComponent : public UActorComponent
{
public:

};

class AActor : public UObject
{
public:
	FVector GetVelocity()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"Actor.GetVelocity");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FVector ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool K2_TeleportTo(FVector DestLocation, FRotator DestRotation)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"Actor.K2_TeleportTo");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FVector DestLocation;
			FRotator DestRotation;
			bool ReturnValue;
		} Params = { DestLocation, DestRotation };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool K2_SetActorLocationAndRotation(FVector NewLocation, FRotator NewRotation)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"Actor.K2_SetActorLocationAndRotation");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FVector NewLocation;
			FRotator NewRotation;
			bool bSweep;
			FHitResult SweepHitResult;
			bool bTeleport;
			bool ReturnValue;
		} Params = { NewLocation, NewRotation, false, FHitResult(), true };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FVector K2_GetActorLocation()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"Actor.K2_GetActorLocation");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FVector ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FRotator K2_GetActorRotation()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"Actor.K2_GetActorRotation");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FRotator ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetGameTimeSinceCreation()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"Actor.GetGameTimeSinceCreation");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class APlayerCameraManager : public AActor
{
public:
	float GetFOVAngle()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PlayerCameraManager.GetFOVAngle");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FVector GetCameraLocation()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PlayerCameraManager.GetCameraLocation");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FVector ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FRotator GetCameraRotation()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PlayerCameraManager.GetCameraRotation");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FRotator ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UFortPlayerStateComponent_Habanero : public AActor
{
public:
	FRankedProgressReplicatedData GetRankedProgress()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortPlayerStateComponent_Habanero.GetRankedProgress");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FRankedProgressReplicatedData ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class AFortPlayerStateAthena : public AActor 
{
public:
	int32_t SeasonLevelUIDisplay()
	{
		return *(int32_t*)(this + 0x1268);
	}
};

class AFortPlayerState : public AFortPlayerStateAthena
{
public:
	UFortPlayerStateComponent_Habanero* HabaneroComponent()
	{
		return *(UFortPlayerStateComponent_Habanero**)(this + 0xa30);
	}

	FString Platform()
	{
		return *(FString*)(this + 0x438);
	}
};

class APlayerState : public AFortPlayerState
{
public:
	FString GetPlayerName()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PlayerState.GetPlayerName");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FString ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool IsABot()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PlayerState.IsABot");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FString ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UMaterial : public UObject
{
public:

};

class UMaterialInterface : public UMaterial
{
public:
	UMaterial* GetMaterial()
	{
		if (auto VTable = *(void***)this)
		{
			return reinterpret_cast<UMaterial * (__cdecl*)(UMaterialInterface*, uintptr_t, void*)>(DoSpoofCall)(this, SpoofCode, VTable[Offsets::GetMaterial]);
		}

		return nullptr;
	}
};

class UMaterialInstance : public UMaterialInterface
{
public:
	TArray<FVectorParameterValue> VectorParameterValues()
	{
		return *(TArray<FVectorParameterValue>*)(this + 0x170);
	}

	TArray<FScalarParameterValue> ScalarParameterValues()
	{
		return *(TArray<FScalarParameterValue>*)(this + 0x160);
	}
};

class UPrimitiveComponent : public UObject
{
public:
	// this + 0x280
	void SetCustomDepthStencilValue(int Value)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PrimitiveComponent.SetCustomDepthStencilValue");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			int Value;
		} Params = { Value };

		this->ProcessEvent(Function, &Params);
	}

	// *(_BYTE *)(this + 0x251) = (true << 6) | *(_BYTE *)(this + 0x251) & 0xBF;
	void SetRenderCustomDepth(bool bValue)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PrimitiveComponent.SetRenderCustomDepth");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			bool bValue;
		} Params = { bValue };

		this->ProcessEvent(Function, &Params);
	}

	UMaterialInterface* GetMaterial(int32_t ElementIndex)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PrimitiveComponent.GetMaterial");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			UMaterialInterface* ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	int32_t GetNumMaterials()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PrimitiveComponent.GetNumMaterials");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			int32_t ReturnValue;
		} Params = { 0 };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void SetMaterial(int32_t ElementIndex, UMaterial* Material)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PrimitiveComponent.SetMaterial");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			int32_t ElementIndex;
			UMaterial* Material;
		} Params = { ElementIndex, Material };

		this->ProcessEvent(Function, &Params);
	}
};

class UMeshComponent : public UPrimitiveComponent
{
public:
	void SetVectorParameterValueOnMaterials(FName ParameterName, FVector ParameterValue)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"MeshComponent.SetVectorParameterValueOnMaterials");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FName ParameterName;
			FVector ParameterValue;
		} Params = { ParameterName, ParameterValue };

		this->ProcessEvent(Function, &Params);
	}

	TArray<UMaterialInterface*> GetMaterials()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"MeshComponent.GetMaterials");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			TArray<UMaterialInterface*> ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class USkinnedMeshComponent : public UMeshComponent
{
public:
	FMatrix GetBoneMatrix(int BoneIndex)
	{
		FMatrix OutMatrix;
		reinterpret_cast<FMatrix* (__cdecl*)(USkinnedMeshComponent*, FMatrix*, int, uintptr_t, void*)>(DoSpoofCall)(this, &OutMatrix, BoneIndex, SpoofCode, (void*)(FortniteClientWin64Shipping + Offsets::GetBoneMatrix));

		return OutMatrix;
	}

	FVector GetBoneLocation(int BoneIndex)
	{
		FMatrix BoneMatrix = this->GetBoneMatrix(BoneIndex);

		return FVector(BoneMatrix.WPlane.X, BoneMatrix.WPlane.Y, BoneMatrix.WPlane.Z);
	}

	int GetNumBones()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"SkinnedMeshComponent.GetNumBones");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			int ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class USkeletalMeshComponent : public USkinnedMeshComponent
{
public:

};

class UMaterialInstanceDynamic : public UMaterialInstance
{
public:
	void SetVectorParameterValueByInfo(FMaterialParameterInfo& ParameterInfo, FLinearColor Value)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"MaterialInstanceDynamic.SetVectorParameterValueByInfo");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FMaterialParameterInfo& ParameterInfo;
			FLinearColor Value;
		} Params = { ParameterInfo, Value };

		this->ProcessEvent(Function, &Params);
	}

	void SetVectorParameterValue(FName ParameterName, FLinearColor Value)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"MaterialInstanceDynamic.SetVectorParameterValue");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FName ParameterName;
			FLinearColor Value;
		} Params = { ParameterName, Value };

		this->ProcessEvent(Function, &Params);
	}

	FLinearColor K2_GetVectorParameterValue(FName ParameterName)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"MaterialInstanceDynamic.K2_GetVectorParameterValue");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FName ParameterName;
			FLinearColor ReturnValue;
		} Params = { ParameterName };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void SetScalarParameterValue(FName ParameterName, float Value)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"MaterialInstanceDynamic.SetScalarParameterValue");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		} 

		struct {
			FName ParameterName;
			float Value;
		} Params = { ParameterName, Value };

		this->ProcessEvent(Function, &Params);
	}

	float K2_GetScalarParameterValue(FName ParameterName)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"MaterialInstanceDynamic.K2_GetScalarParameterValue");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FName ParameterName;
			float ReturnValue;
		} Params = { ParameterName };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UFortMovementComp_Character : public UObject
{
public:
	char pad_0[0x14bc];
	bool bWantsToSwing;

	char pad_1[0x3];
	bool bSwingInstantVelocity;

	char pad_2[0x1];
	FVector SwingAttachLocation;

	char pad_3[0x18];
	FVector SwingLaunch;
};

// Wrapper
class UCharacterMovementComponent : public UFortMovementComp_Character
{
public:

};

class UItemDefinitionBase : public UObject
{
public:
	FText ItemName()
	{
		return *(FText*)(this + 0x40);
	}
};

class UFortItemDefinition : public UItemDefinitionBase
{
public:
	EFortItemTier Tier()
	{
		return *(EFortItemTier*)(this + 0x13c);
	}
};

class UFortWeaponItemDefinition : public UFortItemDefinition
{
public:

};

class AFortWeapon : public AActor
{
public:
	UFortWeaponItemDefinition* WeaponData()
	{
		return *(UFortWeaponItemDefinition**)(this + 0x4f0);
	}

	EFortWeaponCoreAnimation WeaponCoreAnimation()
	{
		return *(EFortWeaponCoreAnimation*)(this + 0x1260);
	}

	bool CanFire()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortWeapon.CanFire");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			bool bPrimaryFire;
			bool ReturnValue;
		} Params = { true };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetReloadProgress()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortWeapon.GetReloadProgress");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		} 

		struct {
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetFiringRate()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortWeapon.GetFiringRate");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetReloadTime()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortWeapon.GetReloadTime");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool IsProjectileWeapon()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortWeapon.IsProjectileWeapon");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	FVector GetMuzzleLocation()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortWeapon.GetMuzzleLocation");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			int PatternIndex;
			FVector ReturnValue;
		} Params{ 0 };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	int GetBulletsPerClip()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortWeapon.GetBulletsPerClip");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			int ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool IsTargeting()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortWeapon.IsTargeting");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool IsReloading()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortWeapon.IsReloading");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct
		{
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	int GetMagazineAmmoCount()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortWeapon.GetMagazineAmmoCount");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			int ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetLastFireTime()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortWeapon.GetLastFireTime");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	int GetRemainingAmmo()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortWeapon.GetRemainingAmmo");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			int ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetChargePercent()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortWeapon.GetChargePercent");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	float GetProjectileSpeed(float ChargePercent)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortWeapon.GetProjectileSpeed");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			float ChargePercent;
			float ReturnValue;
		} Params = { ChargePercent };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class AFortWeaponRanged : public AFortWeapon
{
public:

};

class UFortVehicleFuelComponent : public AActor
{
public:
	char pad_0[0x260];
	float ServerFuel;

	char pad_1[0x170];
	float InfiniteFuel;

	float GetFuelCapacity()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortVehicleFuelComponent.GetFuelCapacity");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void SetFuel(float NewFuel)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortVehicleFuelComponent.SetFuel");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			bool NewFuel;
		} Params = { NewFuel };

		this->ProcessEvent(Function, &Params);
	}
};

class AFortAthenaVehicle : public AActor
{
public:
	UFortVehicleFuelComponent* CachedFuelComponent()
	{
		return *(UFortVehicleFuelComponent**)(this + 0x1268);
	}

	float GetSteeringAngle()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortAthenaVehicle.GetSteeringAngle");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			float ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class APawn : public AActor
{
public:
	APlayerState* PlayerState()
	{
		return *(APlayerState**)(this + 0x2b0);
	}
};

class ACharacter : public APawn
{
public:
	USkeletalMeshComponent* Mesh()
	{
		return *(USkeletalMeshComponent**)(this + 0x318);
	}

	UCharacterMovementComponent* CharacterMovement()
	{
		return *(UCharacterMovementComponent**)(this + 0x320);
	}

	bool CanJump()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"Character.CanJump");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class AFortPawn : public ACharacter
{
public:
	AFortWeapon* CurrentWeapon()
	{
		return *(AFortWeapon**)(this + 0xa68);
	}

	bool IsDead()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortPawn.IsDead");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	bool IsDBNO()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortPawn.IsDBNO");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	char GetTeam()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortPawn.GetTeam");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			char ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void PawnStartFire()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortPawn.PawnStartFire");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			char FireModeNum;
		} Params = { 1 };

		this->ProcessEvent(Function, &Params);
	}

	void PawnStopFire()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortPawn.PawnStopFire");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			char FireModeNum;
		} Params = { 1 };

		this->ProcessEvent(Function, &Params);
	}
};

class AFortPlayerPawn : public AFortPawn
{
public:
	char pad_0[0x2ed8];
	USkeletalMeshComponent* CharacterPartSkeletalMeshComponents[0x7];

	AFortAthenaVehicle* CurrentVehicle()
	{
		return *(AFortAthenaVehicle**)(this + 0x29b0);
	}

	bool IsInVehicle()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortPlayerPawn.IsInVehicle");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class AFortPlayerPawnAthena : public AFortPlayerPawn
{
public:

};

class APlayerPawn_Athena_Generic_Parent_C : public AFortPlayerPawnAthena
{
public:
	TArray <USkeletalMeshComponent*> SkeletalMeshes()
	{
		return *(TArray <USkeletalMeshComponent*>*)(this + 0x5cc0);
	}

	TArray <UMaterialInstanceDynamic*> PawnMaterials_ALL()
	{
		return *(TArray <UMaterialInstanceDynamic*>*)(this + 0x5d60);
	}
};

class APlayerPawn_Athena_Generic_C : public APlayerPawn_Athena_Generic_Parent_C
{
public:

};

class APlayerPawn_Athena_C : public APlayerPawn_Athena_Generic_C
{
public:

};

class AFortPickup : public AActor
{
public:
	UFortItemDefinition* PrimaryPickupItemEntry()
	{
		return *(UFortItemDefinition**)(this + 0x350);
	}
};

class UBaseBuildingStaticMeshComponent : public UMeshComponent
{
public:

};

class ABuildingSMActor : public AActor
{
public:
	UBaseBuildingStaticMeshComponent* StaticMeshComponent()
	{
		return *(UBaseBuildingStaticMeshComponent**)(this + 0x890);
	}
};

class ABuildingContainer : public ABuildingSMActor
{
public:
	char pad_0[0xd92];
	char bAlreadySearched : 1;
};

class ABuildingWeakSpot : public AActor
{
public:
	char pad_0[0x2c0];
	char bHit : 1;
	char bActive : 1;
};

class AFortAthenaSupplyDrop : public AActor
{
public:

};

class ABuildingTrap : public AActor
{
public:

};

class AAthenaSuperDingo : public AActor {
public:

};

class AController : public AActor
{
public:
	APlayerState* PlayerState()
	{
		return *(APlayerState**)(this + 0x298);
	}

	APlayerPawn_Athena_C* K2_GetPawn()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"Controller.K2_GetPawn");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct
		{
			APlayerPawn_Athena_C* ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class AFortPlayerController : public AController
{
public:
	AFortPawn* TargetedFortPawn()
	{
		return *(AFortPawn**)(this + 0x18a0);
	}
};

class APlayerController : public AFortPlayerController
{
public:
	APlayerCameraManager* PlayerCameraManager()
	{
		return *(APlayerCameraManager**)(this + 0x348);
	}

	APlayerPawn_Athena_C* AcknowledgedPawn()
	{
		return *(APlayerPawn_Athena_C**)(this + 0x338);
	}

	float InputYawScale()
	{
		return *(float*)(this + 0x538);
	}

	float InputPitchScale()
	{
		return *(float*)(this + 0x53c);
	}

	void AddYawInput(float Val)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PlayerController.AddYawInput");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			float Val;
		} Params = { Val };

		this->ProcessEvent(Function, &Params);
	}

	void AddPitchInput(float Val)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PlayerController.AddPitchInput");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			float Val;
		} Params = { Val };

		this->ProcessEvent(Function, &Params);
	}

	bool DeprojectScreenPositionToWorld(FVector2D& ScreenLocation, FVector* WorldLocation)
	{
		static UObject* Function;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PlayerController.ProjectWorldLocationToScreen");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			float ScreenX;
			float ScreenY;
			FVector WorldLocation;
			FVector WorldDirection;
			bool ReturnValue;
		} Params = { ScreenLocation.X, ScreenLocation.Y, FVector(), FVector() };

		this->ProcessEvent(Function, &Params);

		if (WorldLocation) *WorldLocation = Params.WorldLocation;

		return Params.ReturnValue && Params.WorldLocation;
	}

	bool ProjectWorldLocationToScreen(FVector WorldLocation, FVector2D* ScreenLocation)
	{
		static UObject* Function;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PlayerController.ProjectWorldLocationToScreen");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FVector WorldLocation;
			FVector2D ScreenLocation;
			bool bPlayerViewportRelative;
			bool ReturnValue;
		} Params = { WorldLocation, FVector2D(), false };

		this->ProcessEvent(Function, &Params);

		if (ScreenLocation) *ScreenLocation = FVector2D(int(Params.ScreenLocation.X), int(Params.ScreenLocation.Y));

		return Params.ReturnValue && Params.ScreenLocation;
	}

	AHUD* GetHUD()
	{
		static UObject* Function;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PlayerController.GetHUD");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct
		{
			AHUD* ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void FOV(float NewFOV)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PlayerController.FOV");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			float NewFOV;
		} Params = { NewFOV };

		this->ProcessEvent(Function, &Params);
	}

	FVector2D GetMousePosition()
	{
		static UObject* Function;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PlayerController.GetMousePosition");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct
		{
			float LocationX;
			float LocationY;
			bool ReturnValue;
		} Params = { };

		this->ProcessEvent(Function, &Params);

		return FVector2D(double(Params.LocationX), double(Params.LocationY));
	}

	bool IsInputKeyDown(FKey key)
	{
		static UObject* Function;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PlayerController.IsInputKeyDown");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct
		{
			FKey key;
			bool ReturnValue;
		} Params = { key };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void ClientIgnoreMoveInput(bool bIgnore)
	{
		static UObject* Function;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PlayerController.ClientIgnoreMoveInput");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct
		{
			bool bIgnore;
		} Params = { bIgnore };

		this->ProcessEvent(Function, &Params);
	}

	void SetIgnoreLookInput(bool bNewLookInput)
	{
		static UObject* Function;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PlayerController.SetIgnoreLookInput");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct
		{
			bool bNewLookInput;
		} Params = { bNewLookInput };

		this->ProcessEvent(Function, &Params);
	}

	bool WasInputKeyJustPressed(FKey key)
	{
		static UObject* Function;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PlayerController.WasInputKeyJustPressed");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct
		{
			FKey key;
			bool ReturnValue;
		} Params = { key };

		this->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	void ClientReturnToMainMenu(FString& ReturnReason)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PlayerController.ClientReturnToMainMenu");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FString ReturnReason;
		} Params = { ReturnReason };

		this->ProcessEvent(Function, &Params);
	}

	void SetMouseLocation(int32_t X, int32_t Y)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"PlayerController.SetMouseLocation");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			int32_t X;
			int32_t Y;
		} Params = { X, Y };

		this->ProcessEvent(Function, &Params);
	}
};

class UGameViewportClient : public UObject
{
public:

};

class UPlayer : public UObject
{
public:
	APlayerController* PlayerController()
	{
		return *(APlayerController**)(this + 0x30);
	}
};

class UFortClientSettingsRecord : public UObject
{
public:
	float HUDScale()
	{
		return *(float*)(this + 0x4d8);
	}
};

class UFortLocalPlayer : public UPlayer 
{
public:
	UFortClientSettingsRecord* ClientSettingsRecord()
	{
		return *(UFortClientSettingsRecord**)(this + 0x378);
	}
};

class ULocalPlayer : public UFortLocalPlayer
{
public:
	UGameViewportClient* ViewportClient()
	{
		return *(UGameViewportClient**)(this + 0x78);
	}
};

class UGameInstance : public UObject
{
public:
	TArray <ULocalPlayer*> LocalPlayers()
	{
		return *(TArray <ULocalPlayer*>*)(this + 0x38);
	}
};

class UWorld : public UObject
{
public:
	UGameInstance* OwningGameInstance()
	{
		return *(UGameInstance**)(this + 0x1d8);
	}

	static UWorld* GetWorld()
	{
		return *(UWorld**)(FortniteClientWin64Shipping + Offsets::GWorld);
	}
};

class UGameUserSettings : public UObject 
{
public:

};

class UFortGameUserSettings : public UGameUserSettings
{
public:

};

class UFortKismetLibrary : public UObject
{
public:
	static bool CheckLineOfSightToActorWithChannel(FVector SourcePos, AActor* Target, ECollisionChannel TargetFilterChannel, AActor* Source)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortKismetLibrary.CheckLineOfSightToActorWithChannel");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FVector SourcePos;
			AActor* Target;
			ECollisionChannel TargetFilterChannel;
			AActor* Source;
			bool ReturnValue;
		} Params = { SourcePos, Target, TargetFilterChannel, Source };

		FortKismetLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static float GetMatchTime(UObject* WorldContextObject)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortKismetLibrary.GetMatchTime");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			UObject* WorldContextObject;
			float ReturnValue;
		} Params = { WorldContextObject };

		FortKismetLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static bool GetSafeZoneLocation(UObject* WorldContextObject, int32_t SafeZoneIndex, FVector& OutLocation)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortKismetLibrary.GetSafeZoneLocation");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			UObject* WorldContextObject;
			int32_t SafeZoneIndex;
			FVector& OutLocation;
			bool ReturnValue;
		} Params = { WorldContextObject, SafeZoneIndex, OutLocation };

		FortKismetLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static int32_t GetCurrentSafeZonePhase(UObject* WorldContextObject)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"FortKismetLibrary.GetCurrentSafeZonePhase");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			UObject* WorldContextObject;
			int32_t ReturnValue;
		} Params = { WorldContextObject };

		FortKismetLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	//char GetActorTeam(struct AActor* Actor);
	//struct TArray<struct AFortPlayerController*> GetAllFortPlayerControllers(struct UObject* WorldContextObject, bool bIncludeNonSpectators, bool bIncludeSpectators);
	//struct AActor* GetClosestActorFromArray(struct AActor* Actor, struct TArray<struct AActor*>& ArrayOfActors);
	//void GetFortPlayerPawnsInRange(struct UObject* WorldContextObject, struct FVector& SourcePosition, float Range, bool bDo2DCheck, struct TArray<struct AFortPlayerPawn*>& OutFortPlayerPawns); // Function FortniteGame.FortKisme

};

class UKismetMathLibrary : public UObject
{
public:
	static float NormalizeAxis(float Angle)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.NormalizeAxis");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			float Angle;
			float ReturnValue;
		} Params = { Angle };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static int32_t FMod(double Dividend, double Divisor, double& Remainder)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.FMod");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			double Dividend; double Divisor; double& Remainder;
			int32_t ReturnValue;
		} Params = { Dividend, Divisor, Remainder };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double FMax(double A, double B)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.FMax");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			double A, B;
			double ReturnValue;
		} Params = { A, B };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FRotator RLerp(FRotator A, FRotator B, float Alpha, bool bShortestPath)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.Lerp");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FRotator A; FRotator B; float Alpha; bool bShortestPath;
			FRotator ReturnValue;
		} Params = { A, B, Alpha, bShortestPath };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double Lerp(double A, double B, double Alpha)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.Lerp");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			double A, B, Alpha;
			double ReturnValue;
		} Params = { A, B, Alpha };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double FClamp(double Value, double Min, double Max)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.FClamp");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			double Value, Min, Max;
			double ReturnValue;
		} Params = { Value, Min, Max };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double sin(double A)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.sin");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			double A;
			double ReturnValue;
		} Params = { A };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double cos(double A)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.cos");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			double A;
			double ReturnValue;
		} Params = { A };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double acos(double A)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.acos");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}
	
		struct {
			double A;
			double ReturnValue;
		} Params = { A };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double tan(double A)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.tan");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			double A;
			double ReturnValue;
		} Params = { A };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double Atan2(double Y, double X)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.Atan2");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			double Y;
			double X;
			double ReturnValue;
		} Params = { Y, X };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double abs(double A)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.abs");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			double A;
			double ReturnValue;
		} Params = { A };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double sqrt(double A)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.sqrt");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			double A;
			double ReturnValue;
		} Params = { A };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static int32 RandomIntegerInRange(int32 Min, int32 Max)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.RandomIntegerInRange");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			int32 Min;
			int32 Max;
			int32 ReturnValue;
		} Params = { Min, Max };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double RandomFloatInRange(double Min, double Max)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.RandomFloatInRange");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			double Min;
			double Max;
			double ReturnValue;
		} Params = { Min, Max };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static float DegreesToRadians(float A)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.DegreesToRadians");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			float A;
			float ReturnValue;
		} Params = { A };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FVector GetRightVector(FRotator InRot)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.GetRightVector");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FRotator InRot;
			FVector ReturnValue;
		} Params = { InRot };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FVector GetForwardVector(FRotator InRot)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.GetForwardVector");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FRotator InRot;
			FVector ReturnValue;
		} Params = { InRot };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FRotator FindLookAtRotation(FVector& Start, FVector& Target)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.FindLookAtRotation");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FVector Start;
			FVector Target;
			FRotator ReturnValue;
		} Params = { Start, Target };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double Vector_Distance(FVector v1, FVector v2)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.Vector_Distance");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FVector v1;
			FVector v2;
			double ReturnValue;
		} Params = { v1, v2 };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double Vector_Distance2D(FVector2D v1, FVector2D v2)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.Vector_Distance2D");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FVector v1;
			FVector v2;
			double ReturnValue;
		} Params = { FVector(v1.X, v1.Y, 0.0), FVector(v2.X, v2.Y, 0.0) };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FVector GetReflectionVector(FVector Direction, FVector SurfaceNormal)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.GetReflectionVector");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FVector Direction;
			FVector SurfaceNormal;
			FVector ReturnValue;
		} Params = { Direction, SurfaceNormal };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FRotator RInterpTo_Constant(FRotator& Current, FRotator& Target, float DeltaTime, float InterpSpeed)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.RInterpTo_Constant");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FRotator Current;
			FRotator Target;
			float DeltaTime;
			float InterpSpeed;
			FRotator ReturnValue;
		} Params = { Current, Target, DeltaTime, InterpSpeed };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double FInterpTo_Constant(double Current, double Target, double DeltaTime, double InterpSpeed)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.FInterpTo_Constant");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			double Current;
			double Target;
			float DeltaTime;
			float InterpSpeed;
			double ReturnValue;
		} Params = { Current, Target, DeltaTime, InterpSpeed };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FRotator RInterpTo(FRotator& Current, FRotator& Target, float DeltaTime, float InterpSpeed)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.RInterpTo");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FRotator Current;
			FRotator Target;
			float DeltaTime;
			float InterpSpeed;
			FRotator ReturnValue;
		} Params = { Current, Target, DeltaTime, InterpSpeed };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double FInterpEaseInOut(double A, double B, double Alpha, double Exponent)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.FInterpEaseInOut");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			double A, B, Alpha, Exponent;
			double ReturnValue;
		} Params = { A, B, Alpha, Exponent };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double FInterpTo(double Current, double Target, double DeltaTime, double Speed)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetMathLibrary.RInterpTo");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct
		{
			double Current;
			double Target;
			double DeltaTime;
			double Speed;
			double ReturnValue;
		} Params = { Current, Target, DeltaTime, Speed };

		KismetMathLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UGameplayStatics : public UObject
{
public:
	static TArray<UObject*> GetAllActorsOfClass(UObject* WorldContextObject, UObject* ActorClass)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"GameplayStatics.GetAllActorsOfClass");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			UObject* WorldContextObject;
			UObject* ActorClass;
			TArray<UObject*> OutActors;
		} Params = { WorldContextObject, ActorClass };

		GameplayStatics->ProcessEvent(Function, &Params);

		return Params.OutActors;
	}

	static bool ObjectIsA(UObject* Object, UObject* ObjectClass)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"GameplayStatics.ObjectIsA");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			UObject* Object;
			UObject* ObjectClass;
			bool ReturnValue;
		} Params = { Object, ObjectClass };

		GameplayStatics->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double GetWorldDeltaSeconds(UObject* WorldContextObject)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"GameplayStatics.GetWorldDeltaSeconds");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			UObject* WorldContextObject;
			double ReturnValue;
		} Params = { WorldContextObject };

		GameplayStatics->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static double GetTimeSeconds(UObject* WorldContextObject)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"GameplayStatics.GetTimeSeconds");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			UObject* WorldContextObject;
			double ReturnValue;
		} Params = { WorldContextObject };

		GameplayStatics->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UKismetSystemLibrary : public UObject
{
public:
	static double GetGameTimeInSeconds(UObject* WorldContextObject)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetSystemLibrary.GetGameTimeInSeconds");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			UObject* WorldContextObject;
			double ReturnValue;
		} Params = { WorldContextObject };

		KismetSystemLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static bool LineTraceSingle(UObject* WorldContextObject, FVector Start, FVector End, ETraceTypeQuery TraceChannel, bool bTraceComplex, TArray<AActor*>& ActorsToIgnore, EDrawDebugTrace DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetSystemLibrary.LineTraceSingle");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			UObject* WorldContextObject;
			FVector Start;
			FVector End;
			ETraceTypeQuery TraceChannel;
			bool bTraceComplex;
			TArray<AActor*> ActorsToIgnore;
			EDrawDebugTrace DrawDebugType;
			FHitResult OutHit;
			bool bIgnoreSelf;
			FLinearColor TraceColor;
			FLinearColor TraceHitColor;
			float DrawTime;
			bool ReturnValue;
		} Params = { WorldContextObject, Start, End, TraceChannel, bTraceComplex, ActorsToIgnore, DrawDebugType, FHitResult(), bIgnoreSelf, FLinearColor(), FLinearColor(), 0.0f };

		KismetSystemLibrary->ProcessEvent(Function, &Params);

		OutHit = Params.OutHit;

		return Params.ReturnValue;
	}

	static FString GetObjectName(UObject* Object)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetSystemLibrary.GetObjectName");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			UObject* Object;
			FString ReturnValue;
		} Params = { Object };

		KismetSystemLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FString GetPathName(UObject* Object)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetSystemLibrary.GetPathName");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			UObject* Object;
			FString ReturnValue;
		} Params = { Object };

		KismetSystemLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static bool IsValid(UObject* Object)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetSystemLibrary.IsValid");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			UObject* Object;
			bool ReturnValue;
		} Params = { Object };

		KismetSystemLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FString GetDeviceId()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetSystemLibrary.GetDeviceId");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FString ReturnValue;
		} Params = { };

		KismetSystemLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static int64_t GetFrameCount()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetSystemLibrary.GetFrameCount");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			int64_t ReturnValue;
		} Params = { };

		KismetSystemLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FString GetPlatformUserName()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetSystemLibrary.GetPlatformUserName");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FString ReturnValue;
		} Params = { };

		KismetSystemLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FString GetEngineVersion()
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetSystemLibrary.GetEngineVersion");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FString ReturnValue;
		} Params = { };

		KismetSystemLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

class UKismetStringLibrary : public UObject
{
public:
	static bool Contains(FString SearchIn, FString Substring, bool bUseCase, bool bSearchFromEnd)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetStringLibrary.Contains");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FString SearchIn;
			FString Substring;
			bool bUseCase;
			bool bSearchFromEnd;
			bool ReturnValue;
		} Params = { SearchIn, Substring, bUseCase, bSearchFromEnd };

		KismetStringLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FName Conv_StringToName(FString InString)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetStringLibrary.Conv_StringToName");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FString InString;
			FName ReturnValue;
		} Params = { InString };

		KismetStringLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FString Conv_NameToString(FName InName)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetStringLibrary.Conv_NameToString");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FName InName;
			FString ReturnValue;
		} Params = { InName };

		KismetStringLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FString BuildString_Int(FString AppendTo, FString Prefix, int InInt, FString Suffix)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetStringLibrary.BuildString_Int");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FString AppendTo;
			FString Prefix;
			int InInt;
			FString Suffix;
			FString ReturnValue;
		} Params = { AppendTo, Prefix, InInt, Suffix };

		KismetStringLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static FString BuildString_Double(FString AppendTo, FString Prefix, double InDouble, FString Suffix)
	{
		static UObject* Function;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetStringLibrary.BuildString_Double");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct
		{
			FString AppendTo;
			FString Prefix;
			double InDouble;
			FString Suffix;
			FString ReturnValue;
		} Params = { AppendTo, Prefix, InDouble, Suffix };

		KismetStringLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}

	static bool IsEmpty(FString InString)
	{
		static UObject* Function = 0;
		if (!Function)
		{
			auto Enc_Function = Encrypt(L"KismetStringLibrary.IsEmpty");
			Function = FindObjectSingle<UObject*>(Enc_Function.decrypt()); Enc_Function.clear();
		}

		struct {
			FString InName;
			bool ReturnValue;
		} Params = { InString };

		KismetStringLibrary->ProcessEvent(Function, &Params);

		return Params.ReturnValue;
	}
};

void* TUObjectArray::FindObject(FName name, void* Class) const
{
	for (auto i = 0u; i < NumElements; i++)
	{
		auto object = reinterpret_cast<UObject1*>(GetObjectPtr(i));
		if (object && object->GetName() == name && UGameplayStatics::ObjectIsA((UObject*)object, (UObject*)Class)) { return object; }
	}

	return nullptr;
}

namespace Custom
{
	FLinearColor GetColorByTier(EFortItemTier Tier)
	{
		if (Tier == EFortItemTier::I || Tier == EFortItemTier::No_Tier)
			return FLinearColor(0.4f, 0.4f, 0.4f, 1.f);
		else if (Tier == EFortItemTier::II)
			return FLinearColor(0.2f, 1.f, 0.2f, 1.f);
		else if (Tier == EFortItemTier::III)
			return FLinearColor(0.2f, 0.2f, 0.9f, 1.f);
		else if (Tier == EFortItemTier::IV)
			return FLinearColor(1.f, 0.2f, 1.f, 1.f);
		else if (Tier == EFortItemTier::V)
			return FLinearColor(1.f, 1.f, 0.2f, 1.f);
		else if (Tier == EFortItemTier::VI)
			return FLinearColor(1.f, 0.84f, 0.f, 1.f);

		return FLinearColor(0.f, 0.f, 0.f, 0.f);
	}

	FString GetNameByPlatform(FString Platform)
	{
		if (Platform == FString(L"XBL"))
			return FString(L"XBox");
		else if (Platform == FString(L"PSN") || Platform == FString(L"PS5"))
			return FString(L"PlayStation");
		else if (Platform == FString(L"WIN"))
			return FString(L"Windows");
		else if (Platform == FString(L"SWT"))
			return FString(L"Nintendo Switch");
		else if (Platform == FString(L"AND"))
			return FString(L"Android");
		else if (Platform == FString(L"MAC"))
			return FString(L"MacOS");
		else if (Platform == FString(L"LNX"))
			return FString(L"Linux");
		else if (Platform == FString(L"IOS"))
			return FString(L"Apple");
		else
			return FString(L"");
	}

	FLinearColor GetColorByPlatform(FString Platform)
	{
		if (Platform == FString(L"XBL"))
			return FLinearColor(0.0627f, 0.4863f, 0.0627f, 1.0f);
		else if (Platform == FString(L"PSN") || Platform == FString(L"PS5"))
			return FLinearColor(0.f, 0.188f, 0.529f, 1.f);
		else if (Platform == FString(L"WIN"))
			return FLinearColor(0.f, 0.1255f, 0.3137f, 1.f);
		else if (Platform == FString(L"SWT"))
			return FLinearColor(0.902f, 0.0f, 0.0706f, 1.f);
		else if (Platform == FString(L"AND"))
			return FLinearColor(0.2392f, 0.8627f, 0.5176f, 1.f);
		else if (Platform == FString(L"MAC"))
			return FLinearColor(0.5569f, 0.5569f, 0.5765f, 1.f);
		else if (Platform == FString(L"LNX"))
			return FLinearColor(1.f, 0.843f, 0.f, 1.f);
		else if (Platform == FString(L"IOS"))
			return FLinearColor(0.f, 0.4784f, 1.f, 1.f);
		else
			return FLinearColor(0.f, 0.f, 0.f, 0.f);
	}

	FString GetRankByProgress(int32_t Tier)
	{
		if (Tier == 0)
			return FString(L"Bronze 1");
		else if (Tier == 1)
			return FString(L"Bronze 2");
		else if (Tier == 2)
			return FString(L"Bronze 3");
		else if (Tier == 3)
			return FString(L"Silver 1");
		else if (Tier == 4)
			return FString(L"Silver 2");
		else if (Tier == 5)
			return FString(L"Silver 3");
		else if (Tier == 6)
			return FString(L"Gold 1");
		else if (Tier == 7)
			return FString(L"Gold 2");
		else if (Tier == 8)
			return FString(L"Gold 3");
		else if (Tier == 9)
			return FString(L"Platinum 1");
		else if (Tier == 10)
			return FString(L"Platinum 2");
		else if (Tier == 11)
			return FString(L"Platinum 3");
		else if (Tier == 12)
			return FString(L"Diamond 1");
		else if (Tier == 13)
			return FString(L"Diamond 2");
		else if (Tier == 14)
			return FString(L"Diamond 3");
		else if (Tier == 15)
			return FString(L"Elite");
		else if (Tier == 16)
			return FString(L"Champion");
		else if (Tier == 17)
			return FString(L"Unreal");
		else
			return FString(L"Unranked");
	}

	FLinearColor GetColorByRank(int32_t Tier)
	{
		if (Tier == 0 || Tier == 1 || Tier == 2)
			return FLinearColor(0.902f, 0.580f, 0.227f, 1.0f); // Bronze
		else if (Tier == 3 || Tier == 4 || Tier == 5)
			return FLinearColor(0.843f, 0.843f, 0.843f, 1.0f); // Silver
		else if (Tier == 6 || Tier == 7 || Tier == 8)
			return FLinearColor(1.0f, 0.871f, 0.0f, 1.0f); // Gold
		else if (Tier == 9 || Tier == 10 || Tier == 11)
			return FLinearColor(0.0f, 0.7f, 0.7f, 1.0f); // Platinum
		else if (Tier == 12 || Tier == 13 || Tier == 14)
			return FLinearColor(0.0f, 0.0f, 0.7f, 1.0f); // Diamond
		else if (Tier == 15)
			return FLinearColor(0.7f, 0.7f, 0.7f, 1.0f); // Elite
		else if (Tier == 16)
			return FLinearColor(1.0f, 0.6f, 0.0f, 1.0f); // Champion
		else if (Tier == 17)
			return FLinearColor(0.6f, 0.0f, 0.6f, 1.0f); // Unreal
		else
			return FLinearColor(0.0f, 0.7859f, 1.0f, 1.0f); // Unranked
	}

	double GetProjectileSpeed(FString WeaponName)
	{
		if (WeaponName == FString(L"Reaper Sniper Rifle"))
			return 60000.0;
		else if (WeaponName == FString(L"Huntress DMR"))
			return 88000.0;
		else if (WeaponName == FString(L"Nemesis AR") || WeaponName == FString(L"Warforged Assault Rifle"))
			return 80000.0;
		else if (WeaponName == FString(L"Harbinger SMG") || WeaponName == FString(L"Thunder Bust SMG"))
			return 70000.0;
		else if (WeaponName == FString(L"Ranger Pistol") || WeaponName == FString(L"Hand Cannon"))
			return 60000.0;
		else if (WeaponName == FString(L"Drum Gun"))
			return 75000.0;
		return 80000.0;
	}

	double GetProjectileGravity(FString WeaponName)
	{
		if (WeaponName == FString(L"Reaper Sniper Rifle") || WeaponName == FString(L"Nemesis AR") || WeaponName == FString(L"Warforged Assault Rifle"))
			return 3.5;
		else if (WeaponName == FString(L"Harbinger SMG") || WeaponName == FString(L"Thunder Bust SMG"))
			return 3.0;
		else if (WeaponName == FString(L"Huntress DMR"))
			return 2.5;
		else if (WeaponName == FString(L"Ranger Pistol") || WeaponName == FString(L"Hand Cannon"))
			return 2.0;
		else if (WeaponName == FString(L"Drum Gun"))
			return 3.0;
		return 2.5;
	}

	void MemoryAim(AFortWeapon* CurrentWeapon, FRotator TargetRotation)
	{
		*(FRotator*)(CurrentWeapon + Offsets::AimRotation) = TargetRotation;
	}

	bool InArea(double target_x, double target_y, double pos_x, double pos_y, double size_x, double size_y)
	{
		if (target_x > pos_x && target_y > pos_y)
			if (target_x < pos_x + size_x && target_y < pos_y + size_y)
				return true;

		return false;
	}

	int GetPoint(int n1, int n2, float perc)
	{
		return n1 + ((n2 - n1) * perc);
	}

	float LerpAngle(float start, float end, float percent) {
		float difference = UKismetMathLibrary::NormalizeAxis(end - start);
		difference = (difference > 180.0f) ? difference - 360.0f : difference;
		return UKismetMathLibrary::NormalizeAxis(start + difference * percent);
	}

	bool InRect(double x, double y, double radius, double coord_x, double coord_y)
	{
		return coord_x >= x - radius && coord_x <= x + radius &&
			coord_y >= y - radius && coord_y <= y + radius;
	}

	bool InCircle(double x, double y, double radius, double coord_x, double coord_y)
	{
		if (InRect(x, coord_y, radius, x, coord_y))
		{
			double dx = x - coord_x;
			double dy = y - coord_y;
			dx *= dx;
			dy *= dy;
			double distance_squared = dx + dy;
			double radius_squared = radius * radius;
			return distance_squared <= radius_squared;
		} return false;
	}
}