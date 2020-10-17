// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "IceKnife.generated.h"

UCLASS()
class GRILLEDPIGEON_API AIceKnife : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* Mesh0P;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* Mesh1P;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* Mesh2P;
	
public:	
	// Sets default values for this actor's properties
	AIceKnife();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CastInDirection(const FVector& ShootDirection);

	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComponent;


	UPROPERTY(VisibleAnywhere, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;

	
	float travelSpeed = 10000.0f;

};
