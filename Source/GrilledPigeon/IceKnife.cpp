// Fill out your copyright notice in the Description page of Project Settings.


#include "IceKnife.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"

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

void AIceKnife::CastInDirection(const FVector& ShootDirection) const
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AIceKnife::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
     APlayerCharacter* wizard = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));


     //player has been hit boi, gank him
     if (OtherActor->IsA(APlayerCharacter::StaticClass()))
     {
         wizard->DamagePlayer(Damage);
     }


     if (OtherActor->IsA(AEnemyCharacter::StaticClass()))
     {
         Cast<AEnemyCharacter>(wizard->getEnemyActor())->DamageAI(Damage);
     }
}

