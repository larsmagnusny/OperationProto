// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "MachineGun.generated.h"

class USoundCue;
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
	virtual FRotator GetPointingAt(FHitResult& hit) override;
	virtual void Update(float DeltaTime) override;
	virtual void Fire(bool& canFireAfter) override;
	virtual bool ReadyToFire() override;
	virtual int GetAmmoCount() override;
	virtual int GetMax() override;
	virtual void AddAmmo(int amount) override;
private:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;	
};
