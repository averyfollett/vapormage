// Fill out your copyright notice in the Description page of Project Settings.


#include "AshBolt.h"
#include "PlayerCharacter.h"


// Sets default values
AAshBolt::AAshBolt()
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
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = false;
    //ProjectileMovementComponent->Bounciness = 0.3f;
    ProjectileMovementComponent->bIsHomingProjectile = true;
    ProjectileMovementComponent->HomingAccelerationMagnitude = InitialTravelSpeed + TrackingTravelSpeed;
    ProjectileMovementComponent->ProjectileGravityScale = 0;

    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AAshBolt::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AAshBolt::BeginPlay()
{
	Super::BeginPlay();
    APlayerCharacter* Wizard = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    //ProjectileMovementComponent->HomingTargetComponent = Wizard->EnemyActor;
    ProjectileMovementComponent->HomingTargetComponent = Wizard->GetRootComponent();

    ProjectileMovementComponent->InitialSpeed = InitialTravelSpeed;
    ProjectileMovementComponent->MaxSpeed = InitialTravelSpeed;
}

void AAshBolt::PlayRandomCastSound()
{
    UGameplayStatics::PlaySound2D(
        GetWorld(),
        CastSoundArray[FMath::RandRange(0, CastSoundArray.Num() - 1)]
    );
}

void AAshBolt::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
    {
        //Destroy();
        this->Destroy();
    }
}

// Called every frame
void AAshBolt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAshBolt::CastInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
    PlayRandomCastSound();
}

