// Fill out your copyright notice in the Description page of Project Settings.

#define PRINT(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ArcaneBolt.h"
#include "IceKnife.h"
#include "IceKnifeTwo.h"
#include "IceKnifeTwoVarTwo.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    // set our turn rates for input
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;

    // Create a CameraComponent	
    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
    FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    // Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
    Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
    Mesh1P->SetOnlyOwnerSee(true);
    Mesh1P->SetupAttachment(FirstPersonCameraComponent);
    Mesh1P->bCastDynamicShadow = false;
    Mesh1P->CastShadow = false;
    Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
    Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Pick the first actor tagged with Enemy and set it as the target
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Enemy"), EnemyArray);
    EnemyActor = EnemyArray[CurrentEnemyIndex];
    
    // Print to screen the enemy actor name
    PRINT("Enemy: " + EnemyActor->GetName());

    //set timers
    DelayTimerCurrent = DelayTimerMax;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    SequenceOut(this->InputComponent->GetAxisValue(TEXT("TurnRate")),
                -(this->InputComponent->GetAxisValue(TEXT("LookUpRate"))),
                Ibr);
    //print(FString::SanitizeFloat(this->InputComponent->GetAxisValue(TEXT("LookUpRate"))));

	Cast();
	
    AutoAimAtEnemy(EnemyActor, FName("spine_03Socket"));

    RegenerateFocus();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // set up gameplay key bindings
    check(PlayerInputComponent);

    // Bind shoulder buttons
    PlayerInputComponent->BindAction("SwitchEnemyLeft", IE_Pressed, this, &APlayerCharacter::PreviousEnemy);
    PlayerInputComponent->BindAction("SwitchEnemyRight", IE_Pressed, this, &APlayerCharacter::NextEnemy);

    // Bind movement events
    PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

    // We have 2 versions of the rotation bindings to handle different kinds of devices differently
    // "turn" handles devices that provide an absolute delta, such as a mouse.
    // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);
}

void APlayerCharacter::MoveForward(float Value)
{
    if (Value != 0.0f)
    {
        // add movement in that direction
        AddMovementInput(GetActorForwardVector(), Value);
    }
}

void APlayerCharacter::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        // add movement in that direction
        AddMovementInput(GetActorRightVector(), Value);
    }
}

void APlayerCharacter::TurnAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::NextEnemy()
{
    if (CurrentEnemyIndex + 1 < EnemyArray.Num())
    {
        CurrentEnemyIndex++;
        EnemyActor = EnemyArray[CurrentEnemyIndex];
    }
    else if (CurrentEnemyIndex + 1 >= EnemyArray.Num())
    {
        CurrentEnemyIndex = 0;
        EnemyActor = EnemyArray[CurrentEnemyIndex];
    }
    PRINT("Next Enemy: " + EnemyActor->GetName() + "Index " + FString::FromInt(CurrentEnemyIndex));
}

void APlayerCharacter::PreviousEnemy()
{
    if (CurrentEnemyIndex - 1 < EnemyArray.Num() && CurrentEnemyIndex - 1 >= 0)
    {
        CurrentEnemyIndex--;
        EnemyActor = EnemyArray[CurrentEnemyIndex];
    }
    else if (CurrentEnemyIndex - 1 < 0)
    {
        CurrentEnemyIndex = EnemyArray.Num() - 1;
        EnemyActor = EnemyArray[CurrentEnemyIndex];
    }
    PRINT("Prev Enemy: " + EnemyActor->GetName() + "Index " + FString::FromInt(CurrentEnemyIndex));
}

