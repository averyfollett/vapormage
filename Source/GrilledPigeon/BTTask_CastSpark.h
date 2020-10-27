// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CastSpark.generated.h"

/**
 * 
 */
UCLASS()
class GRILLEDPIGEON_API UBTTask_CastSpark final : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UBTTask_CastSpark();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;
};
