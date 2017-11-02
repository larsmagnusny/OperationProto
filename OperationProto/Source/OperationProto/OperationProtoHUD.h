// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "OperationProtoCharacter.h"
#include "OperationProtoHUD.generated.h"

UCLASS()
class AOperationProtoHUD : public AHUD
{
	GENERATED_BODY()

public:
	AOperationProtoHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Get Ammo Count")
	int GetAmmoCount();

	float XPos = 0;
	float YPos = 0;

	UClass* HudWidgetTemplate = nullptr;
	UUserWidget* HudWidget = nullptr;
	AOperationProtoCharacter* ControlledCharacter = nullptr;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex = nullptr;

};

