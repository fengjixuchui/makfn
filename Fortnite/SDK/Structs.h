#pragma once

#define MergeInternal(A, B) A##B
#define Merge(A, B) MergeInternal(A, B)
#define Pad(Size) unsigned char Merge(_pad_, __COUNTER__)[Size]

typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

#define M_PI 3.14159265358979323846
#define DBL_MAX 1.7976931348623158e+308
#define DBL_MIN 2.2250738585072014e-308
#define FLT_MAX 3.402823466e+38F
#define FLT_MIN 1.175494351e-38F
#define INT_MAX 2147483647
#define INT_MIN (-2147483647 - 1)
#define INT32_MIN (-2147483647i32 - 1)

template <class ArrayType>
struct TArray
{
	TArray()
	{
		this->Data = nullptr;
		this->Count = this->Max = 0;
	};

	ArrayType* Data;
	int Count, Max;

	ArrayType& operator[](int i)
	{
		return this->Data[i];
	};

	void PushBack(ArrayType Member)
	{
		this->Count += 1;
		this->Max += 1;

		//Allocate new array
		ArrayType* NewAllocation = reinterpret_cast<ArrayType * (__cdecl*)(uintptr_t, int, uintptr_t, void*)>(DoSpoofCall)(this->Count * sizeof(ArrayType), 0x10, SpoofCode, (void*)(Malloc));

		//Fill with old data
		for (int i = 0; i < this->Count - 1; i++)
		{
			NewAllocation[i] = this->Data[i];
		}

		NewAllocation[this->Count - 1] = Member;

		//Free old array
		if (this->Data)
		{
			reinterpret_cast<void* (__cdecl*)(void*, uintptr_t, void*)>(DoSpoofCall)(this->Data, SpoofCode, (void*)(Free));
		}

		//Set new data pointer
		this->Data = NewAllocation;
	}

	void Clear()
	{
		//Free old array
		reinterpret_cast<void* (__cdecl*)(void*, uintptr_t, void*)>(DoSpoofCall)(this->Data, SpoofCode, (void*)(Free));

		this->Data = nullptr;

		this->Count = this->Max = 0;
	}

	int Size()
	{
		return this->Count;
	}

	bool Valid(int i)
	{
		return bool(i < this->Count);
	}
};

struct FString : private TArray<wchar_t>
{
	FString() { };

	FString(const wchar_t* Other)
	{
		this->Max = this->Count = *Other ? static_cast<int>(__wcslen(Other)) + 1 : 0;

		if (this->Count)
		{
			this->Data = const_cast<wchar_t*>(Other);
		}
	};

	FString(const wchar_t* Other, int Size)
	{
		this->Max = this->Count = Size;

		if (this->Count)
		{
			this->Data = const_cast<wchar_t*>(Other);
		}
	};

	operator bool() { return bool(this->Data); }

	FString& operator+=(FString Other)
	{
		for (int i = 0; i < Other.Size(); i++)
		{
			this->PushBack(Other[i]);
		}

		return *this;
	}

	wchar_t* c_str()
	{
		return this->Data;
	}

	int Size()
	{
		return this->Count;
	}

	void clear()
	{
		this->Data = nullptr;
		this->Count = this->Max = 0;
	}
};

struct FVector
{
	FVector() : X(), Y(), Z() { }
	FVector(double X, double Y, double Z) : X(X), Y(Y), Z(Z) { }

	FVector operator + (const FVector& other) const { return { this->X + other.X, this->Y + other.Y, this->Z + other.Z }; }
	FVector operator - (const FVector& other) const { return { this->X - other.X, this->Y - other.Y, this->Z - other.Z }; }
	FVector operator * (double offset) const { return { this->X * offset, this->Y * offset, this->Z * offset }; }
	FVector operator / (double offset) const { return { this->X / offset, this->Y / offset, this->Z / offset }; }

	FVector& operator *= (const double other) { this->X *= other; this->Y *= other; this->Z *= other; return *this; }
	FVector& operator /= (const double other) { this->X /= other; this->Y /= other; this->Z /= other; return *this; }

