// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class GRILLEDPIGEON_API AEnemyController final : public AAIController
{
	GENERATED_BODY()

protected:
    UPROPERTY(Transient)
    class UBlackboardComponent * BlackboardComponent;

    UPROPERTY(Transient)
    class UBehaviorTreeComponent * BehaviorTreeComponent;

public:
    AEnemyController();

    virtual void OnPossess(APawn* InPawn) override;

    uint8 EnemyKeyID;
    uint8 SelfKeyID;
    uint8 ShouldBlockKeyID;
};
