// Fill out your copyright notice in the Description page of Project Settings.

#define PRINT(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

#include "EnemyCharacter.h"
#include "ArcaneBolt.h"
#include "IceKnife.h"
#include "GridPulse.h"

#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	EnemyCharacter = static_cast<APlayerCharacter*>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass()));
	PRINT(EnemyCharacter->GetName());
}

// Called every frame
void AEnemyCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	RegenerateFocus();
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::CastIceKnifeSpell() const
{
	// Attempt to fire a projectile.
	/*if (IsValid(IKSpellClass))
	{
		
	}*/
}

void AEnemyCharacter::BlockSpell()
{
	PRINT("Blocking...");
}

void AEnemyCharacter::RegenerateFocus()
{
	if (CurrentFocus < MaxFocus)
		CurrentFocus += FocusRegenSpeed * GetWorld()->GetDeltaSeconds();
	else if (CurrentFocus > MaxFocus)
		CurrentFocus = MaxFocus;
}

void AEnemyCharacter::DamageAI(const float Damage)
{
	if (CurrentFocus > 0)
	{
		CurrentFocus -= Damage;
	}
	if (CurrentFocus <= MaxFocus * VitalityLossThreshold && CurrentVitality > 0)
	{
		CurrentVitality--;
	}
}

void AEnemyCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	//hit by grid pulse spell, do following effects
    if (OtherActor->IsA(AGridPulse::StaticClass()))
    {
		//knock back
		HitComponent->AddImpulseAtLocation(-GetActorForwardVector() * 300.0f, Hit.ImpactPoint);
		
		//vitality effects
		DamageAI(GridPulseDamageThreshold);
    }

	//hit by ice knife spell, do following effects
    if (OtherActor->IsA(AIceKnife::StaticClass()))
    {
		//vitality effects
		DamageAI(IKDamageThreshold);
    }

	//hit by arcane bolt spell, do following effects
	if (OtherActor->IsA(AArcaneBolt::StaticClass()))
	{
		//vitality effects
		DamageAI(ArcaneDamageThreshold);
	}

}