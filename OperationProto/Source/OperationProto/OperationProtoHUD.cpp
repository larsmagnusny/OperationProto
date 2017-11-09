// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "OperationProtoHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"

AOperationProtoHUD::AOperationProtoHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	static ConstructorHelpers::FClassFinder<UUserWidget> HudLoader(TEXT("/Game/FirstPerson/UI/HUD"));

	if(CrosshairTexObj.Succeeded())
		CrosshairTex = CrosshairTexObj.Object;

	if (HudLoader.Succeeded())
	{
		HudWidgetTemplate = HudLoader.Class;
	}
}

void AOperationProtoHUD::BeginPlay()
{
	Super::BeginPlay();

	// Get the player and not spectators
	for (TActorIterator<AOperationProtoCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ControlledCharacter = *ActorItr;
	}

	UE_LOG(LogTemp, Warning, TEXT("Beginplay"));

	if (HudWidgetTemplate)
	{
		HudWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), HudWidgetTemplate);

		if (HudWidget)
		{
			HudWidget->AddToViewport();
		}
	}
}

int AOperationProtoHUD::GetAmmoCount()
{
	//return ControlledCharacter->GetAmmoCount();
	return 0;
}

void AOperationProtoHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(XPos, YPos);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X-10),
										   (Center.Y-10));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}


