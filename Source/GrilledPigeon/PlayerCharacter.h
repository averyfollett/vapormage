// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Templates/Tuple.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

/*
	flagged enum with bit operations, allows for combination of enums to create new enums
*/
UENUM(BlueprintType)
enum ASIGS_STATE	
{
	ASIGS_default = -1,
	ASIGS_empty = 0,	//for temp usage
	ASIGS_a = 1,		//singular cells
	ASIGS_b = 2,
	ASIGS_c = 4,
	ASIGS_d = 8,
	ASIGS_e = 16,		//nuetral state
	ASIGS_f = 32,
	ASIGS_g = 64,
	ASIGS_h = 128,
	ASIGS_i = 256,
	ASIGS_FLIP = 512,	//THIS NEEDS TO EXIST, its to tell for opposite directions, PUT AT BACK that way we can run if statement 
						//		on the sequence to see if ASIGS_FLIP exists in this call, then we know difference between "beh" and "heb"
						//			To assign said difference, 
	ASIGS_ebeh = ASIGS_e | ASIGS_b | ASIGS_e | ASIGS_h,							//up -> down
	ASIGS_eheb = ASIGS_e | ASIGS_h | ASIGS_e | ASIGS_b | ASIGS_FLIP,				//down -> up
	ASIGS_edef = ASIGS_e | ASIGS_d | ASIGS_e | ASIGS_f,							//left -> right
	ASIGS_efed = ASIGS_e | ASIGS_f | ASIGS_e | ASIGS_d | ASIGS_FLIP,				//right -> left
	ASIGS_eaei = ASIGS_e | ASIGS_a | ASIGS_e | ASIGS_i,							//diagnonal, top left -> bottom right
	ASIGS_eiea = ASIGS_e | ASIGS_i | ASIGS_e | ASIGS_a | ASIGS_FLIP,				//diagonal, bottom right -> top left
	ASIGS_eceg = ASIGS_e | ASIGS_c | ASIGS_e | ASIGS_g,							//diagonal, top right -> bottom left
	ASIGS_egec = ASIGS_e | ASIGS_g | ASIGS_e | ASIGS_c | ASIGS_FLIP,				//diagonal, bottom left -> top right
	ASIGS_eabc = ASIGS_e | ASIGS_a | ASIGS_b | ASIGS_c,							//swoop, top left -> top right
	ASIGS_ecba = ASIGS_e | ASIGS_c | ASIGS_b | ASIGS_a | ASIGS_FLIP,				//swoop, top right -> top left
	ASIGS_eghi = ASIGS_e | ASIGS_g | ASIGS_h | ASIGS_i,							//swoop, bottom left -> bottom right
	ASIGS_eihg = ASIGS_e | ASIGS_i | ASIGS_h | ASIGS_g | ASIGS_FLIP,				//swoop, bottom right -> bottom left
	ASIGS_eadg = ASIGS_e | ASIGS_a | ASIGS_d | ASIGS_g,							//swoop, top left -> bottom left
	ASIGS_egda = ASIGS_e | ASIGS_g | ASIGS_d | ASIGS_a | ASIGS_FLIP,				//swoop, bottom left -> top left
	ASIGS_ecfi = ASIGS_e | ASIGS_c | ASIGS_f | ASIGS_i,							//swoop, top right -> bottom right
	ASIGS_eifc = ASIGS_e | ASIGS_i | ASIGS_f | ASIGS_c | ASIGS_FLIP				//swoop, bottom right -> top right

};

USTRUCT(BlueprintType)
struct Fcoord_cell
{
	// m00  m01
	// m10  m11
	GENERATED_USTRUCT_BODY();

	Fcoord_cell()
	{
		m00.Key = -1;
		m01.Key = 1;
		m10.Key = -1;
		m11.Key = 1;

		m00.Value = 1;
		m01.Value = 1;
		m10.Value = -1;
		m11.Value = -1;
	}

	Fcoord_cell(double _m00x, double _m00y, double _m01x, double _m01y, double _m10x, double _m10y, double _m11x, double _m11y)
	{
		m00.Key = _m00x;
		m01.Key = _m01x;
		m10.Key = _m10x;
		m11.Key = _m11x;

		m00.Value = _m00y;
		m01.Value = _m01y;
		m10.Value = _m10y;
		m11.Value = _m11y;
	}

	Fcoord_cell(TTuple<double, double> _m00, TTuple<double, double> _m01, TTuple<double, double> _m10, TTuple<double, double> _m11)
	{
		m00 = _m00;
		m01 = _m01;
		m10 = _m10;
		m11 = _m11;
	}


