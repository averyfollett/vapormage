// Fill out your copyright notice in the Description page of Project Settings.

#include "IceKnife.h"
#include "PlayerCharacter.h"

#define PRINT(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)


// Sets default values
AIceKnife::AIceKnife()
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
    ProjectileMovementComponent->InitialSpeed = TravelSpeed;
    ProjectileMovementComponent->MaxSpeed = TravelSpeed;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = false;
    //ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0;

    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AIceKnife::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AIceKnife::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AIceKnife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIceKnife::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
    {
        //Destroy();
        this->Destroy();
        PRINT("DESTROY THIS");
    }
}

void AIceKnife::CastInDirection(const FVector& ShootDirection) const
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}
