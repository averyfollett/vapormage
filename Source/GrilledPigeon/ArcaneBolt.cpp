// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcaneBolt.h"

// Sets default values
AArcaneBolt::AArcaneBolt()
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
    //ProjectileMovementComponent->Bounciness = 0.3f;
}

// Called when the game starts or when spawned
void AArcaneBolt::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArcaneBolt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    TargetHoming(); //actually home onto the target
}

void AArcaneBolt::TargetHoming()
{
    //null check for target
    if (EnemyActor)
    {
        FVector target = EnemyActor->GetActorLocation();
        FVector velocityToTarget = target - this->GetActorLocation();   //get the difference to have direction
        velocityToTarget *= TrackingTravelSpeed;                        //impulse increase before attaching it to current direction
        velocityToTarget += GetVelocity();                              //get current velocity and take its redirection velocityToTarget

        sMesh0->AddImpulseAtLocation(velocityToTarget, GetActorLocation());
    }
}

void AArcaneBolt::CastInDirection(const FVector& ShootDirection, AActor* Enemy)
{
    EnemyActor = static_cast<ACharacter*>(Enemy);
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