	FVector& operator = (const FVector& other) { this->X = other.X; this->Y = other.Y; this->Z = other.Z; return *this; }
	FVector& operator += (const FVector& other) { this->X += other.X; this->Y += other.Y; this->Z += other.Z; return *this; }
	FVector& operator -= (const FVector& other) { this->X -= other.X; this->Y -= other.Y; this->Z -= other.Z; return *this; }
	FVector& operator *= (const FVector& other) { this->X *= other.X; this->Y *= other.Y; this->Z *= other.Z; return *this; }
	FVector& operator /= (const FVector& other) { this->X /= other.X; this->Y /= other.Y; this->Z /= other.Z; return *this; }

	operator bool() { return bool(this->X || this->Y || this->Z); }
	friend bool operator == (const FVector& a, const FVector& b) { return a.X == b.X && a.Y == b.Y && a.Z == b.Z; }
	friend bool operator != (const FVector& a, const FVector& b) { return !(a == b); }

	double Dot(const FVector& V) { return X * V.X + Y * V.Y + Z * V.Z; }
	double SizeSquared() { return X * X + Y * Y + Z * Z; }

	double X, Y, Z;
};

struct FVector2D
{
	FVector2D() : X(), Y() { }
	FVector2D(double X, double Y) : X(X), Y(Y) { }

	FVector2D operator + (const FVector2D& other) const { return { this->X + other.X, this->Y + other.Y }; }
	FVector2D operator - (const FVector2D& other) const { return { this->X - other.X, this->Y - other.Y }; }
	FVector2D operator * (double offset) const { return { this->X * offset, this->Y * offset }; }
	FVector2D operator / (double offset) const { return { this->X / offset, this->Y / offset }; }

	FVector2D& operator *= (const double other) { this->X *= other; this->Y *= other; return *this; }
	FVector2D& operator /= (const double other) { this->X /= other; this->Y /= other; return *this; }

	FVector2D& operator = (const FVector2D& other) { this->X = other.X; this->Y = other.Y; return *this; }
	FVector2D& operator += (const FVector2D& other) { this->X += other.X; this->Y += other.Y; return *this; }
	FVector2D& operator -= (const FVector2D& other) { this->X -= other.X; this->Y -= other.Y; return *this; }
	FVector2D& operator *= (const FVector2D& other) { this->X *= other.X; this->Y *= other.Y; return *this; }
	FVector2D& operator /= (const FVector2D& other) { this->X /= other.X; this->Y /= other.Y; return *this; }

	operator bool() { return bool(this->X || this->Y); }
	friend bool operator == (const FVector2D& A, const FVector2D& B) { return A.X == B.X && A.Y == A.Y; }
	friend bool operator != (const FVector2D& A, const FVector2D& B) { return !(A == B); }

	double X, Y;
};

struct FRotator
{
	FRotator() : Pitch(), Yaw(), Roll() { }
	FRotator(double Pitch, double Yaw, double Roll) : Pitch(Pitch), Yaw(Yaw), Roll(Roll) { }

	FRotator operator + (const FRotator& other) const { return { this->Pitch + other.Pitch, this->Yaw + other.Yaw, this->Roll + other.Roll }; }
	FRotator operator - (const FRotator& other) const { return { this->Pitch - other.Pitch, this->Yaw - other.Yaw, this->Roll - other.Roll }; }
	FRotator operator * (double offset) const { return { this->Pitch * offset, this->Yaw * offset, this->Roll * offset }; }
	FRotator operator / (double offset) const { return { this->Pitch / offset, this->Yaw / offset, this->Roll / offset }; }

	FRotator& operator *= (const double other) { this->Pitch *= other; this->Yaw *= other; this->Roll *= other; return *this; }
	FRotator& operator /= (const double other) { this->Pitch /= other; this->Yaw /= other; this->Roll /= other; return *this; }

