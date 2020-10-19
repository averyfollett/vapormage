// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Block.generated.h"

/**
 * 
 */
UCLASS()
class GRILLEDPIGEON_API UBTTask_Block final : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UBTTask_Block();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;
};
