// Fill out your copyright notice in the Description page of Project Settings.


#include "GT/Character/GTCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"
#include "MotionWarpingComponent.h"

AGTCharacterBase::AGTCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

    MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));

    GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, -1.f);
    GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
    GetCharacterMovement()->MaxWalkSpeedCrouched = 250.f;

    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MaxAcceleration = 1200.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 300.f;
    GetCharacterMovement()->BrakingFrictionFactor = 1.f;
    GetCharacterMovement()->bUseSeparateBrakingFriction = false; 
    GetCharacterMovement()->BrakingFriction = 1.5f;


    FGaitSettings SneekingSettings;
    SneekingSettings.MaxWalkSpeed = 250.f;
    SneekingSettings.MaxAcceleration = 800.f;
    SneekingSettings.BrakingDeceleration = 400.f;
    SneekingSettings.BrakingFrictionFactor = 1;
    SneekingSettings.UseSeperateBrakingFriction = true;
    SneekingSettings.BrakingFriction = 1.5f;

    FGaitSettings JoggingSettings;
    JoggingSettings.MaxWalkSpeed = 500.f;
    JoggingSettings.MaxAcceleration = 1200.f;
    JoggingSettings.BrakingDeceleration = 300.f;
    JoggingSettings.BrakingFrictionFactor = 1.f;
    JoggingSettings.UseSeperateBrakingFriction = false;
    JoggingSettings.BrakingFriction = 1.5f;

    FGaitSettings SprintingSettings;
    SprintingSettings.MaxWalkSpeed = 700.f;
    SprintingSettings.MaxAcceleration = 2000.f;
    SprintingSettings.BrakingDeceleration = 200.f;
    SprintingSettings.BrakingFrictionFactor = 1.f;
    SprintingSettings.UseSeperateBrakingFriction = false;
    SprintingSettings.BrakingFriction = 1.5f;

    GaitSettings.Add(EGaits::Sneeking, SneekingSettings);
    GaitSettings.Add(EGaits::Jogging, JoggingSettings);
    GaitSettings.Add(EGaits::Sprinting, SprintingSettings);
}

void AGTCharacterBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AGTCharacterBase::Jump()
{
    Super::Jump();
    //   PlayAudio(UKismetMathLibrary::MapRangeClamped(GetCharacterMovement()->Velocity.Size2D(), 0.f, 500.f, 0.5f, 1.f), true);
}

void AGTCharacterBase::ToggleSneek(bool Newbool)
{
    if (GetCharacterMovement()->IsCrouching()) return;
    if (Gait == EGaits::Sprinting) return;
    Gait = (Newbool) ? EGaits::Sneeking : EGaits::Jogging;
    bWantsToRun = Newbool;

    FGaitSettings* NewSettings = GaitSettings.Find((Newbool) ? EGaits::Sneeking : EGaits::Jogging);
    GetCharacterMovement()->MaxWalkSpeed = CalculateMaxSpeed();
    GetCharacterMovement()->MaxAcceleration = NewSettings->MaxAcceleration;
    GetCharacterMovement()->BrakingDecelerationWalking = NewSettings->BrakingDeceleration;
    GetCharacterMovement()->BrakingFrictionFactor = NewSettings->BrakingFrictionFactor;
    GetCharacterMovement()->bUseSeparateBrakingFriction = NewSettings->UseSeperateBrakingFriction;
    GetCharacterMovement()->BrakingFriction = NewSettings->BrakingFriction;
}

void AGTCharacterBase::ToggleSprint(bool Newbool)
{
    if (GetCharacterMovement()->IsCrouching()) return;
    if (Gait == EGaits::Sneeking) return;
    Gait = (Newbool) ? EGaits::Sprinting : EGaits::Jogging;
    bWantsToRun = Newbool;

    FGaitSettings* NewSettings = GaitSettings.Find((Newbool) ? EGaits::Sprinting : EGaits::Jogging);
    GetCharacterMovement()->MaxWalkSpeed = CalculateMaxSpeed();
    GetCharacterMovement()->MaxAcceleration = NewSettings->MaxAcceleration;
    GetCharacterMovement()->BrakingDecelerationWalking = NewSettings->BrakingDeceleration;
    GetCharacterMovement()->BrakingFrictionFactor = NewSettings->BrakingFrictionFactor;
    GetCharacterMovement()->bUseSeparateBrakingFriction = NewSettings->UseSeperateBrakingFriction;
    GetCharacterMovement()->BrakingFriction = NewSettings->BrakingFriction;
}

void AGTCharacterBase::ToggleCrouch()
{
    ToggleSneek(false);
    ToggleSprint(false);

    if (bIsCrouched)
    {
        // Later should check can UnCrouch, notify if can't
        UnCrouch();
    }
    else
    {
        // Later should check can Crouch, notify if can't
        Crouch();
        GetCharacterMovement()->CanCrouchInCurrentState();
        GetCharacterMovement()->CanEverCrouch();
    }
}

void AGTCharacterBase::ToggleStrafe(bool Newbool)	// true : character rotates along controller. 
{
    // FreeLock camera... alt start ToggleStrafe(false)+@ / alt end ToggleStrafe(true) +@
    bWantsToStrafe = Newbool;
    bUseControllerRotationYaw = Newbool;
    GetCharacterMovement()->bOrientRotationToMovement = !Newbool;
 //   GetCharacterMovement()->bUseControllerDesiredRotation = Newbool; // use? no? turn in place...
}

void AGTCharacterBase::ToggleAim(bool Newbool)
{
    bWantsToAim = Newbool;
//    if (bWantsToAim) ToggleStrafe(true);
}


void AGTCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGTCharacterBase::Landed(const FHitResult& Hit)
{
//    FVector LandVelocity = GetCharacterMovement()->Velocity;
//    PlayAudio(UKismetMathLibrary::MapRangeClamped(LandVelocity.Z, -500.f, -900.f, 0.5f, 1.5f), false);
    JustLanded = true; 

    FTimerHandle Handle;
    GetWorld()->GetTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([this] {
        JustLanded = false;
    }), 0.3f, false);
}

float AGTCharacterBase::CalculateMaxSpeed() const
{
    if (!StrafeSpeedCurve) return 0.f;
    float StrafeSpeed = StrafeSpeedCurve->GetFloatValue(UKismetMathLibrary::Abs(UKismetAnimationLibrary::CalculateDirection
    (GetCharacterMovement()->Velocity, GetActorRotation())));

    FVector SpeedVector = FVector::ZeroVector;
    if (GetCharacterMovement()->IsCrouching())
    {
        SpeedVector = CrouchSpeed;
    }
    else
    {
        switch (Gait)
        {
        case EGaits::Sneeking:
            SpeedVector = WalkSpeed;
            break;
        case EGaits::Jogging:
            SpeedVector = JogSpeed;
            break;
        case EGaits::Sprinting:
            SpeedVector = SprintSpeed;
            break;
        default:
            break;
        }
    }


    if (StrafeSpeed < 1.f)
    {
        return UKismetMathLibrary::MapRangeClamped(StrafeSpeed, 0.f, 1.f, SpeedVector.X, SpeedVector.Y);
    }
    return UKismetMathLibrary::MapRangeClamped(StrafeSpeed, 1.f, 1.f, SpeedVector.Y, SpeedVector.Z);
}
