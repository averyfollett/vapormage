// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Math/Vector.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "ArcaneBolt.generated.h"

UCLASS()
class GRILLEDPIGEON_API AArcaneBolt final : public AActor
{
	GENERATED_BODY()

	//UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	//class UStaticMeshComponent* sMesh;
	
public:	
	// Sets default values for this actor's properties
	AArcaneBolt();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category=Spell)
	void CastInDirection(const FVector& ShootDirection) const;

	UFUNCTION(BlueprintCallable, Category=Spell)
	void TargetHoming();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	/*
		target locked in direction to do homing in tick
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = LockOnSystem)
		ACharacter* EnemyActor;

	/*
		Location of enemy actor
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = LockOnSystem)
		FVector EnemyActorLocation;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float InitialTravelSpeed = 4000.0f;

	/*
		speed and tracking strength
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float TrackingTravelSpeed = 15000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float LifeSpan = 6.0f;
};