	FRotator& operator = (const FRotator& other) { this->Pitch = other.Pitch; this->Yaw = other.Yaw; this->Roll = other.Roll; return *this; }
	FRotator& operator += (const FRotator& other) { this->Pitch += other.Pitch; this->Yaw += other.Yaw; this->Roll += other.Roll; return *this; }
	FRotator& operator -= (const FRotator& other) { this->Pitch -= other.Pitch; this->Yaw -= other.Yaw; this->Roll -= other.Roll; return *this; }
	FRotator& operator *= (const FRotator& other) { this->Pitch *= other.Pitch; this->Yaw *= other.Yaw; this->Roll *= other.Roll; return *this; }
	FRotator& operator /= (const FRotator& other) { this->Pitch /= other.Pitch; this->Yaw /= other.Yaw; this->Roll /= other.Roll; return *this; }

	operator bool() { return bool(this->Pitch || this->Yaw || this->Roll); }
	friend bool operator == (const FRotator& a, const FRotator& b) { return a.Pitch == b.Pitch && a.Yaw == b.Yaw && a.Roll == b.Roll; }
	friend bool operator != (const FRotator& a, const FRotator& b) { return !(a == b); }

	FVector Euler() const
	{
		return FVector(Pitch, Yaw, Roll);
	}

	void NormalizeAngles()
	{
		Pitch = NormalizeAngle(Pitch);
		Yaw = NormalizeAngle(Yaw);
		Roll = NormalizeAngle(Roll);
	}

	double NormalizeAngle(double Angle)
	{
		while (Angle > 180)
			Angle -= 360;
		while (Angle < -180)
			Angle += 360;
		return Angle;
	}

	double Pitch, Yaw, Roll;
};

struct FLinearColor
{
	FLinearColor() : r(0.f), g(0.f), b(0.f), a(0.f) {}
	FLinearColor(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

	operator bool() { return bool(this->r || this->g || this->b || this->a); }
	friend bool operator	== (const FLinearColor& a, const FLinearColor& b) { return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a; }

	FLinearColor ToRGB() { return { r / 255.0f, g / 255.0f, b / 255.0f, a }; }

	float r, g, b, a;
};

struct FPlane : public FVector
{
	FPlane() : W() { }
	FPlane(double W) : W(W) { }

	double W;
};

struct FMatrix
{
	FMatrix() : XPlane(), YPlane(), ZPlane(), WPlane() { }
	FMatrix(FPlane XPlane, FPlane YPlane, FPlane ZPlane, FPlane WPlane) : XPlane(XPlane), YPlane(YPlane), ZPlane(ZPlane), WPlane(WPlane) { }

	FPlane XPlane, YPlane, ZPlane, WPlane;
};

struct FName
{
	FName() : Index() { }
	FName(int Index) : Index(Index) { }

	operator bool() { return bool(this->Index); }
	friend bool operator	== (const FName& a, const FName& b) { return a.Index == b.Index; }
	friend bool operator	!= (const FName& a, const FName& b) { return !(a == b); }

	uint32_t Index;
	uint32_t Number;
};

class TUObjectArray {
public:
	BYTE** Objects;
	BYTE* PreAllocatedObjects;
	uint32_t MaxElements;
	uint32_t NumElements;
	uint32_t MaxChunks;
	uint32_t NumChunks;
	void* GetObjectPtr(uint32_t id) const;
	void* FindObject(FName name, void* Class) const;
};

class FTextData
{
public:
	char Details[0x28]; // 0x28
	FString String;
};

struct FText 
{
	FTextData* Data;

