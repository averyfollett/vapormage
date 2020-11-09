// Fill out your copyright notice in the Description page of Project Settings.

#define PRINT(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

#include "IceKnife.h"
#include "PlayerCharacter.h"

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

void AIceKnife::PlayRandomCastSound()
{
    UGameplayStatics::PlaySound2D(
        GetWorld(),
        CastSoundArray[FMath::RandRange(0, CastSoundArray.Num() - 1)]
        );
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
        PRINT("destroy ik");
    }
}

void AIceKnife::CastInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
    PlayRandomCastSound();
}
