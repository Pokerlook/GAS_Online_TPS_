// Fill out your copyright notice in the Description page of Project Settings.


#include "GT/Character/GTCharacterBase.h"
#include "GT/Actor/TraversableActor.h"
#include "GT/Interface/TraversalInterface.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MotionWarpingComponent.h"
#include "AnimationWarpingLibrary.h"
#include "Chooser.h"
#include "PoseSearch/PoseSearchLibrary.h"
#include "ChooserFunctionLibrary.h"

//#include "Kismet/GameplayStatics.h"
//#include "PlayMontageCallbackProxy.h"

AGTCharacterBase::AGTCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

    MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));

    GetCharacterMovement()->JumpZVelocity = 500.f;
    GetCharacterMovement()->GroundFriction = 5.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 150.f;
    GetCharacterMovement()->bCanWalkOffLedgesWhenCrouching = true;
    GetCharacterMovement()->AirControl = 0.25f;
    GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

    GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, -1.f);
    GetCharacterMovement()->MaxWalkSpeedCrouched = 250.f;

    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MaxAcceleration = 1200.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 750.f;
    GetCharacterMovement()->BrakingFrictionFactor = 1.f;
    GetCharacterMovement()->bUseSeparateBrakingFriction = false; 
    GetCharacterMovement()->BrakingFriction = 1.5f;


    FGaitSettings SneekingSettings;
    SneekingSettings.MaxWalkSpeed = 250.f;
    SneekingSettings.MaxAcceleration = 800.f;
    SneekingSettings.BrakingDeceleration = 900.f;
    SneekingSettings.BrakingFrictionFactor = 1;
    SneekingSettings.UseSeperateBrakingFriction = true;
    SneekingSettings.BrakingFriction = 1.5f;

    FGaitSettings JoggingSettings;
    JoggingSettings.MaxWalkSpeed = 500.f;
    JoggingSettings.MaxAcceleration = 1200.f;
    JoggingSettings.BrakingDeceleration = 750.f;
    JoggingSettings.BrakingFrictionFactor = 1.f;
    JoggingSettings.UseSeperateBrakingFriction = false;
    JoggingSettings.BrakingFriction = 1.5f;

    FGaitSettings SprintingSettings;
    SprintingSettings.MaxWalkSpeed = 700.f;
    SprintingSettings.MaxAcceleration = 2000.f;
    SprintingSettings.BrakingDeceleration = 400.f;
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

