// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_RandomChance.generated.h"

/**
 * 
 */
UCLASS()
class GRILLEDPIGEON_API UBTDecorator_RandomChance final : public UBTDecorator
{
	GENERATED_BODY()

public:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Chance)
    float Chance;
};