	FString Get() 
	{
		if (this->Data)
		{
			return Data->String;
		}

		return nullptr;
	}
};

struct FKey
{
	FName KeyName;
	char Details[0x10]; // Pad_28B4
};

struct FHitResult
{
public:
	int8 bBlockingHit : 1;                                  // Mask: 0x1, PropSize: 0x10x0(0x1)(NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8 bStartPenetrating : 1;                             // Mask: 0x2, PropSize: 0x10x0(0x1)(NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8 BitPad_113 : 6;                                    // Fixing Bit-Field Size  [ Dumper-7 ]
	uint8 Pad_A82[0x3];                                      // Fixing Size After Last Property  [ Dumper-7 ]
	int32 FaceIndex;                                         // 0x4(0x4)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float Time;                                              // 0x8(0x4)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float Distance;                                          // 0xC(0x4)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	FVector Location;                                          // 0x10(0xC)(NoDestructor, NativeAccessSpecifierPublic)
	FVector ImpactPoint;                                       // 0x1C(0xC)(NoDestructor, NativeAccessSpecifierPublic)
	FVector Normal;                                            // 0x28(0xC)(NoDestructor, NativeAccessSpecifierPublic)
	FVector ImpactNormal;                                      // 0x34(0xC)(NoDestructor, NativeAccessSpecifierPublic)
	FVector TraceStart;                                        // 0x40(0xC)(NoDestructor, NativeAccessSpecifierPublic)
	FVector TraceEnd;                                          // 0x4C(0xC)(NoDestructor, NativeAccessSpecifierPublic)
	float PenetrationDepth;                                  // 0x58(0x4)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32 Item;                                              // 0x5C(0x4)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	FName BoneName;                                          // 0x78(0x8)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	FName MyBoneName;                                        // 0x80(0x8)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

struct FMinimalViewInfo
{
public:
	float AspectRatio;
	float DesiredFOV;
	float FOV;
	FVector Location;
	FVector2D OffCenterProjectionOffset;
	float OrthoFarClipPlane;
	float OrthoNearClipPlane;
	float OrthoWidth;
	float PerspectiveNearClipPlane;
	float PostProcessBlendWeight;
	FRotator Rotation;
};

enum EFortItemTier : uint8
{
	No_Tier = 0,
	I = 1,
	II = 2,
	III = 3,
	IV = 4,
	V = 5,
	VI = 6,
	VII = 7,
	VIII = 8,
	IX = 9,
	X = 10,
	NumItemTierValues = 11,
	EFortItemTier_MAX = 12
};

enum class ECollisionChannel : uint8_t
{
	ECC_WorldStatic = 0,
	ECC_WorldDynamic = 1,
	ECC_Pawn = 2,
	ECC_Visibility = 3,
	ECC_Camera = 4,
	ECC_PhysicsBody = 5,
	ECC_Vehicle = 6,
	ECC_Destructible = 7,
	ECC_EngineTraceChannel1 = 8,
	ECC_EngineTraceChannel2 = 9,
	ECC_EngineTraceChannel3 = 10,
	ECC_EngineTraceChannel4 = 11,
	ECC_EngineTraceChannel5 = 12,
	ECC_EngineTraceChannel6 = 13,
	ECC_GameTraceChannel1 = 14,
	ECC_GameTraceChannel2 = 15,
	ECC_GameTraceChannel3 = 16,
	ECC_GameTraceChannel4 = 17,
	ECC_GameTraceChannel5 = 18,
	ECC_GameTraceChannel6 = 19,
	ECC_GameTraceChannel7 = 20,
	ECC_GameTraceChannel8 = 21,
	ECC_GameTraceChannel9 = 22,
	ECC_GameTraceChannel10 = 23,
	ECC_GameTraceChannel11 = 24,
	ECC_GameTraceChannel12 = 25,
	ECC_GameTraceChannel13 = 26,
	ECC_GameTraceChannel14 = 27,
	ECC_GameTraceChannel15 = 28,
	ECC_GameTraceChannel16 = 29,
	ECC_GameTraceChannel17 = 30,
	ECC_GameTraceChannel18 = 31,
	ECC_OverlapAll_Deprecated = 32,
	ECC_MAX = 33
};

enum EFortWeaponCoreAnimation : uint8_t
{
	Melee = 0,
	Pistol = 1,
	Shotgun = 2,
	PaperBlueprint = 3,
	Rifle = 4,
	MeleeOneHand = 5,
	MachinePistol = 6,
	RocketLauncher = 7,
	GrenadeLauncher = 8,
	GoingCommando = 9,
	AssaultRifle = 10,
	TacticalShotgun = 11,
	SniperRifle = 12,
	TrapPlacement = 13,
	ShoulderLauncher = 14,
	AbilityDecoTool = 15,
	Crossbow = 16,
	C4 = 17,
	RemoteControl = 18,
	DualWield = 19,
	AR_BullPup = 20,
	AR_ForwardGrip = 21,
	MedPackPaddles = 22,
	SMG_P90 = 23,
	AR_DrumGun = 24,
	Consumable_Small = 25,
	Consumable_Large = 26,
	Balloon = 27,
	MountedTurret = 28,
	CreativeTool = 29,
	ExplosiveBow = 30,
	AshtonIndigo = 31,
	AshtonChicago = 32,
	MeleeDualWield = 33,
	Unarmed = 34,
	MAX = 35
};

enum ETraceTypeQuery : uint8
{
	TraceTypeQuery1 = 0,
	TraceTypeQuery2 = 1,
	TraceTypeQuery3 = 2,
	TraceTypeQuery4 = 3,
	TraceTypeQuery5 = 4,
	TraceTypeQuery6 = 5,
	TraceTypeQuery7 = 6,
	TraceTypeQuery8 = 7,
	TraceTypeQuery9 = 8,
	TraceTypeQuery10 = 9,
	TraceTypeQuery11 = 10,
	TraceTypeQuery12 = 11,
	TraceTypeQuery13 = 12,
	TraceTypeQuery14 = 13,
	TraceTypeQuery15 = 14,
	TraceTypeQuery16 = 15,
	TraceTypeQuery17 = 16,
	TraceTypeQuery18 = 17,
	TraceTypeQuery19 = 18,
	TraceTypeQuery20 = 19,
	TraceTypeQuery21 = 20,
	TraceTypeQuery22 = 21,
	TraceTypeQuery23 = 22,
	TraceTypeQuery24 = 23,
	TraceTypeQuery25 = 24,
	TraceTypeQuery26 = 25,
	TraceTypeQuery27 = 26,
	TraceTypeQuery28 = 27,
	TraceTypeQuery29 = 28,
	TraceTypeQuery30 = 29,
	TraceTypeQuery31 = 30,
	TraceTypeQuery32 = 31,
	TraceTypeQuery_MAX = 32,
	ETraceTypeQuery_MAX = 33,
};

enum EDrawDebugTrace : uint8
{
	None = 0,
	ForOneFrame = 1,
	ForDuration = 2,
	Persistent = 3,
	EDrawDebugTrace_MAX = 4,
};

enum EInputEvent : uint8
{
	IE_Pressed = 0,
	IE_Released = 1,
	IE_Repeat = 2,
	IE_DoubleClick = 3,
	IE_Axis = 4,
	IE_MAX = 5
};

struct FGuid 
{
	int32_t A; // 0x00(0x04)
	int32_t B; // 0x04(0x04)
	int32_t C; // 0x08(0x04)
	int32_t D; // 0x0c(0x04)
};

struct FRankedProgressReplicatedData 
{
	FString RankType; // 0x00(0x10)
	int32_t Rank; // 0x10(0x04)
	char pad_14[0x4]; // 0x14(0x04)
};

enum class EMaterialParameterAssociation : uint8 {
	LayerParameter = 0,
	BlendParameter = 1,
	GlobalParameter = 2,
	EMaterialParameterAssociation_MAX = 3
};

struct FMaterialParameterInfo 
{
	FMaterialParameterInfo() : Name(), Association(), Index() {}
	FMaterialParameterInfo(FName Name, EMaterialParameterAssociation Association, int32_t Index) : Name(Name), Association(Association), Index(Index) {}