void AGTCharacterBase::TryTraversal()
{
    if (DoingTraversalAction) return;
    if (!GetCharacterMovement()->IsMovingOnGround()) return;

    // Try Traversal Actoion Function //
    float TraceForwardDistance = GetTraversalForwardTraceDistance();

    // Local Variables, without debug variables
    FTraversalCheckResult TraversalCheckResult;
    FVector ActorLocation = FVector::ZeroVector;
    FVector ActorForwardVector = FVector::ZeroVector;
    float CapsuleRadius = 0.f;
    float CapsuleHalfHeight = 0.f;
    FVector HasRoomCheck_FrontLedgeLocation = FVector::ZeroVector; 
    FVector HasRoomCheck_BackLedgeLocation = FVector::ZeroVector; 
    FHitResult TopSweepResult;

    //Step 1: Cache some important values for use later in the function.
    ActorLocation = GetActorLocation();
    ActorForwardVector = GetActorForwardVector();
    CapsuleRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
    CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
    TraversalCheckResult.Speed = GetCharacterMovement()->Velocity.Size2D();
    
    //Step 2.1: Perform a trace in the actor's forward direction to find a Traversable Level Block. 
    //If found, set the Hit Component, if not, exit the function.
    TArray<AActor*> IgnoredActors;
    ETraceTypeQuery TraversalQuery = UEngineTypes::ConvertToTraceType(ECC_Visibility);

    FHitResult HitResult_FindTraversableActor;
    UKismetSystemLibrary::CapsuleTraceSingle(this, 
        ActorLocation, ActorLocation + ActorForwardVector * TraceForwardDistance,
        30.f, 60.f, TraversalQuery, false, IgnoredActors, EDrawDebugTrace::ForDuration, HitResult_FindTraversableActor, true,
        FLinearColor::Black, FLinearColor::Black, 3.f);   // DEBUG
    if (!HitResult_FindTraversableActor.bBlockingHit) return;

    TraversalCheckResult.HitComponent = HitResult_FindTraversableActor.GetComponent();

    ATraversableActor* TraversableBlock = Cast< ATraversableActor>(HitResult_FindTraversableActor.GetActor());
    if (!IsValid(TraversableBlock)) return;

    //Step 2.2: If a traversable level block was found, get the front and back ledge transforms from it (using its own internal function).
    TraversableBlock->GetLedgeTransform(HitResult_FindTraversableActor.ImpactPoint, ActorLocation, TraversalCheckResult);

    // DEBUG
    if (TraversalCheckResult.HasFrontLedge)
    {
        UKismetSystemLibrary::DrawDebugSphere(this, TraversalCheckResult.FrontLedgeLocation, 10.f, 12, FLinearColor::Green, 3.f, 1.f);
    }
    if (TraversalCheckResult.HasBackLedge)
    {
        UKismetSystemLibrary::DrawDebugSphere(this, TraversalCheckResult.BackLedgeLocation, 10.f, 12, FLinearColor::Blue, 3.f, 1.f);
    }

    //Step 3.1 If the traversable level block has a valid front ledge, continue the function. If not, exit early.
    if (!TraversalCheckResult.HasFrontLedge) return;

    //Step 3.2: Perform a trace from the actors location up to the front ledge location to determine if there is room for the actor to move up to it. 
    //If so, continue the function. If not, exit early.
    HasRoomCheck_FrontLedgeLocation= TraversalCheckResult.FrontLedgeLocation + 
        (TraversalCheckResult.FrontLedgeNormal * (CapsuleRadius + 2.f)) + FVector(0.f, 0.f, CapsuleHalfHeight + 2.f);
    FHitResult HitResult_RoomForMovingUp;
    UKismetSystemLibrary::CapsuleTraceSingle(this,
        ActorLocation, HasRoomCheck_FrontLedgeLocation,
        CapsuleRadius, CapsuleHalfHeight, TraversalQuery, false, IgnoredActors, EDrawDebugTrace::ForDuration, HitResult_FindTraversableActor, true,
        FLinearColor::Red, FLinearColor::Green, 3.f);   // DEBUG
    if (!UKismetMathLibrary::BooleanNOR(HitResult_RoomForMovingUp.bBlockingHit, HitResult_RoomForMovingUp.bStartPenetrating))
    {
        TraversalCheckResult.HasFrontLedge = false;
        return;
    }

    //Step 3.3: save the height of the obstacle using the delta between the actor and front ledge transform.
    TraversalCheckResult.ObstacleHeight = UKismetMathLibrary::Abs
    ( ( (ActorLocation - FVector(0.f, 0.f, CapsuleHalfHeight)) - TraversalCheckResult.FrontLedgeLocation).Z);

    //Step 3.4: Perform a trace across the top of the obstacle from the front ledge to the back ledge 
    //to see if theres room for the actor to move across it. 
    HasRoomCheck_BackLedgeLocation = TraversalCheckResult.BackLedgeLocation + 
        TraversalCheckResult.BackLedgeNormal * (CapsuleRadius + 2.f) + FVector(0.f, 0.f, CapsuleHalfHeight + 2.f);
    FHitResult HitResult_HasRoomForCharacter;
    bool bHit_HasRoomForCharacter = UKismetSystemLibrary::CapsuleTraceSingle(this,
        HasRoomCheck_FrontLedgeLocation, HasRoomCheck_BackLedgeLocation,
        CapsuleRadius, CapsuleHalfHeight, TraversalQuery, false, IgnoredActors, EDrawDebugTrace::ForDuration, HitResult_HasRoomForCharacter, true,
        FLinearColor::Red, FLinearColor::Green, 3.f);   // DEBUG
    TopSweepResult = HitResult_HasRoomForCharacter;
    if (!bHit_HasRoomForCharacter)
    {
        //Step 3.5: If there is room, save the obstacle depth using the difference between the front and back ledge locations.
        TraversalCheckResult.ObstacleDepth = (TraversalCheckResult.FrontLedgeLocation - TraversalCheckResult.BackLedgeLocation).Size2D();
    }
    else
    {
        //Step 3.5: If there is not room, save the obstacle depth using the difference between the front ledge and the trace impact point, 
        //and invalidate the back ledge.
        TraversalCheckResult.ObstacleDepth = (TopSweepResult.ImpactPoint - TraversalCheckResult.FrontLedgeLocation).Size2D();
        TraversalCheckResult.HasBackLedge = false;
    }

    //Step 3.6: Trace downward from the back ledge location (using the height of the obstacle for the distance) to find the floor. 
    //If there is a floor, save its location and the back ledges height (using the distance between the back ledge and the floor). 
    //If no floor was found, invalidate the back floor.
    FHitResult HitResult_FindFloor;
    FVector tempEnd = TraversalCheckResult.BackLedgeLocation + TraversalCheckResult.BackLedgeNormal * (CapsuleRadius + 2.f) -
        FVector(0.f, 0.f, (TraversalCheckResult.ObstacleHeight - CapsuleHalfHeight) + 50.f);
    UKismetSystemLibrary::CapsuleTraceSingle(this,
        HasRoomCheck_BackLedgeLocation, tempEnd,
        CapsuleRadius, CapsuleHalfHeight, TraversalQuery, false, IgnoredActors, EDrawDebugTrace::ForDuration, HitResult_FindFloor, true,
        FLinearColor::Red, FLinearColor::Green, 3.f);   // DEBUG

    if (HitResult_FindFloor.bBlockingHit)
    {
        TraversalCheckResult.HasBackFloor = true;
        TraversalCheckResult.BackFloorLocation = HitResult_FindFloor.ImpactPoint;
        TraversalCheckResult.BackLedgeHeight = UKismetMathLibrary::Abs((HitResult_FindFloor.ImpactPoint - TraversalCheckResult.BackLedgeLocation).Z);
    }
    else
    {
        TraversalCheckResult.HasBackFloor = false;
    }

    //Step 4.1: Based on the resulting conditions and parameters of the check, determine what type of traversal action to do.
    if (TraversalCheckResult.HasFrontLedge && TraversalCheckResult.HasBackLedge && !TraversalCheckResult.HasBackFloor && UKismetMathLibrary::
        InRange_FloatFloat(TraversalCheckResult.ObstacleHeight, 50.f, 125.f, true, true) && TraversalCheckResult.ObstacleDepth < 59.f)
    {
        TraversalCheckResult.ActionType = ETraversalActionType::Vault;
    }
    else if (TraversalCheckResult.HasFrontLedge && TraversalCheckResult.HasBackLedge && TraversalCheckResult.HasBackFloor && UKismetMathLibrary::
        InRange_FloatFloat(TraversalCheckResult.ObstacleHeight, 50.f, 125.f, true, true) && TraversalCheckResult.ObstacleDepth < 59.f &&
        TraversalCheckResult.BackLedgeHeight > 50.f) 
    {
        TraversalCheckResult.ActionType = ETraversalActionType::Hurdle;
    }
    else if (TraversalCheckResult.HasFrontLedge && UKismetMathLibrary::InRange_FloatFloat(TraversalCheckResult.ObstacleHeight, 50.f, 275.f, true, true)
        && TraversalCheckResult.ObstacleDepth >= 59.f)
    {
        TraversalCheckResult.ActionType = ETraversalActionType::Mantle;
    }
    else
    {
        TraversalCheckResult.ActionType = ETraversalActionType::None;
    }

    //Step 5.1: Continue if there is a valid action type. If none of the conditions were met, no action can be performed, therefore exit the function.
    if (TraversalCheckResult.ActionType == ETraversalActionType::None) return;

    //Step 5.2: Send the front ledge location to the Anim BP using an interface. 
    //This transform will be used for a custom channel within the following Motion Matching search.
    if (ITraversalInterface* TraversalInterface = Cast< ITraversalInterface>(GetMesh()->GetAnimInstance()) )
    {
        TraversalInterface->SetTraversalTransform(FTransform(
            UKismetMathLibrary::MakeRotFromZ(TraversalCheckResult.FrontLedgeNormal), TraversalCheckResult.FrontLedgeLocation, FVector(1.f, 1.f, 1.f)));
    }
    else
    {
        return;
    }

    //Step 5.3: Evaluate a chooser to select all montages that match the conditions of the traversal check.
    if (!TraversalTable) return; 
    TraversalResult = TraversalCheckResult;
    TArray<UObject*> AssetsToSearch= UChooserFunctionLibrary::EvaluateChooserMulti(this, TraversalTable, UAnimMontage::StaticClass());

    //Step 5.4: Perform a Motion Match on all the montages that were chosen by the chooser to find the best result. 
    //This match will elect the best montage AND the best entry frame (start time) based on the distance to the ledge, and the current characters pose. 
    FPoseSearchFutureProperties NewFuture;
    FPoseSearchBlueprintResult Result;
    UPoseSearchLibrary::MotionMatch(GetMesh()->GetAnimInstance(), UChooserFunctionLibrary::EvaluateChooserMulti(this, TraversalTable, UAnimMontage::StaticClass())
        , "PoseHistory", NewFuture, Result, 42);

    TraversalCheckResult.ChosenMontage = Cast<UAnimMontage>(Result.SelectedAnimation);
    if (!TraversalCheckResult.ChosenMontage) return;
    TraversalCheckResult.StartTime = Result.SelectedTime;
    TraversalCheckResult.PlayRate = Result.WantedPlayRate;

    //Step 5.5: Finally, if the check was a success and a montage was found, trigger the Traversal Event
    PerformTraversalAction(TraversalCheckResult);
}

