// Fill out your copyright notice in the Description page of Project Settings.

#define PRINT(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

#include "EnemyCharacter.h"
#include "ArcaneBolt.h"
#include "GrilledPigeonGameMode.h"
#include "IceKnife.h"
#include "AshBolt.h"
#include "PlayerCharacter.h"
#include "PlayerUI.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnCapsuleBeginOverlap);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	EnemyCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerCharacter::StaticClass()));
	PRINT(EnemyCharacter->GetName());

	CurrentVitality = MaxVitality;
	CurrentFocus = MaxFocus;
}

void AEnemyCharacter::RegenerateFocus()
{
	if (bCanRegenFocus)
	{
		if (CurrentFocus < MaxFocus)
			CurrentFocus += FocusRegenSpeed * GetWorld()->GetDeltaSeconds();
		else if (CurrentFocus > MaxFocus)
			CurrentFocus = MaxFocus;
	}
}

void AEnemyCharacter::EndBlockingStatus()
{
	EnemyStatus.bIsBlocking = false;
}

void AEnemyCharacter::ShowBlockIndicator(const int Direction) const
{
	//TESTING: call a function in the player's UI to pop up a Block Direction indicator.
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	UWidgetComponent* PlayerWidgetComponent= PlayerCharacter->PlayerUIComponent;
	UUserWidget* PlayerUI = PlayerWidgetComponent->GetUserWidgetObject();
	UPlayerUI* PlayerUI2 = Cast<UPlayerUI>(PlayerUI);
	
	PlayerUI2->ActivateBlockIndicator(Direction);
}
	
void AEnemyCharacter::DelayBeforeRegen()
{
	bCanRegenFocus = false;
    
	GetWorldTimerManager().SetTimer(
        RegenDelayTimerHandle, this, &AEnemyCharacter::SetRegenEnabled, RegenDelayLength, false);
}

void AEnemyCharacter::SetRegenEnabled()
{
	bCanRegenFocus = true;
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

void AEnemyCharacter::CastSparkSpell()
{
	// Attempt to fire a projectile.
	if (SparkSpellClass &&
		SparkFocusCost <= CurrentFocus)
	{
		PRINT("Enemy: Cast Spark");

		CurrentFocus -= SparkFocusCost;
		//Displays LEFT block.
		ShowBlockIndicator(3);
		ShowBlockIndicator(1);

		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Transform MuzzleOffset from camera space to world space.
		const FVector CastLocation = CameraLocation + FTransform(CameraRotation).TransformVector(CastOffset);
		FRotator CastRotation = CameraRotation;

		CastRotation.Pitch = 0.0f;  //we want a straight shot with no grav
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			// Spawn the projectile at the muzzle.
			AAshBolt* Projectile = World->SpawnActor<AAshBolt>(SparkSpellClass, CastLocation, CastRotation, SpawnParams);
			if (Projectile)
			{
				// Set the projectile's initial trajectory.
				const FVector LaunchDirection = CastRotation.Vector();
				Projectile->CastInDirection(LaunchDirection);
				//SetCastingStatus(true);
				DelayBeforeRegen();
			}
		}
	}
}

void AEnemyCharacter::CastAshBoltSpell()
{
	// Attempt to fire a projectile.
	if (AshBoltSpellClass &&
		AshBoltFocusCost <= CurrentFocus)
	{
		PRINT("Enemy: Cast Ash Bolt");
		
		//Displays RIGHT block.
		ShowBlockIndicator(1);
		ShowBlockIndicator(3);

		CurrentFocus -= AshBoltFocusCost;

		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Transform MuzzleOffset from camera space to world space.
		const FVector CastLocation = CameraLocation + FTransform(CameraRotation).TransformVector(CastOffset);
		FRotator CastRotation = CameraRotation;

		CastRotation.Pitch = 0.0f;  //we want a straight shot with no grav
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			// Spawn the projectile at the muzzle.
			AAshBolt* Projectile = World->SpawnActor<AAshBolt>(AshBoltSpellClass, CastLocation, CastRotation, SpawnParams);
			if (Projectile)
			{
				// Set the projectile's initial trajectory.
				const FVector LaunchDirection = CastRotation.Vector();
				Projectile->CastInDirection(LaunchDirection);
				//SetCastingStatus(true);
				DelayBeforeRegen();
			}
		}
	}
}

void AEnemyCharacter::CastFlamePoolSpell()
{
	//TO DO: Cast flame pool
}

float AEnemyCharacter::GetCurrentFocus() const
{
	return CurrentFocus;
}

float AEnemyCharacter::GetMaxFocus() const
{
	return MaxFocus;
}

float AEnemyCharacter::GetVitalityLossThreshold() const
{
	return VitalityLossThreshold;
}

void AEnemyCharacter::BlockSpell()
{
	EnemyStatus.bIsBlocking = true;
    
	GetWorldTimerManager().SetTimer(
        BlockingTimerHandle, this, &AEnemyCharacter::EndBlockingStatus, BlockingTimerLength, false);

	DelayBeforeRegen();
}

void AEnemyCharacter::DamageAI(const float Damage, const bool bWasBlocked)
{
	if (bCanTakeDamage)
	{
		if (CurrentFocus > 0 && bWasBlocked)
		{
			CurrentFocus -= Damage;
		}
		if ((CurrentFocus <= MaxFocus * VitalityLossThreshold && CurrentVitality > 0) ||
            !bWasBlocked)
		{
			CurrentVitality--;
			CurrentFocus = MaxFocus;
		}
		if (CurrentVitality <= 0)
		{
			AGrilledPigeonGameMode * GameMode = static_cast<AGrilledPigeonGameMode*>(GetWorld()->GetAuthGameMode());
			GameMode->SetGameWon();
		
			//TO DO: Enemy dies
		}
	}
}

void AEnemyCharacter::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AIceKnife::StaticClass()))
	{
		DamageAI(20, EnemyStatus.bIsBlocking);
	}
	if (OtherActor->IsA(AArcaneBolt::StaticClass()))
	{
		DamageAI(25, EnemyStatus.bIsBlocking);
	}
	// Add other spell collision checks here...
}
