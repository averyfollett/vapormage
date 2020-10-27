// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindNewPosition.h"
#include "EnemyCharacter.h"
#include "EnemyController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_FindNewPosition::UBTTask_FindNewPosition()
{
}

EBTNodeResult::Type UBTTask_FindNewPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // get self character
    AEnemyCharacter * Self = Cast<AEnemyCharacter>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>("SelfActor"));

    AEnemyController * EnemyController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
    
    AActor* Enemy = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(
        EnemyController->EnemyKeyID));

    const FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(Enemy->GetActorLocation(), Self->GetActorLocation());

    const FVector NewPosition = Enemy->GetActorLocation() + (Direction * DistanceFromPlayer);

    FNavLocation NewLocation;

    const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

    NavSystem->GetRandomReachablePointInRadius(NewPosition, Radius, NewLocation);

    OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(EnemyController->TargetPositionKeyID, NewLocation.Location);
    
    return EBTNodeResult::Succeeded;
}