float AGTCharacterBase::GetTraversalForwardTraceDistance() const
{
    //Gets how fast the character is moving in its forward direction and uses the value to scale the distance of the forward trace.
    return UKismetMathLibrary::MapRangeClamped(
        UKismetMathLibrary::Quat_UnrotateVector(GetActorRotation().Quaternion(),GetCharacterMovement()->Velocity).X, 
        0.f, 500.f, 75.f, 350.f);
}

void AGTCharacterBase::TraversalUpdateWarpTargets()
{
    //In order for the actor to move to the exact points on the obstacle, we use a Motion Warping component which warps the montage¡¯s root motion using notify states on the montage. 
    //This function updates the warp targets in the component using the ledge locations.

    // Local Variables
    float AnimatedDistanceFromFrontLedgeToBackLedge = 0.f;
    float AnimatedDistanceFromFrontLedgeToBackFloor = 0.f;

    //Update the FrontLedge warp target using the front ledge's location and rotation.
    MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation("FrontLedge", TraversalResult.FrontLedgeLocation,
        UKismetMathLibrary::MakeRotFromX(UKismetMathLibrary::NegateVector(TraversalResult.FrontLedgeNormal)));

    //If the action type was a hurdle or a vault, we need to also update the BackLedge target. If it is not a hurdle or vault, remove it.
    if (TraversalResult.ActionType == ETraversalActionType::Hurdle || TraversalResult.ActionType == ETraversalActionType::Vault)
    {
        //Because the traversal animations move at different distances (no fixed metrics), we need to know how far the animation moves in order to warp it properly. 
        //Here we cache a curve value at the end of the Back Ledge warp window 
        //to determine how far the animation is from the front ledge once the character reaches the back ledge location in the animation.
        TArray<FMotionWarpingWindowData> OutData;
        UMotionWarpingUtilities::GetMotionWarpingWindowsForWarpTargetFromAnimation(TraversalResult.ChosenMontage, "BackLedge", OutData);

        if (OutData.IsEmpty()) MotionWarpingComponent->RemoveWarpTarget("BackLedge");
        else
        {
            UAnimationWarpingLibrary::GetCurveValueFromAnimation(TraversalResult.ChosenMontage, "Distance_From_Ledge", OutData[0].EndTime,
                AnimatedDistanceFromFrontLedgeToBackLedge);
            //Update the BackLedge warp target.
            MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation("BackLedge", TraversalResult.BackLedgeLocation, FRotator::ZeroRotator);
        }
    }
    else
    {
        MotionWarpingComponent->RemoveWarpTarget("BackLedge");
    }

    //If the action type was a hurdle, we need to also update the BackFloor target. If it is not a hurdle, remove it.
    if (TraversalResult.ActionType == ETraversalActionType::Hurdle)
    {
        //Caches a curve value at the end of the Back Floor warp window 
        //to determine how far the animation is from the front ledge once the character touches the ground.
        TArray<FMotionWarpingWindowData> OutData2;
        UMotionWarpingUtilities::GetMotionWarpingWindowsForWarpTargetFromAnimation(TraversalResult.ChosenMontage, "BackFloor", OutData2);

        if (OutData2.IsEmpty()) MotionWarpingComponent->RemoveWarpTarget("BackFloor");
        else
        {
            UAnimationWarpingLibrary::GetCurveValueFromAnimation(TraversalResult.ChosenMontage, "Distance_From_Ledge", OutData2[0].EndTime,
                AnimatedDistanceFromFrontLedgeToBackFloor);

            //Since the animations may land on the floor at different distances (a run hurdle may travel further than a walk or stand hurdle), 
            //use the total animated distance away from the back ledge as the X and Y values of the BackFloor warp point. 
            //This could technically cause some collision issues if the floor is not flat, or there is an bostacle in the way, 
            //therefore having fixed metrics for all traversal animations would be an improvement.
            FVector TempLoc(FVector(TraversalResult.BackLedgeLocation + (TraversalResult.BackLedgeNormal * UKismetMathLibrary::Abs
            (AnimatedDistanceFromFrontLedgeToBackLedge - AnimatedDistanceFromFrontLedgeToBackFloor))));
            FVector NewLoc(TempLoc.X, TempLoc.Y, TraversalResult.BackFloorLocation.Z);
            MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation("BackFloor", NewLoc, FRotator::ZeroRotator);
        }
    }
    else
    {
        MotionWarpingComponent->RemoveWarpTarget("BackFloor");
    }

}

