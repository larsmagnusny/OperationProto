// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "HandGun.generated.h"


UCLASS()
class OPERATIONPROTO_API UHandGun : public UWeapon
{
	GENERATED_BODY()
public:
	UHandGun();
	virtual void BeginPlay() override;
	virtual FRotator GetPointingAt(FHitResult& hit) override;
	virtual void Update(float DeltaTime) override;
	virtual void Fire(bool& canFireAfter) override;
	virtual bool ReadyToFire() override;
	virtual int GetAmmoCount() override;
	virtual int GetMax() override;
	virtual void AddAmmo(int amount) override;
private:
	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;
};
