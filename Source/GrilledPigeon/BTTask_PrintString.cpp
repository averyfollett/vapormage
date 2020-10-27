// Fill out your copyright notice in the Description page of Project Settings.

#define PRINT(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

#include "BTTask_PrintString.h"

UBTTask_PrintString::UBTTask_PrintString()
{
}

EBTNodeResult::Type UBTTask_PrintString::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    GEngine->AddOnScreenDebugMessage(-1, TimeToDisplay, TextColor, String);

    return EBTNodeResult::Succeeded;
}
