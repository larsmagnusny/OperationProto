// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "TargetableActor.generated.h"

/**
 * 
 */
UCLASS()
class OPERATIONPROTO_API ATargetableActor : public ASkeletalMeshActor
{
	GENERATED_BODY()
public:
	ATargetableActor();
	int GetHP();

	void ApplyDamage(int d);

	UFUNCTION(BlueprintCallable, Category = "Is Target Dead?")
	bool isDead();
private:
	int HP = 100;
};
