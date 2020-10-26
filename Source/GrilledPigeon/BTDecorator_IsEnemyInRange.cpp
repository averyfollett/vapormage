// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsEnemyInRange.h"
#include "EnemyController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTDecorator_IsEnemyInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    APawn * Self = OwnerComp.GetAIOwner()->GetPawn();

    AEnemyController * EnemyController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
    
    AActor* Enemy = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(
        EnemyController->EnemyKeyID));
    
    if (FVector::Dist(Self->GetActorLocation(), Enemy->GetActorLocation()) <= Range)
        return true;
    else
        return false;
}
