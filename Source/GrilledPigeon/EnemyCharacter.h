// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class GRILLEDPIGEON_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

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
	APlayerCharacter * EnemyCharacter;

public:
	UPROPERTY(EditAnywhere, Category=Behaviour)
	class UBehaviorTree * BehaviorTree;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Function to run when blocking
	UFUNCTION(CallInEditor, BlueprintCallable, Category=Combat)
	virtual void BlockSpell();

};
