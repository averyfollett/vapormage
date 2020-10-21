// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPulse.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"


// Sets default values
AGridPulse::AGridPulse()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Use a sphere as a simple collision representation.
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    // Set the sphere's collision radius.
    CollisionComponent->InitSphereRadius(15.0f);
    // Set the root component to be the collision component.
    RootComponent = CollisionComponent;


    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = InitialTravelSpeed;
    ProjectileMovementComponent->MaxSpeed = InitialTravelSpeed;             //have to change this if we want to have it gradually increase as time goes on
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = false;
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

void AGridPulse::CastInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}




