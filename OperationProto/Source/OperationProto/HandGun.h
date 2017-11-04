// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HandGun.generated.h"

class USoundCue;
/**
 * 
 */
UCLASS()
class OPERATIONPROTO_API UHandGun : public UWeapon
{
	GENERATED_BODY()
public:
	UHandGun();
	virtual void BeginPlay() override;
	virtual void GetPointingAt(FHitResult& hit) override;
	virtual void Update(float DeltaTime) override;
	virtual void Fire(bool& canFireAfter) override;
	virtual bool ReadyToFire() override;
	virtual int GetAmmoCount() override;
	virtual int GetMax() override;
	virtual void AddAmmo(int amount) override;

	USkeletalMesh* Mesh;
	UMaterialInterface* Material;
	UAudioComponent* GunSoundPlayer;
	USoundCue* FireSound;
	USoundBase* EmptySound;
	UAnimMontage* FireAnimation;
private:
	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	UMaterialInterface* decal = nullptr;
	UParticleSystem** particleSystems;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	int ammoCount = 8;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	int maxAmmo = 42;

	float FireTimer = 0.f;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	float FireCooldown = 0.3f;

	bool hasFired = false;
};
