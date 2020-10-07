// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

#include <utility>
#include <vector>

enum ASIGS_STATE
{
	ASIGS_a = 0,	//singular cells
	ASIGS_b,
	ASIGS_c,
	ASIGS_d,
	ASIGS_e,		//nuetral state
	ASIGS_f,
	ASIGS_g,
	ASIGS_h,
	ASIGS_i,
	ASIGS_beh,	//up -> down
	ASIGS_heb,	//down -> up
	ASIGS_def,	//left -> right
	ASIGS_fed,	//right -> left
	ASIGS_aei,	//diagnonal, top left -> bottom right
	ASIGS_iea,	//diagonal, bottom right -> top left
	ASIGS_ceg,	//diagonal, top right -> bottom left
	ASIGS_gec,	//diagonal, bottom left -> top right
	ASIGS_abc,	//swoop, top left -> top right
	ASIGS_cba,	//swoop, top right -> top left
	ASIGS_ghi,	//swoop, bottom left -> bottom right
	ASIGS_ihg,	//swoop, bottom right -> bottom left
	ASIGS_adg,	//swoop, top left -> bottom left
	ASIGS_gda,	//swoop, bottom left -> top left
	ASIGS_cfi,	//swoop, top right -> bottom right
	ASIGS_ifc	//swoop, bottom right -> top right
};

struct coord_cell
{
	// m00  m01
	// m10  m11

	coord_cell()
	{
		m00.first = -1;
		m01.first = 1;
		m10.first = -1;
		m11.first = 1;

		m00.second = 1;
		m01.second = 1;
		m10.second = -1;
		m11.second = -1;
	}

	coord_cell(double _m00x, double _m00y, double _m01x, double _m01y, double _m10x, double _m10y, double _m11x, double _m11y)
	{
		m00.first = _m00x;
		m01.first = _m01x;
		m10.first = _m10x;
		m11.first = _m11x;

		m00.second = _m00y;
		m01.second = _m01y;
		m10.second = _m10y;
		m11.second = _m11y;
	}

	coord_cell(std::pair<double, double> _m00, std::pair<double, double> _m01, std::pair<double, double> _m10, std::pair<double, double> _m11)
	{
		m00 = _m00;
		m01 = _m01;
		m10 = _m10;
		m11 = _m11;
	}

	std::pair<double, double> m00, m01, m10, m11;
};

struct ASIGS_cell
{
	ASIGS_cell(ASIGS_STATE cell)
	{
		mCellNum = cell;
		init();
	}

	void init()
	{

		//current system sets grid cells to be set on unit circle
		switch (mCellNum)
		{
			case ASIGS_a:
				coord_cell temp(-1.0, 1.0, -0.33, 1.0, -1.0, 0.33, -0.33, 0.33);
				mCellCords = temp;
				break;
			case ASIGS_b:
				coord_cell temp(-0.33, 1.0, 0.33, 1.0, -0.33, 0.33, 0.33, 0.33);
				mCellCords = temp;
				break;
			case ASIGS_c:
				coord_cell temp(0.33, 1.0, 1.0, 1.0, 0.33, 0.33, 1.0, 0.33);
				mCellCords = temp;
				break;
			case ASIGS_d:
				coord_cell temp(-1.0, 0.33, -0.33, 0.33, -1.0, -0.33, -0.33, -0.33);
				mCellCords = temp;
				break;
			case ASIGS_e:
				coord_cell temp(-0.33, 0.33, 0.33, 0.33, -0.33, -0.33, 0.33, -0.33);
				mCellCords = temp;
				break;
			case ASIGS_f:
				coord_cell temp(0.33, 0.33, 1.0, 0.33, 0.33, -0.33, 1.0, -0.33);
				mCellCords = temp;
				break;
			case ASIGS_g:
				coord_cell temp(-1.0, -0.33, -0.33, -0.33, -1.0, -1.0, -0.33, -1.0);
				mCellCords = temp;
				break;
			case ASIGS_h:
				coord_cell temp(-0.33, -0.33, 0.33, -0.33, -0.33, -1.0, 0.33, -1.0);
				mCellCords = temp;
				break;
			case ASIGS_i:
				coord_cell temp(0.33, -0.33, 1.0, -0.33, 0.33, -1.0, 1.0, -1.0);
				mCellCords = temp;
				break;
		}
	} 
	
	ASIGS_STATE mCellNum;
	coord_cell mCellCords;
};

struct ASIGS_grid3x3
{
	ASIGS_grid3x3()
	{
		mGrid.push_back(ASIGS_cell(ASIGS_a));
		mGrid.push_back(ASIGS_cell(ASIGS_b));
		mGrid.push_back(ASIGS_cell(ASIGS_c));
		mGrid.push_back(ASIGS_cell(ASIGS_d));
		mGrid.push_back(ASIGS_cell(ASIGS_e));
		mGrid.push_back(ASIGS_cell(ASIGS_f));
		mGrid.push_back(ASIGS_cell(ASIGS_g));
		mGrid.push_back(ASIGS_cell(ASIGS_h));
		mGrid.push_back(ASIGS_cell(ASIGS_i));
	}

	std::vector<ASIGS_cell> mGrid;
};

class UInputComponent;

UCLASS(config = Game)
class GRILLEDPIGEON_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/**
	 * Switch to next enemy in EnemyArray (looping)
	 */
	void NextEnemy();

	/**
	 * Switch to previous enemy in EnemyArray (looping)
	 */
	void PreviousEnemy();

	/**
	 * Capsule trace from the camera position forward looking for other pawns
	 * Returns the hit AActor
	 */
	UFUNCTION(BlueprintCallable)
		AActor * CapsuleTraceForEnemy();

	UFUNCTION(BlueprintCallable)
		void AutoAimAtEnemy(AActor * enemy);


	/*
		take in analog stick axis value, determine cell coord, add to sequence vector
	*/
	void NextInSequence(float xAxis, float yAxis);

	/*
		return true if given the buffer range that the analog stick has moved "fast enough" for input (its out of the range)
		uses simple 2d circle point collision. Uses the stored previous value with radius of inputBuffer
	*/
	bool bufferRangeCheck(float inputBufferRadius, float currentAnalogPosX, float currentAnalogPosY);

	/*
		determine sequence out from vector, call where input is located
	*/
	void SequenceOut(float xAxis, float yAxis, float inputBufferRadius);


protected:
	/**
	 * Whether or not the player's crosshair is locked on to an enemy
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LockOnSystem)
		bool IsLockedOn = false;

	/**
	 * The enemy actor to be locked on to
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LockOnSystem)
		AActor * EnemyActor;

	/**
	 * Array of all enemy actors
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LockOnSystem)
		TArray<AActor*> EnemyArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LockOnSystem)
		int CurrentEnemyIndex = 0;

	/**
	 * Whether or not the player's wand is raised (for casting a spell)
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Wand)
		bool IsWandRaised = false;

	/**
	 * Whether or not the player's wand is in a defensive position
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Wand)
		bool IsWandDefense = false;

	/**
	 * The transform location of the tip of the wand.
	 * Used for spawning spells
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Wand)
		FTransform WandTipPosition;

	/**
	 * Whether or not the player's shield is activated
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Shield)
		bool IsShieldActive = false;

	/*
		
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ASIGS_grid3x3 ASIGS;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		std::vector<ASIGS_STATE> Sequence;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		std::pair<float, float> PreviousInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool GatheringSequence = false;


public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

