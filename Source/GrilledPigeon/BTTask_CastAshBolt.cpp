// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_CastAshBolt.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CastAshBolt::UBTTask_CastAshBolt()
{
}

EBTNodeResult::Type UBTTask_CastAshBolt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // get self character
    AEnemyCharacter * Char = Cast<AEnemyCharacter>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>("SelfActor"));

    // call block function on character
    Char->CastAshBoltSpell();

    return EBTNodeResult::Succeeded;
}
