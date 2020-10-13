// Fill out your copyright notice in the Description page of Project Settings.


#include "IceKnifeTwo.h"

// Sets default values
AIceKnifeTwo::AIceKnifeTwo()
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
    ProjectileMovementComponent->InitialSpeed = 3000.0f;
    ProjectileMovementComponent->MaxSpeed = 3000.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = true;
    ProjectileMovementComponent->Bounciness = 0.3f;
}

// Called when the game starts or when spawned
void AIceKnifeTwo::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AIceKnifeTwo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIceKnifeTwo::CastInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}