	TTuple<double, double> m00, m01, m10, m11;
};

USTRUCT(BlueprintType)
struct FASIGS_cell
{
	GENERATED_USTRUCT_BODY();

	FASIGS_cell()
	{
		mCellNum = ASIGS_e;
		init();
	}

	FASIGS_cell(ASIGS_STATE cell)
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
				mCellCords = Fcoord_cell(-1.0, 1.0, -0.33, 1.0, -1.0, 0.33, -0.33, 0.33);
				break;
			case ASIGS_b:
				mCellCords = Fcoord_cell(-0.33, 1.0, 0.33, 1.0, -0.33, 0.33, 0.33, 0.33);
				break;
			case ASIGS_c:
				mCellCords = Fcoord_cell(0.33, 1.0, 1.0, 1.0, 0.33, 0.33, 1.0, 0.33);
				break;
			case ASIGS_d:
				mCellCords = Fcoord_cell(-1.0, 0.33, -0.33, 0.33, -1.0, -0.33, -0.33, -0.33);
				break;
			case ASIGS_e:
				mCellCords = Fcoord_cell(-0.33, 0.33, 0.33, 0.33, -0.33, -0.33, 0.33, -0.33);
				break;
			case ASIGS_f:
				mCellCords = Fcoord_cell(0.33, 0.33, 1.0, 0.33, 0.33, -0.33, 1.0, -0.33);
				break;
			case ASIGS_g:
				mCellCords = Fcoord_cell(-1.0, -0.33, -0.33, -0.33, -1.0, -1.0, -0.33, -1.0);
				break;
			case ASIGS_h:
				mCellCords = Fcoord_cell(-0.33, -0.33, 0.33, -0.33, -0.33, -1.0, 0.33, -1.0);
				break;
			case ASIGS_i:
				mCellCords = Fcoord_cell(0.33, -0.33, 1.0, -0.33, 0.33, -1.0, 1.0, -1.0);
				break;
			default:
				mCellCords = Fcoord_cell(-0.33, 0.33, 0.33, 0.33, -0.33, -0.33, 0.33, -0.33);
				break;
		}
	} 
	
	ASIGS_STATE mCellNum;
	Fcoord_cell mCellCords;
};

USTRUCT(BlueprintType)
struct FASIGS_grid3x3
{
	GENERATED_USTRUCT_BODY();

	FASIGS_grid3x3()
	{
		mGrid.Add(FASIGS_cell(ASIGS_a));
		mGrid.Add(FASIGS_cell(ASIGS_b));
		mGrid.Add(FASIGS_cell(ASIGS_c));
		mGrid.Add(FASIGS_cell(ASIGS_d));
		mGrid.Add(FASIGS_cell(ASIGS_e));
		mGrid.Add(FASIGS_cell(ASIGS_f));
		mGrid.Add(FASIGS_cell(ASIGS_g));
		mGrid.Add(FASIGS_cell(ASIGS_h));
		mGrid.Add(FASIGS_cell(ASIGS_i));
	}

	TArray<FASIGS_cell> mGrid;
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

	/*
		combine the sequence list into a single enum value and return it
	*/
	ASIGS_STATE ConcatSequence();


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
		AActor* EnemyActor;

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
		3x3 Grid system for unit circle based analog stick input
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASIGS)
		FASIGS_grid3x3 ASIGS;

	/*
		range to judge a considerable amount of intended input
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASIGS)
		float IBR = 0.000001f;

	/*
		timer system to delay the concatonation stage
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASIGS)
		int delayTimerMax = 20;	//If at 10, wait for 1/6 of a second before going into concat stage when fps is locked at 60

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASIGS)
		int delayTimerCurrent;

	/*
		current single element sequence of enums, later to be combined to create a unique enum 
		to output to editor (possible swoop or diagonal creation)
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASIGS)
		TArray<int32> UpdatingSequence;

	/*
		prior frames input for pitch and yaw analog stick axis
	*/
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASIGS)
		TTuple<float, float> PreviousInput;
	

	/*
		Value determined by lock on state, if locked on and ready to start casting we are true. default is true.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASIGS)
		bool GatheringSequence = true;

	/*
		final return sequence enum signifier to grab from the editor
		IN EDITOR BLUEPRINTS EXAMPLE:

			if outputsequence == ASIGS_cfi then do the swoop spell
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASIGS)
		int32 OutputSequence;

	/*
		For telling wether we are goin in the negative direction or the positive direction
			if the FIRST MOVE goes in the negative direction, allow for the appendage of the negative enum
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASIGS)
		bool isNegative = false;


public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