void AGTCharacterBase::PerformTraversalAction(FTraversalCheckResult TraversalCheckResult)
{
    //Perform Traversal Action Event 
    //This event is triggered at the end of the TryTraversalAction function, and uses the result to play a traversal montage. 
    //We use the convenient Play Montage Event node, which triggers latent events when the montage is completed, blends out, or is interrupted.
    //We use the Flying movement mode while doing the traversal action to allow Root Motion to move the character in the Z axis. 
    //We also disable collision with the traversed obstacle during the action to ensure the character aligns properly. This is a relatively simple event that can be replaced with a more robust traversal system, and is here for demonstration purposes only.
    TraversalResult = TraversalCheckResult;
    TraversalUpdateWarpTargets();

    UAnimMontage* MontageToPlay = const_cast<UAnimMontage*>(TraversalResult.ChosenMontage);
    if (!MontageToPlay) return;

    // Get the AnimInstance to play the montage
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (!AnimInstance) return;

    // Play the montage
    float MontageDuration = AnimInstance->Montage_Play(MontageToPlay);

    DoingTraversalAction = true;
    GetCapsuleComponent()->IgnoreComponentWhenMoving(TraversalResult.HitComponent, true);
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying, 0);

    if (MontageDuration > 0.f)
    {
        // Bind On Completed event
        FOnMontageEnded EndDelegate;
        EndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
        {
            // if (bInterrupted) Montage was interrupted, else Montage completed
            DoingTraversalAction = true;
            GetCapsuleComponent()->IgnoreComponentWhenMoving(TraversalResult.HitComponent, false);
            EMovementMode NewMovementMode = (TraversalResult.ActionType == ETraversalActionType::Vault) ?
                EMovementMode::MOVE_Falling : EMovementMode::MOVE_Walking;
            GetCharacterMovement()->SetMovementMode(NewMovementMode, 0);
        });

        // Bind the delegate to the AnimInstance's OnMontageEnded
        AnimInstance->Montage_SetEndDelegate(EndDelegate, MontageToPlay);
    }

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
