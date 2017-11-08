// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ParticleDefinitions.h"
#include "GenericTeamAgentInterface.h"
#include "OperationProtoCharacter.generated.h"

class UInputComponent;
class UWeapon;

UCLASS(config=Game)
class AOperationProtoCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	AOperationProtoCharacter();

	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);

	void SwitchWeaponUp();
	void SwitchWeaponDown();

	void FireDown();
	void FireUp();

	void MoveForward(float Val);
	void MoveRight(float Val);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TEAM")
	void SetGenericTeamId(const FGenericTeamId& TeamID);
	void SetGenericTeamId_Implementation(const FGenericTeamId& TeamID);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "TEAM")
	FGenericTeamId GetGenericTeamId() const;
	FGenericTeamId GetGenericTeamId_Implementation() const;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	TArray<UWeapon*> FP_Gun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	int GetAmmoCount();
	int GetMax();
	void AddAmmo(int amount);
private:
	bool firePressed = false;
	int CurrentWeapon = 0;
	int NumWeapons = 2;

	FGenericTeamId TeamId;
};

