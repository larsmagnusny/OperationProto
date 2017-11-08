#include "ObjectLoader.h"

using namespace std;

TArray<UObject*> ObjectLoader::ObjectBank;
TArray<FString> ObjectLoader::ObjectNames;

TArray<FString> ObjectLoader::LoadOrder;

void ObjectLoader::Initialize()
{
	// Materials used frequently
	FString MaterialInstanceRef[]
	{
		"MaterialInterface'/Game/FirstPerson/Textures/Decals/BulletHoledecal.BulletHoledecal'",
		"Material'/Game/FirstPerson/ParticleEffects/BloodSplatter.BloodSplatter'",
		"Material'/Game/FirstPerson/FPWeapon/Materials/M_FPGun.M_FPGun'"
	};

	for (int i = 0; i < 3; i++)
	{
		LoadOrder.Add(MaterialInstanceRef[i]);
	}

	FString ParticleSystemRef[]
	{
		"ParticleSystem'/Game/FirstPerson/ParticleEffects/Hit_System.Hit_System'",
		"ParticleSystem'/Game/FirstPerson/ParticleEffects/Hit_System_Wood.Hit_System_Wood'",
		"ParticleSystem'/Game/FirstPerson/ParticleEffects/Hit_System_Metal.Hit_System_Metal'",
		"ParticleSystem'/Game/FirstPerson/ParticleEffects/BloodSplat.BloodSplat'"
	};

	for (int i = 0; i < 4; i++)
	{
		LoadOrder.Add(ParticleSystemRef[i]);
	}

	FString SkeletalMeshRef[]
	{
		"SkeletalMesh'/Game/FirstPerson/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"
	};

	for (int i = 0; i < 1; i++)
	{
		LoadOrder.Add(SkeletalMeshRef[i]);
	}

	FString SoundCueRef[]
	{
		"SoundCue'/Game/FirstPerson/Audio/GunCue.GunCue'",
		"SoundCue'/Game/FirstPerson/Audio/MachineGun.MachineGun'"
	};

	for (int i = 0; i < 2; i++)
	{
		LoadOrder.Add(SoundCueRef[i]);
	}

	FString SoundBaseRef[]
	{
		"SoundWave'/Game/FirstPerson/Audio/GunClick.GunClick'"
	};

	for (int i = 0; i < 1; i++)
	{
		LoadOrder.Add(SoundBaseRef[i]);
	}

	FString AnimMontageRef[]
	{
		"AnimMontage'/Game/FirstPerson/Animations/FirstPersonFire_Montage.FirstPersonFire_Montage'"
	};

	for (int i = 0; i < 1; i++)
	{
		LoadOrder.Add(AnimMontageRef[i]);
	}

	// Load everything into memory
	for (int i = 0; i < LoadOrder.Num(); i++)
	{
		pair<UObject*, FString> p;
		ConstructorHelpers::FObjectFinder<UObject> OLoader(*LoadOrder[i]);

		if (OLoader.Succeeded())
		{
			ObjectBank.Add(OLoader.Object);
			
			int from, to;
			LoadOrder[i].FindLastChar('/', from);
			LoadOrder[i].FindLastChar('.', to);
			ObjectNames.Add(LoadOrder[i].Mid(from + 1, to - from - 1));
			UE_LOG(LogTemp, Warning, TEXT("Assetname: %s"), *LoadOrder[i].Mid(from + 1, to - from - 1));
		}
	}
}

UObject * ObjectLoader::GetObject(FString Str)
{
	int Index;
	bool found = ObjectNames.Find(Str, Index);

	return found ? ObjectBank[Index] : nullptr;
}

bool ObjectLoader::IsInitialized()
{
	return LoadOrder.Num() != 0;
}
