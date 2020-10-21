// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Math/Vector.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "ArcaneBolt.generated.h"

UCLASS()
class GRILLEDPIGEON_API AArcaneBolt : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class UStaticMeshComponent* sMesh0;
	
public:	
	// Sets default values for this actor's properties
	AArcaneBolt();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CastInDirection(const FVector& ShootDirection, AActor* Enemy);

	void TargetHoming();

	//void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	/*
		target locked in direction to do homing in tick
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = LockOnSystem)
		ACharacter* EnemyActor;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComponent;


	UPROPERTY(VisibleAnywhere, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float InitialTravelSpeed = 7500.0f;

	/*
		speed and tracking strength
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float TrackingTravelSpeed = 300.0f;

};
