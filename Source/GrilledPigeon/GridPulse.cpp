// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPulse.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"


// Sets default values
AGridPulse::AGridPulse()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridPulse::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridPulse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGridPulse::CastInDirection(const FVector& ShootDirection, AActor* Enemy, AActor* Player)
{
    EnemyActor = static_cast<AEnemyCharacter*>(Enemy);
    PlayerActor = static_cast<APlayerCharacter*>(Player);
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AGridPulse::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
 {
     //if we hit physics simulated object that isnt the player
     if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
     {
         OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * ImpactForce, Hit.ImpactPoint);
     }

     //player has been hit boi, gank him
     if (OtherActor->IsA(APlayerCharacter::StaticClass()))
     {
         if (PlayerActor)
         {
             
         }
     }

     //if enemy has been hit
     if (OtherActor->IsA(AEnemyCharacter::StaticClass()))
     {
         if (EnemyActor)
         {

         }
     }
 }


