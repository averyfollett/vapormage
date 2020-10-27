// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_CastIceKnife.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CastIceKnife::UBTTask_CastIceKnife()
{
}

EBTNodeResult::Type UBTTask_CastIceKnife::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // get self character
    AEnemyCharacter * Char = Cast<AEnemyCharacter>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>("SelfActor"));

    // call block function on character
    Char->CastIceKnifeSpell();

    return EBTNodeResult::Succeeded;
}
