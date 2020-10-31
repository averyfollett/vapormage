// Fill out your copyright notice in the Description page of Project Settings.

// ReSharper disable IdentifierTypo

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Templates/Tuple.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

/*
	flagged enum with bit operations, allows for combination of enums to create new enums
*/
UENUM(BlueprintType)
enum EAsigs_State
{
    Asigs_Default = -1,
    Asigs_Empty = 0,
    //for temp usage
    Asigs_A = 1,
    //singular cells
    Asigs_B = 2,
    Asigs_C = 4,
    Asigs_D = 8,
    Asigs_E = 16,
    //neutral state
    Asigs_F = 32,
    Asigs_G = 64,
    Asigs_H = 128,
    Asigs_I = 256,
    Asigs_Flip = 512,
    //THIS NEEDS TO EXIST, its to tell for opposite directions, PUT AT BACK that way we can run if statement 
    //		on the sequence to see if ASIGS_FLIP exists in this call, then we know difference between "beh" and "heb"
    //			To assign said difference, 
    Asigs_Ebeh = Asigs_E | Asigs_B | Asigs_E | Asigs_H,
    //up -> down
    Asigs_Eheb = Asigs_E | Asigs_H | Asigs_E | Asigs_B | Asigs_Flip,
    //down -> up
    Asigs_Edef = Asigs_E | Asigs_D | Asigs_E | Asigs_F,
    //left -> right
    Asigs_Efed = Asigs_E | Asigs_F | Asigs_E | Asigs_D | Asigs_Flip,
    //right -> left
    Asigs_Eaei = Asigs_E | Asigs_A | Asigs_E | Asigs_I,
    //diagonal, top left -> bottom right
    Asigs_Eiea = Asigs_E | Asigs_I | Asigs_E | Asigs_A | Asigs_Flip,
    //diagonal, bottom right -> top left
    Asigs_Eceg = Asigs_E | Asigs_C | Asigs_E | Asigs_G,
    //diagonal, top right -> bottom left
    Asigs_Egec = Asigs_E | Asigs_G | Asigs_E | Asigs_C | Asigs_Flip,
    //diagonal, bottom left -> top right
    Asigs_Eabc = Asigs_E | Asigs_A | Asigs_B | Asigs_C,
    //swoop, top left -> top right
    Asigs_Ecba = Asigs_E | Asigs_C | Asigs_B | Asigs_A | Asigs_Flip,
    //swoop, top right -> top left
    Asigs_Eghi = Asigs_E | Asigs_G | Asigs_H | Asigs_I,
    //swoop, bottom left -> bottom right
    Asigs_Eihg = Asigs_E | Asigs_I | Asigs_H | Asigs_G | Asigs_Flip,
    //swoop, bottom right -> bottom left
    Asigs_Eadg = Asigs_E | Asigs_A | Asigs_D | Asigs_G,
    //swoop, top left -> bottom left
    Asigs_Egda = Asigs_E | Asigs_G | Asigs_D | Asigs_A | Asigs_Flip,
    //swoop, bottom left -> top left
    Asigs_Ecfi = Asigs_E | Asigs_C | Asigs_F | Asigs_I,
    //swoop, top right -> bottom right
    Asigs_Eifc = Asigs_E | Asigs_I | Asigs_F | Asigs_C | Asigs_Flip,
    //swoop, bottom right -> top right
    Asigs_Ed = Asigs_E | Asigs_D | Asigs_Flip,
    //flick left
    Asigs_Ef = Asigs_E | Asigs_F,
    //flick right

	Asigs_Edab = Asigs_E | Asigs_D | Asigs_A | Asigs_B | Asigs_Flip,
	//quarter circle left up
	Asigs_Efcb = Asigs_E | Asigs_F | Asigs_C | Asigs_B
	//quarter circle right up
};

USTRUCT(BlueprintType)
struct FCoord_Cell
{
    // m00  m01
    // m10  m11
    GENERATED_USTRUCT_BODY()
    ;

    FCoord_Cell()
    {
        M00.Key = -1;
        M01.Key = 1;
        M10.Key = -1;
        M11.Key = 1;

        M00.Value = 1;
        M01.Value = 1;
        M10.Value = -1;
        M11.Value = -1;
    }

    FCoord_Cell(const double M00X, const double M00Y, const double M01X, const double M01Y, const double M10X, const double M10Y, const double M11X,
                const double M11Y)
    {
        M00.Key = M00X;
        M01.Key = M01X;
        M10.Key = M10X;
        M11.Key = M11X;

        M00.Value = M00Y;
        M01.Value = M01Y;
        M10.Value = M10Y;
        M11.Value = M11Y;
    }

    FCoord_Cell(const TTuple<double, double> M00In, const TTuple<double, double> M01In, const TTuple<double, double> M10In,
                const TTuple<double, double> M11In)
    {
        M00 = M00In;
        M01 = M01In;
        M10 = M10In;
        M11 = M11In;
    }

    TTuple<double, double> M00, M01, M10, M11;
};

