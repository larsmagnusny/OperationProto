// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "AmmoBox.generated.h"

/**
 * 
 */
UCLASS()
class OPERATIONPROTO_API AAmmoBox : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	AAmmoBox();
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPickup(AActor* OurActor, AActor * OtherActor);
private:
	int ammoAmount = 30;

	UStaticMeshComponent* MeshComponent = nullptr;
};