	FName Name; // 0x00(0x04)
	EMaterialParameterAssociation Association; // 0x04(0x01)
	char pad_5[0x3]; // 0x05(0x03)
	int32_t Index; // 0x08(0x04)
};

struct FVectorParameterValue 
{
	FMaterialParameterInfo ParameterInfo; // 0x00(0x0c)
	FLinearColor ParameterValue; // 0x0c(0x10)
	FGuid ExpressionGUID; // 0x1c(0x10)
};

struct FScalarParameterValue 
{
	FMaterialParameterInfo ParameterInfo; // 0x00(0x0c)
	float ParameterValue; // 0x0c(0x04)
	FGuid ExpressionGUID; // 0x10(0x10)
};

struct UField {
	char pad_28[0x8];
};

struct UStruct : UField {
	char pad_30[0x80];
};

struct UScriptStruct : UStruct {
	char pad_B0[0x10];
};

struct UDataTable {
	UScriptStruct* RowStruct;
	char pad_30[0x50];
	char bStripFromClientBuilds : 1;
	char bIgnoreExtraFields : 1;
	char bIgnoreMissingFields : 1;
	char pad_80_3 : 5;
	char pad_81[0x7];
	FString ImportKeyField;
	char pad_98[0x18];
};

struct FDataTableRowHandle {
	UDataTable* DataTable;
	FName RowName;
	char pad_C[0x4];
};

struct FFortPawnStats
{
public:

};

enum EBlendMode : uint8 
{
	BLEND_Opaque = 0,
	BLEND_Masked = 1,
	BLEND_Translucent = 2,
	BLEND_Additive = 3,
	BLEND_Modulate = 4,
	BLEND_AlphaComposite = 5,
	BLEND_AlphaHoldout = 6,
	BLEND_TranslucentColoredTransmittance = 7,
	BLEND_MAX = 8,
	BLEND_TranslucentGreyTransmittance = 2,
	BLEND_ColoredTransmittanceOnly = 4
};

enum EDecalBlendMode : uint8
{
	DBM_Translucent = 0,
	DBM_Stain = 1,
	DBM_Normal = 2,
	DBM_Emissive = 3,
	DBM_DBuffer_ColorNormalRoughness = 4,
	DBM_DBuffer_Color = 5,
	DBM_DBuffer_ColorNormal = 6,
	DBM_DBuffer_ColorRoughness = 7,
	DBM_DBuffer_Normal = 8,
	DBM_DBuffer_NormalRoughness = 9,
	DBM_DBuffer_Roughness = 10,
	DBM_DBuffer_Emissive = 11,
	DBM_DBuffer_AlphaComposite = 12,
	DBM_DBuffer_EmissiveAlphaComposite = 13,
	DBM_Volumetric_DistanceFunction = 14,
	DBM_AlphaComposite = 15,
	DBM_AmbientOcclusion = 16,
	DBM_MAX = 17
};

enum EElementType
{
	ET_Box,
	ET_DebugQuad,
	ET_Text,
	ET_ShapedText,
	ET_Spline,
	ET_Line,
	ET_Gradient,
	ET_Viewport,
	ET_Border,
	ET_Custom,
	ET_CustomVerts,
	ET_PostProcessPass,
	ET_Count,
};

enum ESimpleElementBlendMode
{
	SE_BLEND_Opaque = 0,
	SE_BLEND_Masked,
	SE_BLEND_Translucent,
	SE_BLEND_Additive,
	SE_BLEND_Modulate,
	SE_BLEND_MaskedDistanceField,
	SE_BLEND_MaskedDistanceFieldShadowed,
	SE_BLEND_TranslucentDistanceField,
	SE_BLEND_TranslucentDistanceFieldShadowed,
	SE_BLEND_AlphaComposite,
	SE_BLEND_AlphaHoldout,
	SE_BLEND_AlphaBlend,
	SE_BLEND_TranslucentAlphaOnly,
	SE_BLEND_TranslucentAlphaOnlyWriteAlpha,
	SE_BLEND_RGBA_MASK_START,
	SE_BLEND_RGBA_MASK_END = SE_BLEND_RGBA_MASK_START + 31,
	SE_BLEND_MAX,
};

struct FDepthFieldGlowInfo
{
	FDepthFieldGlowInfo()
	{
		enable_glow = false;
		glow_color = FLinearColor{ 0, 0, 0, 0 };
		glow_outer_radius = FVector2D{ 0, 0 };
		glow_inner_radius = FVector2D{ 0, 0 };
	}

	uint32_t enable_glow : 1;
	FLinearColor glow_color;
	FVector2D glow_outer_radius;
	FVector2D glow_inner_radius;
};

struct FBatchedThickLines
{
	FVector start;
	FVector end;
	float thickness;
	FLinearColor color;
	int hitproxy_id;
	float depht_bias;
	uint32_t screen_space;
};