AActor* APlayerCharacter::CapsuleTraceForEnemy() const
{
    FHitResult HitActor;
    FVector StartPos = FirstPersonCameraComponent->GetComponentLocation();
    FVector EndPos = FirstPersonCameraComponent->GetComponentLocation() + (FirstPersonCameraComponent->
        GetForwardVector() * 1500.0f);
    FCollisionShape Capsule = FCollisionShape::MakeCapsule(FVector(100.0f, 100.0f, 100.0f));
    const FName TraceTag("LockOnTrace");
    GetWorld()->DebugDrawTraceTag = TraceTag;
    FCollisionQueryParams CollisionParams;
    CollisionParams.TraceTag = TraceTag;

    GetWorld()->SweepSingleByObjectType(HitActor, StartPos, EndPos, FQuat(), ECC_Pawn, Capsule, CollisionParams);

    return HitActor.GetActor();
}

void APlayerCharacter::AutoAimAtEnemy(AActor* Enemy, FName SocketName) const
{
    // Cast actor to character class
    ACharacter* EnemyCharacter = static_cast<ACharacter*>(Enemy);

    // Get socket from enemy skeleton
    USkeletalMeshSocket const* EnemySocket = EnemyCharacter->GetMesh()->GetSocketByName(SocketName);

    // Calculate the target rotation
    const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation
        (FirstPersonCameraComponent->GetComponentLocation(), EnemySocket->GetSocketLocation(EnemyCharacter->GetMesh()));

    // Interpolate between current and target rotation
    const FRotator NewRotation = FMath::RInterpTo
        (GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 10.0f);

    // Set rotation to interpolated rotation
    GetController()->SetControlRotation(NewRotation);
}

void APlayerCharacter::SequenceOut(const float XAxis, const float YAxis, const float InputBufferRadius)
{
    //don't bother doing this if we don't care about gathering the input, save on some input load times
    if (GatheringSequence)
    {
        /*if (UpdatingSequence.Num() > 0)
        {
            print("Number in Sequence: " + FString::FromInt(UpdatingSequence.Num()));
        }*/


        if (DelayTimerCurrent <= 0)
        {
            //combine and release whatever sequence we have
            //	even if we are constantly gathering, but failing the buffer range check, its okay to release a blank sequence
            OutputSequence = ConcatSequence();

            //reset times

            //DEBUG
            const int OS = static_cast<int>(OutputSequence);

            if (OS != 0)
            {
                PRINT(FString::FromInt(OS)); //display when sequence isn't empty
            }
        }

        //check for significant movement in the stick
        if (BufferRangeCheck(InputBufferRadius, XAxis, YAxis) == false)
        {
            //print("passing buffer range check");
            //no collision, we have stick movement, record the values
            NextInSequence(XAxis, YAxis);

            DelayTimerCurrent = DelayTimerMax;
        }


        if (DelayTimerCurrent >= 0)
        {
            DelayTimerCurrent--;
        }

        //print("Timer: " + FString::FromInt(delayTimerCurrent));
    }

    //make sure this is at very end for next update tick
    PreviousInput.Key = XAxis;
    PreviousInput.Value = YAxis;
}

bool APlayerCharacter::BufferRangeCheck(const float InputBufferRadius, const float CurrentAnalogPosX,
                                        const float CurrentAnalogPosY) const
{
    const float XDist = CurrentAnalogPosX - PreviousInput.Key;
    const float YDist = CurrentAnalogPosY - PreviousInput.Value;
    const float Distance = sqrt((XDist * XDist) + (YDist * YDist));

    if (Distance <= InputBufferRadius)
    {
        return true;
    }
    return false;
}

