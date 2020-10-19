// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/BlackboardComponent.h"
#include "BTService_GetPlayerStatus.h"

#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "EnemyController.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

class AEnemyController;

UBTService_GetPlayerStatus::UBTService_GetPlayerStatus()
{
    bCreateNodeInstance = true;
}

void UBTService_GetPlayerStatus::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AEnemyController * EnemyController = static_cast<AEnemyController*>(OwnerComp.GetAIOwner());
    
    APlayerCharacter* Enemy = static_cast<APlayerCharacter*>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(
        EnemyController->EnemyKeyID));

    OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(EnemyController->ShouldBlockKeyID, Enemy->PlayerStatus.bIsCasting);
}
