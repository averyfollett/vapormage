// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "IceKnifeTwo.generated.h"

UCLASS()
class GRILLEDPIGEON_API AIceKnifeTwo : public AActor
{
	GENERATED_BODY()
	
	//https://docs.unrealengine.com/en-US/Programming/Tutorials/FirstPersonShooter/3/1/index.html
	
public:	
	// Sets default values for this actor's properties
	AIceKnifeTwo();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComponent;


	UPROPERTY(VisibleAnywhere, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;


	void CastInDirection(const FVector& ShootDirection);
};