void APlayerCharacter::NextInSequence(float XAxis, float YAxis)
{
    for (int i = 0; i < Asigs.MGrid.Num(); ++i)
    {
        const FCoord_Cell Rect = Asigs.MGrid[i].MCellCords;
        //print(FString::FromInt(i));

        //basic point box collision detection
        if (XAxis >= Rect.M00.Key && XAxis <= Rect.M01.Key &&
            YAxis <= Rect.M00.Value && YAxis >= Rect.M10.Value)
        {
            //print("passed box collision");

            switch (static_cast<int32>(Asigs.MGrid[i].MCellNum))
            {
            case static_cast<int32>(Asigs_A):
                //check and see if we are still in the previous cell. If so, we don't need to record it again.
                if (UpdatingSequence.Num() > 0)
                {
                    if (UpdatingSequence.Last() == Asigs_A)
                    {
                        //print("Still in cell");
                    }
                    else
                    {
                        UpdatingSequence.Add(Asigs_A);
                        //print("A Cell");
                    }
                }
                else
                {
                    UpdatingSequence.Add(Asigs_A);
                    //print("A Cell");
                }
                break;
            case static_cast<int32>(Asigs_B):
                //check and see if we are still in the previous cell. If so, we don't need to record it again.
                if (UpdatingSequence.Num() > 0)
                {
                    if (UpdatingSequence.Last() == Asigs_B)
                    {
                        //print("Still in cell");
                    }
                    else
                    {
                        UpdatingSequence.Add(Asigs_B);
                        //print("B Cell");
                    }
                }
                else
                {
                    UpdatingSequence.Add(Asigs_B);
                    //print("B Cell");
                }
                break;
            case static_cast<int32>(Asigs_C):
                //check and see if we are still in the previous cell. If so, we don't need to record it again.
                if (UpdatingSequence.Num() > 0)
                {
                    if (UpdatingSequence.Last() == Asigs_C)
                    {
                        //print("Still in cell");
                    }
                    else
                    {
                        UpdatingSequence.Add(Asigs_C);
                        //print("C Cell");
                    }
                }
                else
                {
                    UpdatingSequence.Add(Asigs_C);
                    //print("C Cell");
                }
                break;
            case static_cast<int32>(Asigs_D):
                //check and see if we are still in the previous cell. If so, we don't need to record it again.
                if (UpdatingSequence.Num() > 0)
                {
                    if (UpdatingSequence.Last() == Asigs_D)
                    {
                        //print("Still in cell");
                    }
                    else
                    {
                        UpdatingSequence.Add(Asigs_D);
                        //print("D Cell");
                    }
                }
                else
                {
                    UpdatingSequence.Add(Asigs_D);
                    //print("D Cell");
                }
                break;
            case static_cast<int32>(Asigs_E):
                //check and see if we are still in the previous cell. If so, we don't need to record it again.
                if (UpdatingSequence.Num() > 0)
                {
                    if (UpdatingSequence.Last() == Asigs_E)
                    {
                        //print("Still in cell");
                    }
                    else
                    {
                        UpdatingSequence.Add(Asigs_E);
                        //print("E Cell");
                    }
                }
                else
                {
                    UpdatingSequence.Add(Asigs_E);
                    //print("E Cell");
                }
                break;
            case static_cast<int32>(Asigs_F):
                //check and see if we are still in the previous cell. If so, we don't need to record it again.
                if (UpdatingSequence.Num() > 0)
                {
                    if (UpdatingSequence.Last() == Asigs_F)
                    {
                        //print("Still in cell");
                    }
                    else
                    {
                        UpdatingSequence.Add(Asigs_F);
                        //print("F Cell");
                    }
                }
                else
                {
                    UpdatingSequence.Add(Asigs_F);
                    //print("F Cell");
                }
                break;
            case static_cast<int32>(Asigs_G):
                //check and see if we are still in the previous cell. If so, we don't need to record it again.
                if (UpdatingSequence.Num() > 0)
                {
                    if (UpdatingSequence.Last() == Asigs_G)
                    {
                        //print("Still in cell");
                    }
                    else
                    {
                        UpdatingSequence.Add(Asigs_G);
                        //print("G Cell");
                    }
                }
                else
                {
                    UpdatingSequence.Add(Asigs_G);
                    //print("G Cell");
                }
                break;
            case static_cast<int32>(Asigs_H):
                //check and see if we are still in the previous cell. If so, we don't need to record it again.
                if (UpdatingSequence.Num() > 0)
                {
                    if (UpdatingSequence.Last() == Asigs_H)
                    {
                        //print("Still in cell");
                    }
                    else
                    {
                        UpdatingSequence.Add(Asigs_H);
                        //print("H Cell");
                    }
                }
                else
                {
                    UpdatingSequence.Add(Asigs_H);
                    //print("H Cell");
                }
                break;
            case static_cast<int32>(Asigs_I):
                //check and see if we are still in the previous cell. If so, we don't need to record it again.
                if (UpdatingSequence.Num() > 0)
                {
                    if (UpdatingSequence.Last() == Asigs_I)
                    {
                        //print("Still in cell");
                    }
                    else
                    {
                        UpdatingSequence.Add(Asigs_I);
                        //print("I Cell");
                    }
                }
                else
                {
                    UpdatingSequence.Add(Asigs_I);
                    //print("I Cell");
                }
                break;
            default:
                PRINT("default");
                break;
            }
        }


        //flip direction
        //	use second cells direction based from the middle to judge positive or negative first movement
        //	
        //	(-)A	(+)B	(+)C	
        //	(-)D	(N)E	(+)F
        //	(-)G	(-)H	(+)I

        if (UpdatingSequence.Num() == 2)
        {
            const int32 SecondCell = static_cast<int32>(UpdatingSequence[1]);

            if (SecondCell == Asigs_A || SecondCell == Asigs_D || SecondCell == Asigs_G || SecondCell == Asigs_H)
            {
                bIsNegative = true;
            }
        }
    }
}

