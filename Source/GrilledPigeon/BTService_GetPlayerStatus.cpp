// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_GetPlayerStatus.h"

#include "EnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "EnemyController.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

UBTService_GetPlayerStatus::UBTService_GetPlayerStatus()
{
    bCreateNodeInstance = true;
}

void UBTService_GetPlayerStatus::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AEnemyController * EnemyController = static_cast<AEnemyController*>(OwnerComp.GetAIOwner());
    
    APlayerCharacter* Enemy = static_cast<APlayerCharacter*>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(
        EnemyController->EnemyKeyID));

    AEnemyCharacter* Self = Cast<AEnemyCharacter>(EnemyController->GetPawn());

    if (IsValid(EnemyController) && IsValid(Enemy) && IsValid(&OwnerComp))
    {
        if (!(Self->GetCurrentFocus() < (Self->GetMaxFocus() * Self->GetVitalityLossThreshold())))
        {
            OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(EnemyController->ShouldBlockKeyID, Enemy->PlayerStatus.bIsCasting);
        } else
        {
            OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(EnemyController->ShouldBlockKeyID, false);
        }
        
    }
        
}
