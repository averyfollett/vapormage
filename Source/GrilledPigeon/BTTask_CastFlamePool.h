// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CastFlamePool.generated.h"

/**
 * 
 */
UCLASS()
class GRILLEDPIGEON_API UBTTask_CastFlamePool : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_CastFlamePool();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;
};
