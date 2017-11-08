// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "OperationProtoCharacter.h"
#include "Animation/AnimInstance.h"
#include "Components/DecalComponent.h"
#include "Components/AudioComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Engine/StaticMeshActor.h"
#include "ObjectLoader.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class OPERATIONPROTO_API UWeapon : public USkeletalMeshComponent
{
	GENERATED_BODY()
public:
	UWeapon();
	virtual void Update(float DeltaTime);
	virtual void GetPointingAt(FHitResult& hit);
	virtual void Fire(bool& canFireAfter);
	virtual bool ReadyToFire();
	virtual int GetAmmoCount();
	virtual int GetMax();
	virtual void AddAmmo(int amount);

	USkeletalMesh* Mesh;
	UMaterialInterface* Material;
	UAudioComponent* GunSoundPlayer;
	USoundCue* FireSound;
	USoundBase* EmptySound;
	UAnimMontage* FireAnimation;

	UMaterialInterface** decals;
	UParticleSystem** particleSystems;

	static ObjectLoader Loader;

	int numDecals = 2;
	int numParticleSystems = 4;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	int ammoCount = 8;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	int maxAmmo = 42;

	float FireTimer = 0.f;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	float FireCooldown = 0.3f;

	bool hasFired = false;
};