void APlayerCharacter::DamagePlayer(const float Damage)
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

void APlayerCharacter::RegenerateFocus()
{
    if (CurrentFocus < MaxFocus)
        CurrentFocus += FocusRegenSpeed * GetWorld()->GetDeltaSeconds();
    else if (CurrentFocus > MaxFocus)
        CurrentFocus = MaxFocus;
}

EAsigs_State APlayerCharacter::ConcatSequence()
{
	EAsigs_State Temp = Asigs_Empty;

	//go through and concat into one enum
	for (int i = 0; i < UpdatingSequence.Num(); ++i)
	{
		Temp = static_cast<EAsigs_State>(Temp | UpdatingSequence[i]); //i don't know why I had to typecast this
	}

	//if we are only a unique single direction then our stick is just being nothing up there
	if (Temp == Asigs_A || Temp == Asigs_B || Temp == Asigs_C || Temp == Asigs_D || Temp == Asigs_E || Temp == Asigs_F
		|| Temp == Asigs_H || Temp == Asigs_I)
	{
		Temp = Asigs_Empty;
	}

	//	!!!RETURN TO THIS!!!!!!
	// find a way to get opposite directions working, probably has to do with checking the
	if (bIsNegative)
	{
		Temp = static_cast<EAsigs_State>(Temp | Asigs_Flip);
	}

	//clear the Updating sequence
	UpdatingSequence.Empty();
	//UpdatingSequence.Add(ASIGS_e);

	bIsNegative = false; //reset isNegative

	//print("SEQUENCE BREAK");

	return Temp;
}

void APlayerCharacter::CastArcaneBoltSpell(AActor* Enemy)
{
    // Attempt to fire a projectile.
    if (ArcaneBoltSpellClass)
    {
        // Get the camera transform.
        FVector CameraLocation;
        FRotator CameraRotation;
        GetActorEyesViewPoint(CameraLocation, CameraRotation);

        // Transform MuzzleOffset from camera space to world space.
        const FVector CastLocation = CameraLocation + FTransform(CameraRotation).TransformVector(CastOffset);
        FRotator CastRotation = CameraRotation;

        //add a little bit of a start pitch and yaw to give a small bit of arc
        CastRotation.Pitch = FMath::RandRange(0.0f, 3.0f);
        CastRotation.Yaw = FMath::RandRange(0.0f, 3.0f);

        UWorld* World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            AArcaneBolt* Projectile = World->SpawnActor<AArcaneBolt>(ArcaneBoltSpellClass, CastLocation, CastRotation, SpawnParams);
            if (Projectile)
            {
                // Set the projectile's initial trajectory.
                const FVector LaunchDirection = CastRotation.Vector();
                Projectile->CastInDirection(LaunchDirection, Enemy);
                SetCastingStatus(true);
            }
        }
    }
}

