#pragma once
#include "OperationProto.h"
#include "Materials/MaterialInstance.h"
#include "Runtime/Engine/Public/ParticleDefinitions.h"
#include "Particles/ParticleSystem.h"
#include "Engine/SkeletalMesh.h"
#include "Class.h"
#include "Animation/AnimMontage.h"
#include "Sound/SoundCue.h"
#include "ConstructorHelpers.h"
#include <set>

static class ObjectLoader
{
public:
	static void Initialize();
	static UObject* GetObject(FString Str);

	static bool IsInitialized();
private:
	static TArray<UObject*> ObjectBank;
	static TArray<FString> ObjectNames;

	static TArray<FString> LoadOrder;
};

