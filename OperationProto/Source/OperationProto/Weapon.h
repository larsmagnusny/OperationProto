// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
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
private:
	USkeletalMesh* WeaponMesh = nullptr;
	UMaterial* Material = nullptr;
};
