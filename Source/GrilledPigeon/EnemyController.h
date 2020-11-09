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

public:
    AEnemyController();

    UFUNCTION(BlueprintCallable)
    void UnPauseBehaviourTree() const;

    virtual void OnPossess(APawn* InPawn) override;

public:
    uint8 EnemyKeyID;
    uint8 SelfKeyID;
    uint8 ShouldBlockKeyID;
    uint8 TargetPositionKeyID;

protected:
    UPROPERTY(Transient)
    class UBlackboardComponent * BlackboardComponent;

    UPROPERTY(Transient)
    class UBehaviorTreeComponent * BehaviorTreeComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=BehaviorTree)
    bool bStartBehaviorTreePaused = true;
};
