// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UInputComponent;

UCLASS(config = Game)
class GRILLEDPIGEON_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/**
	 * Switch to next enemy in EnemyArray (looping)
	 */
	void NextEnemy();

	/**
	 * Switch to previous enemy in EnemyArray (looping)
	 */
	void PreviousEnemy();

	/**
	 * Capsule trace from the camera position forward looking for other pawns
	 * Returns the hit AActor
	 */
	UFUNCTION(BlueprintCallable)
		AActor * CapsuleTraceForEnemy();

	UFUNCTION(BlueprintCallable)
		void AutoAimAtEnemy(AActor * enemy);


protected:
	/**
	 * Whether or not the player's crosshair is locked on to an enemy
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LockOnSystem)
		bool IsLockedOn = false;

	/**
	 * The enemy actor to be locked on to
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LockOnSystem)
		AActor* EnemyActor;

	/**
	 * Array of all enemy actors
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LockOnSystem)
		TArray<AActor*> EnemyArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LockOnSystem)
		int CurrentEnemyIndex = 0;

	/**
	 * Whether or not the player's wand is raised (for casting a spell)
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Wand)
		bool IsWandRaised = false;

	/**
	 * Whether or not the player's wand is in a defensive position
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Wand)
		bool IsWandDefense = false;

	/**
	 * The transform location of the tip of the wand.
	 * Used for spawning spells
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Wand)
		FTransform WandTipPosition;

	/**
	 * Whether or not the player's shield is activated
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Shield)
		bool IsShieldActive = false;


public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

