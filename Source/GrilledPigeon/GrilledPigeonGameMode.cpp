// Copyright Epic Games, Inc. All Rights Reserved.

#include "GrilledPigeonGameMode.h"
#include "GrilledPigeonHUD.h"
#include "PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGrilledPigeonGameMode::AGrilledPigeonGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/PlayerCharacter/PlayerCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AGrilledPigeonHUD::StaticClass();
}
