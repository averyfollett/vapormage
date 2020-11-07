// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "EnemyCharacter.generated.h"

USTRUCT(BlueprintType)
struct FEnemyStatus
{
	GENERATED_USTRUCT_BODY()
    
    //Contains all player status stuff
    bool bIsCasting;
	bool bIsBlocking;
};

UCLASS()
class GRILLEDPIGEON_API AEnemyCharacter final : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	//Spell to be cast
	UFUNCTION(BlueprintCallable, Category=Cast)
	void CastIceKnifeSpell();

	//Spell to be cast
	UFUNCTION(BlueprintCallable, Category=Cast)
	void CastSparkSpell();

	//Spell to be cast
	UFUNCTION(BlueprintCallable, Category=Cast)
	void CastAshBoltSpell();

	//Spell to be cast
	UFUNCTION(BlueprintCallable, Category=Cast)
	void CastFlamePoolSpell();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	* Run each tick to regenerate enemy's focus up to max based on focus regen speed
	* Also clamps max current focus to max focus
	*/
	UFUNCTION(BlueprintCallable, Category=Health)
    void RegenerateFocus();

	UFUNCTION(BlueprintCallable, Category=Blocking)
	void EndBlockingStatus();

	UFUNCTION(BlueprintCallable, Category=Blocking)
	void ShowBlockIndicator(int Direction) const;
	UFUNCTION(BlueprintCallable, Category=Health)
	void DelayBeforeRegen();

	UFUNCTION(BlueprintCallable, Category=Health)
	void SetRegenEnabled();
	
protected:
	/*
	* Maximum amount of focus the enemy can have at any given time
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float MaxFocus = 100;

	/*
	* Current value of focus (0 - MaxFocus)
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
	float CurrentFocus = 100;

	/*
	* Maximum amount of vitality points the player can have at any given time
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	int MaxVitality = 3;

	/*
	* Current value of vitality points (0 - MaxVitality)
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health)
	int CurrentVitality = 3;

	/*
	* The speed (focus/sec) at which CurrentFocus is regenerated
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float FocusRegenSpeed = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float VitalityLossThreshold = 0.1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Enemy)
	class APlayerCharacter * EnemyCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CASTING)
	FVector CastOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CASTING)
	TSubclassOf<class AIceKnife> IceKnifeSpellClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CASTING)
	TSubclassOf<class AIceKnife> SparkSpellClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CASTING)
	TSubclassOf<class AIceKnife> AshBoltSpellClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float IKDamageThreshold = 15;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float ArcaneDamageThreshold = 13;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float GridPulseDamageThreshold = 7;

	FTimerHandle BlockingTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Blocking)
	float BlockingTimerLength = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FocusCosts)
	float FlamePoolFocusCost = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FocusCosts)
	float SparkFocusCost = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FocusCosts)
	float AshBoltFocusCost = 30;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Health)
	bool bCanRegenFocus = true;

	FTimerHandle RegenDelayTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health)
	float RegenDelayLength = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Health)
	bool bCanTakeDamage = true;

public:
	UPROPERTY(EditAnywhere, Category=Behaviour)
	class UBehaviorTree * BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	FEnemyStatus EnemyStatus;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
    void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// Function to run when blocking
	UFUNCTION(BlueprintCallable, Category=Combat)
	void BlockSpell();

	UFUNCTION(BlueprintCallable, Category=Health)
	void DamageAI(float Damage, bool bWasBlocked);
};
