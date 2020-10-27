// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"


UCLASS()
class GRILLEDPIGEON_API AEnemyCharacter final : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	//Spell to be cast
	void CastIceKnifeSpell();

	//Spell to be cast
	void CastSparkSpell();

	//Spell to be cast
	void CastAshBoltSpell();

	//Spell to be cast
	void CastFlamePoolSpell();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	/*
	* Maximum amount of focus the player can have at any given time
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	float MaxFocus = 100;

	/*
	* Current value of focus (0 - MaxFocus)
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	float CurrentFocus = 20;

	/*
	* Maximum amount of vitality points the player can have at any given time
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	int MaxVitality = 3;

	/*
	* Current value of vitality points (0 - MaxVitality)
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	int CurrentVitality = 3;

	/*
	* The speed (focus/sec) at which CurrentFocus is regenerated
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	float FocusRegenSpeed = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
	float VitalityLossThreshold = 0.1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Enemy)
	class APlayerCharacter * EnemyCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CASTING)
	FVector CastOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CASTING)
	TSubclassOf<class AIceKnife> IceKnifeSpellClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CASTING)
	TSubclassOf<class AIceKnife> SparkSpellClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CASTING)
	TSubclassOf<class AIceKnife> AshBoltSpellClass;

public:
	UPROPERTY(EditAnywhere, Category=Behaviour)
	class UBehaviorTree * BehaviorTree;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	//Spell to be cast
	UFUNCTION(Category=Combat)
	void CastIceKnifeSpell() const;

	// Function to run when blocking
	UFUNCTION(Category=Combat)
	void BlockSpell();

	void RegenerateFocus();

	UFUNCTION(BlueprintCallable)
	void DamageAI(float Damage);


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
		float IKDamageThreshold = 15;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
		float ArcaneDamageThreshold = 13;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health)
		float GridPulseDamageThreshold = 7;
};
