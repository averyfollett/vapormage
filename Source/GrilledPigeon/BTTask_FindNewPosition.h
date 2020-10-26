// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindNewPosition.generated.h"

/**
 * 
 */
UCLASS()
class GRILLEDPIGEON_API UBTTask_FindNewPosition : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FindNewPosition();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Positon)
	float DistanceFromPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Positon)
	float Radius;
};
