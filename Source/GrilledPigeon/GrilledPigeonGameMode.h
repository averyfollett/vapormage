// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GrilledPigeonGameMode.generated.h"

UCLASS(minimalapi)
class AGrilledPigeonGameMode final : public AGameModeBase
{
    GENERATED_BODY()

public:
    AGrilledPigeonGameMode();

    UFUNCTION(BlueprintCallable, Category=State)
    void SetGameLost();

    UFUNCTION(BlueprintCallable, Category=State)
    void SetGameWon();
    
    UFUNCTION(BlueprintCallable, Category=UI)
    void CreateLossWidget() const;

    UFUNCTION(BlueprintCallable, Category=UI)
    void CreateWinWidget() const;

    UFUNCTION(BlueprintCallable, Category=State)
    void PauseAllEnemies() const;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=State)
    bool bIsGameLost = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=State)
    bool bIsGameWon = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=UI)
    TSubclassOf<UUserWidget> LossWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=UI)
    TSubclassOf<UUserWidget> WinWidget;
};
