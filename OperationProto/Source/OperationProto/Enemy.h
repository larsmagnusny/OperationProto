// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TextRenderComponent.h"
#include "Enemy.generated.h"

UCLASS()
class OPERATIONPROTO_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ApplyDamage(int amount);
	
	int Health = 100;
	USkeletalMeshComponent* MeshComponent = nullptr;
	UCapsuleComponent* Capsule = nullptr;
	UTextRenderComponent* TextRender = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Print Text")
	void PrintText(FString Text);
};