USTRUCT(BlueprintType)
struct FAsigs_Cell
{
    GENERATED_USTRUCT_BODY()
    ;

    FAsigs_Cell()
    {
        MCellNum = Asigs_E;
        Init();
    }

    explicit FAsigs_Cell(const EAsigs_State Cell)
    {
        MCellNum = Cell;
        Init();
    }

    void Init()
    {
        //current system sets grid cells to be set on unit circle
        switch (MCellNum)
        {
        case Asigs_A:
            MCellCords = FCoord_Cell(-1.0, 1.0, -0.33, 1.0, -1.0, 0.33, -0.33, 0.33);
            break;
        case Asigs_B:
            MCellCords = FCoord_Cell(-0.33, 1.0, 0.33, 1.0, -0.33, 0.33, 0.33, 0.33);
            break;
        case Asigs_C:
            MCellCords = FCoord_Cell(0.33, 1.0, 1.0, 1.0, 0.33, 0.33, 1.0, 0.33);
            break;
        case Asigs_D:
            MCellCords = FCoord_Cell(-1.0, 0.33, -0.33, 0.33, -1.0, -0.33, -0.33, -0.33);
            break;
        case Asigs_E:
            MCellCords = FCoord_Cell(-0.33, 0.33, 0.33, 0.33, -0.33, -0.33, 0.33, -0.33);
            break;
        case Asigs_F:
            MCellCords = FCoord_Cell(0.33, 0.33, 1.0, 0.33, 0.33, -0.33, 1.0, -0.33);
            break;
        case Asigs_G:
            MCellCords = FCoord_Cell(-1.0, -0.33, -0.33, -0.33, -1.0, -1.0, -0.33, -1.0);
            break;
        case Asigs_H:
            MCellCords = FCoord_Cell(-0.33, -0.33, 0.33, -0.33, -0.33, -1.0, 0.33, -1.0);
            break;
        case Asigs_I:
            MCellCords = FCoord_Cell(0.33, -0.33, 1.0, -0.33, 0.33, -1.0, 1.0, -1.0);
            break;
        default:
            MCellCords = FCoord_Cell(-0.33, 0.33, 0.33, 0.33, -0.33, -0.33, 0.33, -0.33);
            break;
        }
    }

    EAsigs_State MCellNum;
    FCoord_Cell MCellCords;
};

USTRUCT(BlueprintType)
struct FAsigs_Grid3X3
{
    GENERATED_USTRUCT_BODY()
    ;

    FAsigs_Grid3X3()
    {
        MGrid.Add(FAsigs_Cell(Asigs_A));
        MGrid.Add(FAsigs_Cell(Asigs_B));
        MGrid.Add(FAsigs_Cell(Asigs_C));
        MGrid.Add(FAsigs_Cell(Asigs_D));
        MGrid.Add(FAsigs_Cell(Asigs_E));
        MGrid.Add(FAsigs_Cell(Asigs_F));
        MGrid.Add(FAsigs_Cell(Asigs_G));
        MGrid.Add(FAsigs_Cell(Asigs_H));
        MGrid.Add(FAsigs_Cell(Asigs_I));
    }

    TArray<FAsigs_Cell> MGrid;
};

USTRUCT(BlueprintType)
struct FPlayerStatus
{
    GENERATED_USTRUCT_BODY()
    
    //Contains all player status stuff
    bool bIsCasting;
    bool bIsBlockingLeft;
    bool bIsBlockingRight;
};

class UInputComponent;

