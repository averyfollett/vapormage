// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Math/Vector.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GridPulse.generated.h"



UCLASS()
class GRILLEDPIGEON_API AGridPulse : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridPulse();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void CastInDirection(const FVector& ShootDirection);

	/*
		target locked in direction to do pushback onto the target
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LockOnSystem)
		ACharacter* EnemyActor;

	/*
		player for vitality damage
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LockOnSystem)
		ACharacter* PlayerActor;


	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComponent;


	UPROPERTY(VisibleAnywhere, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float InitialTravelSpeed = 7500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float ImpactForce = 200.0f;

};
