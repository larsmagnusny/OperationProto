// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "MachineGun.generated.h"

/**
 * 
 */
UCLASS()
class OPERATIONPROTO_API UMachineGun : public UWeapon
{
	GENERATED_BODY()
public:
	UMachineGun();
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
	USoundBase* FireSound;
	USoundBase* EmptySound;
	UAnimMontage* FireAnimation;
private:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	UMaterialInterface* decal = nullptr;
	UParticleSystem* particleSystem = nullptr;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	int ammoCount = 142;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	int maxAmmo = 142;

	float FireTimer = 0.f;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	float FireCooldown = 0.1f;

	bool hasFired = false;
	
};
