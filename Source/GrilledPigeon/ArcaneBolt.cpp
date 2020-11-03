// Fill out your copyright notice in the Description page of Project Settings.

#define PRINT(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

#include "ArcaneBolt.h"
#include "PlayerCharacter.h"

// Sets default values
AArcaneBolt::AArcaneBolt()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    //sMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sMesh0"));
    //sMesh0->SetOnlyOwnerSee(true);
    //sMesh->bCastDynamicShadow = false;
    //sMesh->CastShadow = false;
    //sMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
    //sMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    
    //sMesh0->SetSimulatePhysics(true);

    // Use a sphere as a simple collision representation.
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    // Set the sphere's collision radius.
    CollisionComponent->InitSphereRadius(15.0f);

    //don't let actors jump onto the projectile
    CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
    CollisionComponent->CanCharacterStepUpOn = ECB_No;

    // Set the root component to be the collision component.
    RootComponent = CollisionComponent;

    //SetLifeSpan(LifeSpan);

    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = InitialTravelSpeed;
    ProjectileMovementComponent->MaxSpeed = InitialTravelSpeed; //have to change this if we want to have it gradually increase as time goes on
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = false;
    ProjectileMovementComponent->bIsHomingProjectile = true;
    ProjectileMovementComponent->HomingAccelerationMagnitude = InitialTravelSpeed + TrackingTravelSpeed;
    ProjectileMovementComponent->ProjectileGravityScale = 0;

    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AArcaneBolt::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AArcaneBolt::BeginPlay()
{
	Super::BeginPlay();
    
    APlayerCharacter* Wizard = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    //ProjectileMovementComponent->HomingTargetComponent = Wizard->EnemyActor;
    ProjectileMovementComponent->HomingTargetComponent = Wizard->GetEnemyActor()->GetRootComponent();
}

// Called every frame
void AArcaneBolt::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

    APlayerCharacter* Wizard = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    if (Wizard != nullptr)
    {
        //EnemyActor = Cast<ACharacter>(wizard->EnemyActor);
        EnemyActorLocation = Wizard->EnemyActorLocation.GetSafeNormal();
    }
    
    //ProjectileMovementComponent->HomingTargetComponent = EnemyActorLocation;

    TargetHoming(); //actually home onto the target
}

void AArcaneBolt::TargetHoming()
{
    //FVector target = EnemyActorLocation;
    //FVector velocityToTarget = target - this->GetActorLocation();   //get the difference to have direction
    //velocityToTarget *= TrackingTravelSpeed;                        //impulse increase before attaching it to current direction
    //velocityToTarget += GetVelocity();                              //get current velocity and take its redirection velocityToTarget

    //sMesh->AddImpulseAtLocation(velocityToTarget * 1000, GetActorLocation());

    //PRINT("VelocityToTarget:  " + FString::SanitizeFloat(velocityToTarget.X) + ", " + FString::SanitizeFloat(velocityToTarget.Y) + ", " + FString::SanitizeFloat(velocityToTarget.Z));
    //PRINT("TargetLoc:  " + FString::SanitizeFloat(target.X) + ", " + FString::SanitizeFloat(target.Y) + ", " + FString::SanitizeFloat(target.Z));
    //PRINT("this loc:  " + FString::SanitizeFloat(this->GetActorLocation().X) + ", " + FString::SanitizeFloat(this->GetActorLocation().Y) + ", " + FString::SanitizeFloat(this->GetActorLocation().Z));
}

void AArcaneBolt::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr)
    {
        this->Destroy();
        PRINT("destroy ab");
    }
}

void AArcaneBolt::CastInDirection(const FVector& ShootDirection) const
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}