UCLASS(config = Game)
class GRILLEDPIGEON_API APlayerCharacter final : public ACharacter
{
    GENERATED_BODY()

protected:
    /** Pawn mesh: 1st person view (arms; seen only by self) */
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Mesh)
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

    UFUNCTION()
    void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

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
    UFUNCTION(BlueprintCallable, Category=AutoAim)
    AActor* CapsuleTraceForEnemy() const;

    /*
     * Smoothly aim at skeletal mesh socket on given enemy actor
     */
    UFUNCTION(BlueprintCallable, Category=AutoAim)
    void AutoAimAtEnemy(AActor* Enemy, FName SocketName) const;

    /*
        take in analog stick axis value, determine cell coordinate, add to sequence vector
    */
    UFUNCTION(BlueprintCallable, Category=ASIGS)
    void NextInSequence(float XAxis, float YAxis);

    /*
        return true if given the buffer range that the analog stick has moved "fast enough" for input (its out of the range)
        uses simple 2d circle point collision. Uses the stored previous value with radius of inputBuffer
    */
    UFUNCTION(BlueprintCallable, Category=ASIGS)
    bool BufferRangeCheck(float InputBufferRadius, float CurrentAnalogPosX, float CurrentAnalogPosY) const;

    /*
        determine sequence out from vector, call where input is located
    */
    UFUNCTION(BlueprintCallable, Category=ASIGS)
    void SequenceOut(float XAxis, float YAxis, float InputBufferRadius);

    /*
        combine the sequence list into a single enum value and return it
    */
    UFUNCTION(BlueprintCallable, Category=ASIGS)
    EAsigs_State ConcatSequence();
    
    /*
     * Run each tick regenerate player's focus up to max based on focus regen speed
     * Also clamps max current focus to max focus
     */
    UFUNCTION(BlueprintCallable, Category=Health)
    void RegenerateFocus();

    UFUNCTION(BlueprintCallable, Category=Cast)
	void Cast();

	//Spell to be cast
    UFUNCTION(BlueprintCallable, Category=Cast)
    void CastArcaneBoltSpell();

    UFUNCTION(BlueprintCallable, Category=Cast)
    void CastGridPulseSpell();

    UFUNCTION(BlueprintCallable, Category=Cast)
	void CastIceKnifeSpell();

    /*
        accessing spell class of arcane bolt
    */
    UPROPERTY(EditDefaultsOnly, Category = CASTING)
        TSubclassOf<class AArcaneBolt> ArcaneBoltSpellClass;

    /*
        accessing spell class of grid pulse
    */
    UPROPERTY(EditDefaultsOnly, Category = CASTING)
        TSubclassOf<class AGridPulse> GridPulseSpellClass;

	/*
		accessing spell class of ice knife
	*/
    UPROPERTY(EditDefaultsOnly, Category = CASTING)
    TSubclassOf<class AIceKnife> IKSpellClass;

    UFUNCTION(BlueprintCallable, Category=Cast)
    void SetCastingStatus(const bool B);

    UFUNCTION(BlueprintCallable, Category=Cast)
    void EndCastingStatus();

    UFUNCTION(BlueprintCallable, Category=Block)
    void BlockLeft();

    UFUNCTION(BlueprintCallable, Category=Block)
    void EndBlockingLeftStatus();

    UFUNCTION(BlueprintCallable, Category=Block)
    void BlockRight();

    UFUNCTION(BlueprintCallable, Category=Block)
    void EndBlockingRightStatus();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CameraShake)
    TSubclassOf<class UCameraShake> CameraShake_CastSuccess;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CameraShake)
    TSubclassOf<class UCameraShake> CameraShake_SpellBlocked;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CameraShake)
    TSubclassOf<class UCameraShake> CameraShake_DamageTaken;
    

protected:
    /**
     * Whether or not the player's crosshair is locked on to an enemy
     */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LockOnSystem)
    bool IsLockedOn = false;

    /**
     * Array of all enemy actors
     */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LockOnSystem)
    TArray<AActor*> EnemyArray;

    /*
     * Index value of the currently selected/targeted enemy actor from EnemyArray
     */
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
    FAsigs_Grid3X3 Asigs;

    /*
        range to judge a considerable amount of intended input
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASIGS)
    float Ibr = 0.000001f;

    /*
        timer system to delay the concatenation stage
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASIGS)
    int DelayTimerMax = 10; // WAS AT 35 // If at 10, wait for 1/6 of a second before going into concat stage when fps is locked at 60

    /*
     * Current time remaining on delay timer (0 - DelayTimerMax)
     */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASIGS)
    int DelayTimerCurrent;

    /*
        current single element sequence of enums, later to be combined to create a unique enum 
        to output to editor (possible swoop or diagonal creation)
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASIGS)
    TArray<int32> UpdatingSequence;

    /*
        prior frames input for pitch and yaw analog stick axis
    */
    TTuple<float, float> PreviousInput;

    /*
        Value determined by lock on state, if locked on and ready to start casting we are true. default is true.
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASIGS)
    bool GatheringSequence = true;

    /*
        final return sequence enum signifier to grab from the editor
        IN EDITOR BLUEPRINTS EXAMPLE:

            if output sequence == ASIGS_cfi then do the swoop spell
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASIGS)
    int32 OutputSequence;

    /*
        For telling whether we are going in the negative direction or the positive direction
            if the FIRST MOVE goes in the negative direction, allow for the appendage of the negative enum
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ASIGS)
    bool bIsNegative = false;

    /*
        Offset of spell
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CASTING)
    FVector CastOffset = FVector(50,50,50);

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

    FTimerHandle CastingTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Combat)
    float PlayerAttackingTimerLength = 1.0;

    FTimerHandle BlockingLeftTimerHandle;

    FTimerHandle BlockingRightTimerHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Blocking)
    float PlayerBlockingTimerLength = 1.0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Animation)
    bool bAnimIsCasting = false;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
    FPlayerStatus PlayerStatus;

    /**
     * The enemy actor to be locked on to
     */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LockOnSystem)
    AActor* EnemyActor;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LockOnSystem)
    FVector EnemyActorLocation;

public:
    /** Returns Mesh1P subobject **/
    FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
    /** Returns FirstPersonCameraComponent subobject **/
    FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

    UFUNCTION(BlueprintCallable, Category=Helper)
    AActor* GetEnemyActor() const { return EnemyActor; }

    /*
     * apply x amount of damage to the player
     * if they have vitality points, those are used first
     */
    UFUNCTION(BlueprintCallable)
        void DamagePlayer(float Damage, bool bWasBlocked);
};
