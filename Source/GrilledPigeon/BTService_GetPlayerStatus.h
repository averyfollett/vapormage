// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_GetPlayerStatus.generated.h"

/**
 * 
 */
UCLASS()
class GRILLEDPIGEON_API UBTService_GetPlayerStatus final : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_GetPlayerStatus();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
