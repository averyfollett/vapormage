// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_RandomChance.h"

#include "Kismet/KismetMathLibrary.h"

bool UBTDecorator_RandomChance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    return UKismetMathLibrary::RandomBoolWithWeight(Chance);
}
