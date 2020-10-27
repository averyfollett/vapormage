// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <string>

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PrintString.generated.h"

/**
 * 
 */
UCLASS()
class GRILLEDPIGEON_API UBTTask_PrintString : public UBTTaskNode
{
	GENERATED_BODY()

public:
    UBTTask_PrintString();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=String)
    FString String;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=String)
    FColor TextColor = FColor::Green;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=String)
    float TimeToDisplay;
};
