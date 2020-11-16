// Copyright Epic Games, Inc. All Rights Reserved.

#define PRINT(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

#include "GrilledPigeonGameMode.h"

#include "EnemyCharacter.h"
#include "EnemyController.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AGrilledPigeonGameMode::AGrilledPigeonGameMode()
    : Super()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(
        TEXT("/Game/Blueprints/PlayerCharacter/BP_PlayerCharacter"));
    DefaultPawnClass = PlayerPawnClassFinder.Class;

    // use our custom HUD class
    static ConstructorHelpers::FClassFinder<AHUD> BP_HUD(TEXT("/Game/Blueprints/UI/InGameUI/BP_HUD"));
    HUDClass = BP_HUD.Class;
}

void AGrilledPigeonGameMode::SetGameLost()
{
    if (!bIsGameWon)
    {
        bIsGameLost = true;
        PauseAllEnemies();
        CreateLossWidget();
    }
}

void AGrilledPigeonGameMode::SetGameWon()
{
    if (!bIsGameLost)
    {
        bIsGameWon = true;
        PauseAllEnemies();
        CreateWinWidget();
    }
}

void AGrilledPigeonGameMode::CreateLossWidget() const
{
    UUserWidget * Widget = CreateWidget<UUserWidget>(GetWorld(), LossWidget);
    Widget->AddToViewport();
    APlayerController * PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    PC->bShowMouseCursor = true;
    PC->bEnableClickEvents = true; 
    PC->bEnableMouseOverEvents = true;
}

void AGrilledPigeonGameMode::CreateWinWidget() const
{
    UUserWidget * Widget = CreateWidget<UUserWidget>(GetWorld(), WinWidget);
    Widget->AddToViewport();
    APlayerController * PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    PC->bShowMouseCursor = true;
    PC->bEnableClickEvents = true; 
    PC->bEnableMouseOverEvents = true;
}

void AGrilledPigeonGameMode::PauseAllEnemies() const
{
    TArray<AActor*> EnemyActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), EnemyActors);
    for (int i = 0; i < EnemyActors.Num(); i++)
    {
        Cast<AEnemyController>(Cast<AEnemyCharacter>(EnemyActors[i])->GetController())->PauseBehaviourTree();
    }
}
