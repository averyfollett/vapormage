// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "IceKnife.generated.h"

UCLASS()
class GRILLEDPIGEON_API AIceKnife final : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	AIceKnife();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CastInDirection(const FVector& ShootDirection) const;

	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = LockOnSystem)
		ACharacter* EnemyActor;*/

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement)
	float TravelSpeed = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float Damage = 20.0f;

};
