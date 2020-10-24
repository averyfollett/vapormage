// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckForPlayer.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "EnemyController.h"
#include "PlayerCharacter.h"

UBTService_CheckForPlayer::UBTService_CheckForPlayer()
{
    bCreateNodeInstance = true;
}

void UBTService_CheckForPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AEnemyController * EnemyController = Cast<AEnemyController>(OwnerComp.GetAIOwner());

    if (EnemyController)
    {
        APlayerCharacter * Enemy = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

        if (Enemy)
        {
            OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(EnemyController->EnemyKeyID, Enemy);
        }
    }
}
