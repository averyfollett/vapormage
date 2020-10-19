// Fill out your copyright notice in the Description page of Project Settings.

#define PRINT(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

#include "EnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyCharacter.h"

AEnemyController::AEnemyController()
{
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComponent"));
}

void AEnemyController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    AEnemyCharacter * Char = static_cast<AEnemyCharacter*>(InPawn);

    if (Char && Char->BehaviorTree)
    {
        BlackboardComponent->InitializeBlackboard(*Char->BehaviorTree->BlackboardAsset);

        EnemyKeyID = BlackboardComponent->GetKeyID("EnemyActor");
        SelfKeyID = BlackboardComponent->GetKeyID("SelfActor");
        ShouldBlockKeyID = BlackboardComponent->GetKeyID("bShouldBlock");

        BehaviorTreeComponent->StartTree(*Char->BehaviorTree);
    }
}