void APlayerCharacter::CastIceKnifeSpell()
{
    // Attempt to fire a projectile.
    if (IKSpellClass)
    {
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
            AIceKnife* Projectile = World->SpawnActor<AIceKnife>(IKSpellClass, CastLocation, CastRotation, SpawnParams);
            if (Projectile)
            {
                // Set the projectile's initial trajectory.
                const FVector LaunchDirection = CastRotation.Vector();
                Projectile->CastInDirection(LaunchDirection);
                SetCastingStatus(true);
            }
        }
    }
}

void APlayerCharacter::CastIceKnifeTwoSpell()
{
	// Attempt to fire a projectile.
	if (IceKnifeTwoSpellClass)
	{
		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Transform MuzzleOffset from camera space to world space.
        const FVector CastLocation = CameraLocation + FTransform(CameraRotation).TransformVector(CastOffset);
		FRotator CastRotation = CameraRotation;
		// Skew the aim to be slightly upwards.
		CastRotation.Pitch += 10.0f;
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			// Spawn the projectile at the muzzle.
			AIceKnifeTwo* Projectile = World->SpawnActor<AIceKnifeTwo>(IceKnifeTwoSpellClass, CastLocation, CastRotation, SpawnParams);
			if (Projectile)
			{
				// Set the projectile's initial trajectory.
                const FVector LaunchDirection = CastRotation.Vector();
				Projectile->CastInDirection(LaunchDirection);
			    SetCastingStatus(true);
			}
		}
	}
}

void APlayerCharacter::CastIceKnifeVarTwoSpell()
{
	// Attempt to fire a projectile.
	if (IceKnifeVarTwoSpellClass)
	{
		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Transform MuzzleOffset from camera space to world space.
        const FVector CastLocation = CameraLocation + FTransform(CameraRotation).TransformVector(CastOffset);
		FRotator CastRotation = CameraRotation;
		// Skew the aim to be slightly upwards.
		CastRotation.Pitch += 10.0f;
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			// Spawn the projectile at the muzzle.
			AIceKnifeTwoVarTwo* Projectile = World->SpawnActor<AIceKnifeTwoVarTwo>(IceKnifeVarTwoSpellClass, CastLocation, CastRotation, SpawnParams);
			if (Projectile)
			{
				// Set the projectile's initial trajectory.
                const FVector LaunchDirection = CastRotation.Vector();
				Projectile->CastInDirection(LaunchDirection);
			    SetCastingStatus(true);
			}
		}
	}
}

void APlayerCharacter::SetCastingStatus(const bool B)
{
    PlayerStatus.bIsCasting = B;
    
    GetWorldTimerManager().SetTimer(
        CastingTimerHandle, this, &APlayerCharacter::EndCastingStatus, PlayerAttackingTimerLength, false);
}

void APlayerCharacter::EndCastingStatus()
{
    PlayerStatus.bIsCasting = false;
}

void APlayerCharacter::Cast()
{
	if (OutputSequence == Asigs_Ebeh)
	{
        PRINT("FIRING ICE KNIFE BLUE");
		CastIceKnifeTwoSpell();
	}
	if (OutputSequence == Asigs_Eheb)
	{
        PRINT("FIRING ICE KNIFE VAR 2 GREEN");
		CastIceKnifeVarTwoSpell();
	}
	if (OutputSequence == Asigs_Efed)
	{
        PRINT("FIRING TRUE ICE KNIFE");
		CastIceKnifeSpell();
	}
	if (OutputSequence == Asigs_Edef)
	{
		PRINT("FIRING ARCANE BOLT");
        CastArcaneBoltSpell(EnemyActor);
	}
}


