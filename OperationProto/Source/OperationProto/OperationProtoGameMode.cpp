// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "OperationProtoGameMode.h"
#include "OperationProtoHUD.h"
#include "OperationProtoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOperationProtoGameMode::AOperationProtoGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/MyOperationProtoCharacter.MyOperationProtoCharacter_C"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AOperationProtoHUD::StaticClass();
}
