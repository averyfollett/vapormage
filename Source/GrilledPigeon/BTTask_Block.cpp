// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Block.h"

#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTTask_Block::UBTTask_Block()
{
    
}

EBTNodeResult::Type UBTTask_Block::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // get self character
    AEnemyCharacter * Char = static_cast<AEnemyCharacter*>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>("SelfActor"));

    // call block function on character
    //Char->BlockSpell();


    return EBTNodeResult::Succeeded;
}
