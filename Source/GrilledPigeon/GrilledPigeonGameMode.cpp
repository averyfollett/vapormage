// Copyright Epic Games, Inc. All Rights Reserved.

#include "GrilledPigeonGameMode.h"
#include "PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/HUD.h"

AGrilledPigeonGameMode::AGrilledPigeonGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/PlayerCharacter/BP_PlayerCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	static ConstructorHelpers::FClassFinder<AHUD> BP_HUD(TEXT("/Game/Blueprints/UI/BP_HUD"));
	HUDClass = BP_HUD.Class;
}
