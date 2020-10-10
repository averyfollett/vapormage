// Fill out your copyright notice in the Description page of Project Settings.

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

#include "PlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

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
	print("Enemy: " + EnemyActor->GetName());
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SequenceOut(this->InputComponent->GetAxisValue(TEXT("LookUpRate")), this->InputComponent->GetAxisValue(TEXT("TurnRate")), IBR);

	AutoAimAtEnemy(EnemyActor);
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
	print("Next Enemy: " + EnemyActor->GetName() + "Index " + FString::FromInt(CurrentEnemyIndex));
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
	print("Prev Enemy: " + EnemyActor->GetName() + "Index " + FString::FromInt(CurrentEnemyIndex));
}

AActor * APlayerCharacter::CapsuleTraceForEnemy()
{
	FHitResult hitActor;
	FVector startPos = FirstPersonCameraComponent->GetComponentLocation();
	FVector endPos = FirstPersonCameraComponent->GetComponentLocation() + (FirstPersonCameraComponent->GetForwardVector() * 1500.0f);
	FCollisionShape capsule = FCollisionShape::MakeCapsule(FVector(100.0f, 100.0f, 100.0f));
	const FName traceTag("LockOnTrace");
	GetWorld()->DebugDrawTraceTag = traceTag;
	FCollisionQueryParams collisionParams;
	collisionParams.TraceTag = traceTag;

	GetWorld()->SweepSingleByObjectType(hitActor, startPos, endPos, FQuat(), ECC_Pawn, capsule, collisionParams);

	return hitActor.GetActor();
}

void APlayerCharacter::AutoAimAtEnemy(AActor* enemy)
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation
		(FirstPersonCameraComponent->GetComponentLocation(), enemy->GetActorLocation());

	FRotator newRotation = FMath::RInterpTo
		(FirstPersonCameraComponent->GetComponentRotation(), targetRotation, GetWorld()->GetDeltaSeconds(), 20.0f);
	
	GetController()->SetControlRotation(newRotation);

}


void APlayerCharacter::SequenceOut(float xAxis, float yAxis, float inputBufferRadius)
{
	//dont bother doing this if we dont care about gathering the input, save on some input load times
	if (GatheringSequence)
	{
		//check for significant movement in the stick
		if (bufferRangeCheck(inputBufferRadius, xAxis, yAxis) == false)
		{
			//no collision, we have stick movement, record the values
			NextInSequence(xAxis, yAxis);
		}
		else
		{
			//combine and release whatever sequence we have
			//	even if we are constantly gathering, but failing the buffer range check, its okay to release a blank sequence
			OutputSequence = ConcatSequence();

			//DEBUG
			int os = (int)OutputSequence;
			print(FString::FromInt(os));
		}
	}

	//make sure this is at very end for next update tick
	PreviousInput.Key = xAxis;
	PreviousInput.Value = yAxis;
}

bool APlayerCharacter::bufferRangeCheck(float inputBufferRadius, float currentAnalogPosX, float currentAnalogPosY)
{
	float xDist = currentAnalogPosX - PreviousInput.Key;
	float yDist = currentAnalogPosY - PreviousInput.Value;
	float distance = sqrt((xDist * xDist) + (yDist * yDist));

	if (distance <= inputBufferRadius)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void APlayerCharacter::NextInSequence(float xAxis, float yAxis)
{
	for (int i = 0; i < ASIGS.mGrid.Num() - 1; ++i)
	{
		Fcoord_cell rect = ASIGS.mGrid[i].mCellCords;

		//basic point box collision detection
		if (xAxis >= rect.m00.Key && xAxis <= rect.m01.Key &&
			yAxis >= rect.m00.Value && yAxis <= rect.m10.Value)
		{
			switch (ASIGS.mGrid[i].mCellNum)
			{
			case ASIGS_a:
				//check and see if we are still in the previous cell. If so, we dont need to record it again.
				if (UpdatingSequence.Last() != ASIGS_a)
				{
					UpdatingSequence.Add(ASIGS_a);
				}
				break;
			case ASIGS_b:
				//check and see if we are still in the previous cell. If so, we dont need to record it again.
				if (UpdatingSequence.Last() != ASIGS_b)
				{
					UpdatingSequence.Add(ASIGS_b);
				}
				break;
			case ASIGS_c:
				//check and see if we are still in the previous cell. If so, we dont need to record it again.
				if (UpdatingSequence.Last() != ASIGS_c)
				{
					UpdatingSequence.Add(ASIGS_c);
				}
				break;
			case ASIGS_d:
				//check and see if we are still in the previous cell. If so, we dont need to record it again.
				if (UpdatingSequence.Last() != ASIGS_d)
				{
					UpdatingSequence.Add(ASIGS_d);
				}
				break;
			case ASIGS_e:
				//check and see if we are still in the previous cell. If so, we dont need to record it again.
				if (UpdatingSequence.Last() != ASIGS_e)
				{
					UpdatingSequence.Add(ASIGS_e);
				}
				break;
			case ASIGS_f:
				//check and see if we are still in the previous cell. If so, we dont need to record it again.
				if (UpdatingSequence.Last() != ASIGS_f)
				{
					UpdatingSequence.Add(ASIGS_f);
				}
				break;
			case ASIGS_g:
				//check and see if we are still in the previous cell. If so, we dont need to record it again.
				if (UpdatingSequence.Last() != ASIGS_g)
				{
					UpdatingSequence.Add(ASIGS_g);
				}
				break;
			case ASIGS_h:
				//check and see if we are still in the previous cell. If so, we dont need to record it again.
				if (UpdatingSequence.Last() != ASIGS_h)
				{
					UpdatingSequence.Add(ASIGS_h);
				}
				break;
			case ASIGS_i:
				//check and see if we are still in the previous cell. If so, we dont need to record it again.
				if (UpdatingSequence.Last() != ASIGS_i)
				{
					UpdatingSequence.Add(ASIGS_i);
				}
				break;
			
			}
		}
	}
}

ASIGS_STATE APlayerCharacter::ConcatSequence()
{
	ASIGS_STATE temp = ASIGS_empty;

	//go through and concat into one enum
	for (int i = 0; i < UpdatingSequence.Num() - 1; ++i)
	{
		temp = (ASIGS_STATE)(temp | UpdatingSequence[i]);	//i dont know why I had to typecast this
	}

	//	!!!RETURN TO THIS!!!!!!
	// find a way to get opposite directions working, probably has to do with checking the


	//clear the Updating sequence
	UpdatingSequence.Empty();

	return temp;
}
