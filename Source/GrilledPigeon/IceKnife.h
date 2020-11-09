// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
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

	UFUNCTION(BlueprintCallable, Category=Sound)
	void PlayRandomCastSound();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category=Cast)
	void CastInDirection(const FVector& ShootDirection);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = LockOnSystem)
		ACharacter* EnemyActor;*/

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement)
	float TravelSpeed = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float Damage = 20.0f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Sound)
	TArray<USoundCue*> CastSoundArray;
};
