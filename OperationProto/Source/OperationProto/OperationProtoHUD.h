// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OperationProtoHUD.generated.h"

UCLASS()
class AOperationProtoHUD : public AHUD
{
	GENERATED_BODY()

public:
	AOperationProtoHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	float XPos = 0;
	float YPos = 0;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

