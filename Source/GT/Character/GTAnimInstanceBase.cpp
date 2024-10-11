// Fill out your copyright notice in the Description page of Project Settings.


#include "GT/Character/GTAnimInstanceBase.h"
#include "GTCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AnimationWarpingLibrary.h"
#include "ChooserFunctionLibrary.h"
#include "PoseSearch/MotionMatchingAnimNodeLibrary.h"
#include "BlendStack/BlendStackAnimNodeLibrary.h"

//#include "MotionTrajectory/Public/CharacterTrajectoryComponent.h"
//#include "PoseSearch/PoseSearchLibrary.h"
//#include "KismetAnimationLibrary.h"
//#include "Kismet/KismetSystemLibrary.h"
//#include "Kismet/GameplayStatics.h"

void FGTAnimInstanceProxy::InitializeObjects(UAnimInstance* InAnimInstance)
{
	FAnimInstanceProxy::InitializeObjects(InAnimInstance);

	Owner = InAnimInstance->TryGetPawnOwner();
	if (!IsValid(Owner))return;

	Character = Cast<AGTCharacterBase>(Owner);
	MovementComponent = Cast<UCharacterMovementComponent>(Owner->GetMovementComponent());
}

void FGTAnimInstanceProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
	FAnimInstanceProxy::PreUpdate(InAnimInstance, DeltaSeconds);
}

void FGTAnimInstanceProxy::Update(float DeltaSeconds)
{
	FAnimInstanceProxy::Update(DeltaSeconds);
}

void UGTAnimInstanceBase::SetTraversalTransform(FTransform InTransform)
{
	TraversalInteractionTransform = InTransform;
}

FTransform UGTAnimInstanceBase::GetTraversalTransform()
{
	return TraversalInteractionTransform;
}

void UGTAnimInstanceBase::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	if (!Proxy.Owner) return;
	
	UpdateEssentialValues(DeltaSeconds);
	GenerateTrajectory(DeltaSeconds);
	UpdateStates();
}

void UGTAnimInstanceBase::UpdateEssentialValues(float DeltaSeconds)
{
	//Cache the actor (capsule) transform. This is mainly for convenience and to ensure thread safety with Property Access.
	CharacterTransform = Proxy.Owner->GetActorTransform();

	//Caches the root bone transform from the offset root bone node if possible. 
	//Storing the offset this way instead of using the Get Socket Transform function ensures a more accurate value. 
	//Since Skeletal Meshes are rotated -90 degrees by default in editor, an offset is added to make angle comparisons against other transforms easier.
	OffsetRootBoneEnabled = UKismetSystemLibrary::GetConsoleVariableBoolValue("a.animnode.offsetrootbone.enable");
	if (OffsetRootBoneEnabled)
	{
		// At GameAnimationSample, here UpdateRootBone. Need AnimNodeGraph_OffsetRootBone, how to get it at C++?
		// Instead, bind UpdateOffsetRoot fuction to AnimNodeGraph_OffsetRootBone.
	}
	else 
	{
		RootTransform = CharacterTransform;
	}

	//Caches important information about the Character¡¯s Acceleration, which is the input acceleration applied by the movement component, not the physical acceleration. 
	//Although not all of these values are currently used in the graph, they are important to have on hand for additional features.
	Acceleration = Proxy.MovementComponent->GetCurrentAcceleration();
	AccelerationAmount = Acceleration.Size() / Proxy.MovementComponent->MaxAcceleration;
	HasAcceleration = AccelerationAmount > 0.f;

	//Caches important information about the Character¡¯s Velocity. 
	//Although not all of these values are currently used in the graph, they are important to have on hand for additional features.
	VelocityLastFrame = Velocity;
	Velocity = Proxy.MovementComponent->Velocity;
	Speed2D = Velocity.Size2D();
	HasVelocity = Speed2D > 5.f;
	VelocityAcceleration = Velocity - VelocityLastFrame / UKismetMathLibrary::Max(DeltaSeconds, 0.001f);
	if (HasVelocity) LastNonZeroVelocity = Velocity;

	//Caches the value of a data driven CVar (Console Variables that are created in the project settings). 
	//We use this integer in the main Motion Matching Chooser to swap between Dense and Sparse databases.
	MMDatabaseLOD = UKismetSystemLibrary::GetConsoleVariableBoolValue("DDCvar.MMDatabaseLOD");

	//Caches the tags from the currently selected Motion matching database. This allows us to make additional selection choices based on the current database.
	if (CurrentSelectedDatabase.IsValid()) CurrentDatabaseTags = CurrentSelectedDatabase->Tags;
}

void UGTAnimInstanceBase::GenerateTrajectory(float DeltaSeconds)
{
	//This function updates the trajectory history and generates a future prediction using the motion model from the default character movement component. 
	//Custom character movement components may need a custom Generate Trajectory function. 
	//Known Issues : Montage root motion and warping will not affect trajectory generation.
	FPoseSearchQueryTrajectory OutTrajectory;
	UPoseSearchTrajectoryLibrary::PoseSearchGenerateTrajectory(this, (Speed2D > 0.f) ? TrajectoryGenerationData_Moving : TrajectoryGenerationData_Idle,
		DeltaSeconds, Trajectory, PreviousDesiredControllYaw, OutTrajectory, -1.f, 30, 0.1f, 15);

	//EXPERIMENTAL!: This function takes the generated trajectory and applies gravity over time, and also uses traces to predict collision, 
	//mainly used while in the air.
	//Known Issues : collision checks are simple and often produce bad trajectories when near walls and ceilings, 
	//in the future we want collision checks to be more robust and handle more cases.
	TArray<AActor*> IgnoredActors;
	UPoseSearchTrajectoryLibrary::HandleTrajectoryWorldCollisions(Proxy.Character, this, OutTrajectory, true, 0.01f, Trajectory, TrajectoryCollision,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, IgnoredActors, EDrawDebugTrace::None, true, 150.f, FLinearColor::Red, FLinearColor::Green, 5.0f);

	//Each Get Trajectory Sample at Time node grabs a different trajectory position in the future based on the Time input pin value. 
	//2 positions in time are retrieved in order to calculate the future velocity, which is used to determine whether the character is moving, stopping, pivoting, etc. 
	//In the future we plan to offer trajectory analysis functions which will perform calculations like these for you.
	FPoseSearchQueryTrajectorySample OutSample1;
	UPoseSearchTrajectoryLibrary::GetTrajectorySampleAtTime(Trajectory, 0.5f, OutSample1, false);
	FPoseSearchQueryTrajectorySample OutSample2;
	UPoseSearchTrajectoryLibrary::GetTrajectorySampleAtTime(Trajectory, 0.4f, OutSample2, false);
	FutureVelocity = (OutSample1.Position - OutSample2.Position) * 10.f;	// 0.1second diff, position diff. *10, 1sec's position diff
}

void UGTAnimInstanceBase::UpdateStates()
{
	//This function caches all of the important gameplay relevant states that the character is in as well as saves the last frame's value 
	//so that we can determine when the value has changed. It saves them into easy to read Enums for convenience, 
	//which are primarily used in the Motion Matching Chooser to filter out motion matching databases, 
	//so that only motions from the current gameplay state can be selected.


	//Caches the character¡¯s Movement Mode and saves the last frame's value. The Movement Mode Enum is the main mode that the character movement component is in. 
	//This mode is saved into a custom Enum with similar names, so that we can map multiple modes into one, and gives us more freedom over the naming.
	MovementModeLastFrame = MovementMode;
	switch (Proxy.MovementComponent->MovementMode)
	{
	case MOVE_None:
		MovementMode = EMovementType::OnGround;
		break;
	case MOVE_Walking:
		MovementMode = EMovementType::OnGround;
		break;
	case MOVE_NavWalking:
		MovementMode = EMovementType::OnGround;
		break;
	case MOVE_Falling:
		MovementMode = EMovementType::InAir;
		break;
	case MOVE_Swimming:
		break;
	case MOVE_Flying:
		break;
	case MOVE_Custom:
		break;
	case MOVE_MAX:
		break;
	default:
		break;
	}

	//Caches the character's Rotation Mode and saves the last frame's value. The Rotation Mode Enum describes the rotation behavior of the capsule in the character movement component.
	//OrientToMovement describes when the character rotates toward the direction of movement(traditional 3rd person action adventure style).
	//Strafe describes when the character rotates toward the camera direction(traditional 3rd person shooter style).
	RotationModeLastFrame = RotationMode;
	RotationMode = (Proxy.MovementComponent->bOrientRotationToMovement) ? ERotationMode::OrientToMovement : ERotationMode::Strafe;

	//Caches the character¡¯s Movement State and saves the last frame's value. 
	//The Movement State Enum describes whether or not the character wants to be moving and playing locomotion animations, or wants to be stopped and playing idle animations. 
	//It does not represent whether or not the character currently has a velocity, rather it is calculated using the current and future velocities 
	//to determine if the character is trying to move (future velocity does not equal 0), or trying to stop (future velocity equals 0).
	MovementStateLastFrame = MovementState;
	MovementState = IsMoving() ? EMovementState::Moving : EMovementState::Idle;

	//Caches the character's Gait and saves the last frame's value. The Gait Enum describes the intended style of locomotion from input, not the current speed. 
	//For example, the character could be moving slowly but still be in the Run Gait Enum state.
	GaitLastFrame = Gait;
	Gait = Proxy.Character->Gait;

	//Caches the character Stance and saves the last frame's value. The Stance Enum describes whether the character movement component is in the crouching mode or not.
	StanceLastFrame = Stance;
	Stance = Proxy.MovementComponent->IsCrouching() ? EStance::Crouch : EStance::Stand;
}

bool UGTAnimInstanceBase::IsMoving() const
{
	return Velocity != FVector::ZeroVector && FutureVelocity != FVector::ZeroVector;
}

bool UGTAnimInstanceBase::IsStarting() const
{
	//This function is used to determine if the character is starting to move by checking if the future velocity is greater than the current velocity. 
	//If the current Database asset is a pivot database, this function will always return false. 
	//This prevents the Motion Matching system from interrupting a pivot, since the second half of a pivot is very similar to a start.
	if (!Proxy.MovementComponent) return false;
	return FutureVelocity.Size2D() >= Velocity.Size2D() + 100.f && IsMoving() && !CurrentDatabaseTags.Contains("Pivots"); //  && !CurrentDatabaseTags.Contains("Pivots"), if combine this function with IsPivoting 
}

bool UGTAnimInstanceBase::IsPivoting() const
{
	//This function is used to determine if the character is pivoting by checking 
	// if the character¡¯s future trajectory is moving in a much different direction than the character¡¯s current trajectory. 
	//The Rotation Modes have a different threshold, since 45 degree pivots work nicely during strafing, but are not necessary during Orient to Movement.
	if (!Proxy.MovementComponent) return false;
	return UKismetMathLibrary::Abs(UKismetMathLibrary::NormalizedDeltaRotator(UKismetMathLibrary::MakeRotFromX(FutureVelocity),
		UKismetMathLibrary::MakeRotFromX(Velocity)).Yaw) >=40.f;
}

bool UGTAnimInstanceBase::ShouldTurnInPlace() const
{
	//This function is used to determine if the character is turning in place 
	//by checking if the root bone rotation is different from the character¡¯s capsule rotation. 
	//For this project, if the rotation is greater than MaxTurnAngle or the character is currently aiming, the character should be turned in place. We also allow turn in places to play if the character has just stopped, which gives us a "Stick Flick" behavior.

	//Turn in place behavior during the aiming state is still WIP.Additional limits need to be applied to the steering or root offset node to prevent the character from lagging too far behind.
	if (!Proxy.Character) return false;
	return UKismetMathLibrary::Abs(UKismetMathLibrary::NormalizedDeltaRotator(CharacterTransform.Rotator(),
		RootTransform.Rotator()).Yaw) >= MaxTurnAngle || MovementState == EMovementState::Idle ||
		MovementStateLastFrame == EMovementState::Moving && Proxy.Character->bWantsToAim;
}

bool UGTAnimInstanceBase::ShouldSpin() const
{
	//If the root bone rotation and character¡¯s capsule rotations are very different while moving, this function will allow a spin transition animation to play. Spin transitions are locomotion animations that rotate the character while moving in a fixed world direction, and are useful when switching rotation modes. 

	//For example, if the character is running toward the camera using the Orient to Movement mode and then switching to strafe, 
	//this would require the character to spin 180 degrees very quickly.A spin transition animation would be an ideal transition for this gameplay scenario.
	//Currently, we are using refacing starts in place of spin transitions, but plan to provide actual spin transition data in a future release.
	return UKismetMathLibrary::Abs(UKismetMathLibrary::NormalizedDeltaRotator(CharacterTransform.Rotator(),
		RootTransform.Rotator()).Yaw) >= 130.f && Speed2D >= 150.f;
}

bool UGTAnimInstanceBase::JustLanded_Light() const
{
	//If the character has just landed and the land velocity was less than the heavy land speed threshold, play light land animations.
	if (!Proxy.Character) return false;
	return Proxy.Character->JustLanded && UKismetMathLibrary::Abs(Proxy.Character->LandVelocity.Z)
		< UKismetMathLibrary::Abs(HeavyLandSpeedThreshold);
}

bool UGTAnimInstanceBase::JustLanded_Heavy() const
{
	//If the character has just landed and the land velocity was greater than the heavy land speed threshold, play heavy land animations.
	if (!Proxy.Character) return false;
	return Proxy.Character->JustLanded && UKismetMathLibrary::Abs(Proxy.Character->LandVelocity.Z)
		>= UKismetMathLibrary::Abs(HeavyLandSpeedThreshold);
}

bool UGTAnimInstanceBase::JustTraversed() const
{
	return !IsSlotActive("DefaultSlot") && GetCurveValue("MovingTraversal");
}

bool UGTAnimInstanceBase::ShouldSpinTransition() const
{
	//If the root bone rotation and character¡¯s capsule rotations are very different while moving, this function will allow a spin transition animation to play. Spin transitions are locomotion animations that rotate the character while moving in a fixed world direction, and are useful when switching rotation modes. 

	//For example, if the character is running toward the camera using the Orient to Movement mode and then switching to strafe, this would require the character to spin 180 degrees very quickly.
	//A spin transition animation would be an ideal transition for this gameplay scenario.
	//Currently, we are using refacing starts in place of spin transitions, but plan to provide actual spin transition data in a future release.
	return UKismetMathLibrary::Abs(UKismetMathLibrary::NormalizedDeltaRotator(CharacterTransform.Rotator(),
		RootTransform.Rotator()).Yaw)>= 130.f && Speed2D>=150.f && !CurrentDatabaseTags.Contains("Pivots");
}

EOffsetRootBoneMode UGTAnimInstanceBase::GetOffsetRootRotationMode() const
{
	//This function is used to determine the Offset Root Rotation mode. 
	//If there is currently a montage playing in the default slot, we do not want to maintain any rotational offset, therefore release the offset. 
	//If not, allow the offset to accumulate.

	//The Release Enum essentially blends out any offset, after which it will be locked to the capsule rotation, just as it would be without a root offset node.

	//The Accumulate Enum means the root will counter - rotate any changes to the capsule rotation, making it appear to rotate independently from the capsule, which allows root motion and steering to fully control its rotation.
	return IsSlotActive("DefaultSlot") ? EOffsetRootBoneMode::Release : EOffsetRootBoneMode::Accumulate;
}

EOffsetRootBoneMode UGTAnimInstanceBase::GetOffsetRootTranslationMode() const
{
	//This function is used to determine the Offset Root Translation mode. 
	//If we are currently playing a montage in the default slot, if we are in the air, or if we are on the ground but not moving, we do not want to maintain any Translational offset.

	//The Release Enum essentially blends out any offset, after which it will be locked to the capsule location, just as it would be without a root offset node.

	//The Interpolate Enum means the root is allowed to deviate slightly from the capsule location based on root motion, 
	//but will always try to interpolate back toward center.
	//This is helpful when the animation data and capsule movement are not perfectly matched, such as during starts, pivots, and other complex movements.
	if (IsSlotActive("DefaultSlot")) return EOffsetRootBoneMode::Release;
	switch (MovementMode)
	{
	case EMovementType::OnGround:
		return IsMoving() ? EOffsetRootBoneMode::Interpolate : EOffsetRootBoneMode::Release;
		break;
	case EMovementType::InAir:
		return EOffsetRootBoneMode::Release;
		break;
	default:
		return EOffsetRootBoneMode();
		break;
	}
}

float UGTAnimInstanceBase::GetOffsetRootTranslationHalfLife() const
{
	//This function controls the speed at which the Root Offset node can interpolate the root bone's translation. 
	//When stopped, we want to interpolate very quickly, so that the stop always ends at the capsule's center, 
	//but when moving, we allow for slightly smoother interpolation.
	switch (MovementState)
	{
	case EMovementState::Idle:return 0.1f;
		break;
	case EMovementState::Moving: return 0.2f;
		break;
	default:
		return 0.f;
		break;
	}
}

EOrientationWarpingSpace UGTAnimInstanceBase::GetOrientationWarpingSpace() const
{
	//Orientation Warping uses Offset Root Bone's root orientation as its warping space when it's enabled.
	//When warping mode is in Component mode, we assume root bone is identity.
	return OffsetRootBoneEnabled ? EOrientationWarpingSpace::RootBoneTransform : EOrientationWarpingSpace::ComponentTransform;
}

void UGTAnimInstanceBase::UpdateOffsetRoot(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	//bind this fuction to AnimNodeGraph_OffsetRootBone at AnimBP
	if (!OffsetRootBoneEnabled) return;
	FTransform NewTransform = UAnimationWarpingLibrary::GetOffsetRootTransform(Node);
	RootTransform = FTransform(FRotator(NewTransform.Rotator().Pitch, NewTransform.Rotator().Yaw + 90.f,
		NewTransform.Rotator().Roll), NewTransform.GetLocation(), FVector(1.f, 1.f, 1.f));
}

bool UGTAnimInstanceBase::EnableAO() const
{
	//This function enables the aim offset if the character is in strafe mode, 
	//and is currently aligned with the camera within a set threshold, but not when a montage is playing in the default slot.
	if (!Proxy.Owner) return false;
	return UKismetMathLibrary::Abs(GetAOValue().Yaw) <= 115.f && RotationMode == ERotationMode::Strafe && GetSlotMontageLocalWeight("DefaultSlot") < 0.5f;
}

FRotator UGTAnimInstanceBase::GetAOValue() const
{
	//This function calculates the rotation difference between the character¡¯s root bone and the camera¡¯s control rotation. 
	//The difference is used to drive the aim offset blendspace.
	if (!Proxy.Owner) return FRotator::ZeroRotator;
	return UKismetMathLibrary::NormalizedDeltaRotator(Proxy.Character->GetControlRotation(), RootTransform.Rotator());
}

FVector UGTAnimInstanceBase::CalculateRelativeAccelerationAmount() const
{
	//This function calculates the character¡¯s Relative Acceleration Amount. 
	//This value represents the current amount of acceleration or deceleration relative to the actor rotation. 
	//It is normalized to a range of -1 to 1 so that -1 equals the Max Braking Deceleration, and 1 equals the Max Acceleration of the Character Movement Component.
	if (!Proxy.Owner) return FVector::ZeroVector;

	if (Proxy.MovementComponent->MaxAcceleration <= 0.f || Proxy.MovementComponent->GetMaxBrakingDeceleration() <= 0.f) return FVector::ZeroVector;

	if (UKismetMathLibrary::Dot_VectorVector(Proxy.MovementComponent->GetCurrentAcceleration(), Velocity) > 0.f)
	{
		FVector OutVector = UKismetMathLibrary::Quat_UnrotateVector(CharacterTransform.GetRotation(), UKismetMathLibrary::Vector_ClampSizeMax
		(VelocityAcceleration, Proxy.MovementComponent->MaxAcceleration) / Proxy.MovementComponent->MaxAcceleration);
		return OutVector;
	}
	else
	{
		FVector OutVector = UKismetMathLibrary::Quat_UnrotateVector(CharacterTransform.GetRotation(), UKismetMathLibrary::Vector_ClampSizeMax
		(VelocityAcceleration, Proxy.MovementComponent->GetMaxBrakingDeceleration()) / Proxy.MovementComponent->GetMaxBrakingDeceleration());
		return OutVector;
	}
}

float UGTAnimInstanceBase::GetLeanAmountX() const
{
	//This function uses the character¡¯s Relative Acceleration Amount (calculated from the CalculateRelativeAcceleractionAmount function) 
	//to determine how far the additive lean blendspace can lean the character. 
	//It also scales its value by the current speed so that the character cannot lean as far when moving slowly.
	return CalculateRelativeAccelerationAmount().Y * UKismetMathLibrary::MapRangeClamped(Speed2D, 200.f, 500.f, 0.5f, 1.f);
}

void UGTAnimInstanceBase::UpdateMotionMatching(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	//This Anim Node Function is called whenever the Motion Matching node is updated. 
	//It evaluates a Chooser asset which returns an array of Pose Search Database assets based on the current gameplay context. 
	//This allows us to perform higher level filtering, giving us more control over 
	//what animations the motion matching system is able to select from.

	//For example, we only search from the walk databases when the character is walking(controlled via the player¡¯s input), 
	//preventing motion matching from selecting runs when the character is trying to walk.
	if (!PoseSearchDatabases) return;

	EAnimNodeReferenceConversionResult Result{};
	FMotionMatchingAnimNodeReference MM = UMotionMatchingAnimNodeLibrary::ConvertToMotionMatchingNode(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Failed) return;

	UPoseSearchDatabase* DB = Cast<UPoseSearchDatabase>(UChooserFunctionLibrary::EvaluateChooser(this, PoseSearchDatabases, UPoseSearchDatabase::StaticClass()));
	if (!DB) return;
	UMotionMatchingAnimNodeLibrary::SetDatabaseToSearch(MM, DB, GetMMInterupMode());
}

void UGTAnimInstanceBase::MotionMatchingPostSelection(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	//This function is called after the Motion Matching node has selected an animation. 
	//In this case, we cache the database the selected animation is in, in order to grab the tags in the EventGraph (due to a thread safety issue).

	//In the future, we plan to use this function to control additional things such as blend time and blend profiles based on the selected animation.
	EAnimNodeReferenceConversionResult Result{};
	FMotionMatchingAnimNodeReference MM = UMotionMatchingAnimNodeLibrary::ConvertToMotionMatchingNode(Node, Result);
	if (Result == EAnimNodeReferenceConversionResult::Failed) return;

	FPoseSearchBlueprintResult OutResult;
	bool IsValidResult;
	UMotionMatchingAnimNodeLibrary::GetMotionMatchingSearchResult(MM, OutResult, IsValidResult);
	CurrentSelectedDatabase = OutResult.SelectedDatabase;
}

float UGTAnimInstanceBase::GetMMBlendTime() const
{
	//This function is used to change the blend time of the Motion Matching node, based on the current and previous states. 
	//In the future, we plan to allow blend times to be more directly set from the chosen databases.
	switch (MovementMode)
	{
		//If the movement mode is on the ground but the mode last frame was in the air, the character must've just landed, 
		//therefore set a shorter blend time for the land animations. 
	case EMovementType::OnGround:
		return (MovementModeLastFrame == EMovementType::OnGround) ? 0.4f : 0.2f;
		break;

		//If the movement mode is in the air and the character is moving quickly upward, the character must've just jumped, 
		//therefore set a very short blend time for the jump animations.
	case EMovementType::InAir:
		return Velocity.Z > 100.f ? 0.15 : 0.5;
		break;

	default:
		return 0.f;
		break;
	}
}

EPoseSearchInterruptMode UGTAnimInstanceBase::GetMMInterupMode() const
{
	//This function controls the Interupt Mode of the motion matching node.
	//This determines whether motion matching will force a blend into a new database if the selectable databases have changed, 
	//or wait until it finds a match that costs less than the currently playing animation.By default, we do not interrupt.
	//However, whenever a core state has changed, we know we want to start playing a new animation, therefore force an interrupt.
	//This prevents motion matching from sticking in the idle if the character has started moving, 
	// or staying in a cycle animation if the character wants to stop, which can happen based on the "continuing pose bias" tuning.
	//Essentially, this keeps motion matching responsive to changes in core states.
	bool b1 = (MovementMode != MovementModeLastFrame);
	bool b2 = (MovementState != MovementStateLastFrame) || (Gait != GaitLastFrame) || (Stance != StanceLastFrame) && (MovementMode == EMovementType::OnGround);
	if (b1 || b2)
	{
		return EPoseSearchInterruptMode::InterruptOnDatabaseChange;
	}
	else
	{
		return EPoseSearchInterruptMode::DoNotInterrupt;
	}
}

bool UGTAnimInstanceBase::EnableSteering() const
{
	//This function determines if root motion from the animations can be steered by checking if the character is moving or in the air. 
	//This prevents idle animations from getting steered, which could cause sliding.
	return MovementState == EMovementState::Moving || MovementMode == EMovementType::InAir;
}

FQuat UGTAnimInstanceBase::GetDesiredFacing() const
{
	//This function gives the steering node a target rotation. 
	//This target is calculated using the future facing direction from the predicted trajectory. 
	//This allows the steering node to rotate toward a future direction, rather than always steering toward the current actor rotation, 
	//which could cause it to lag too far behind.
	return Trajectory.GetSampleAtTime(0.5f, false).Facing;
}

void UGTAnimInstanceBase::UpdateBlendStack(const FAnimUpdateContext& Context, const FAnimNodeReference& Node)
{
	// Use this at MotionMatching node's Blend Stack Graph, Update.
	AnimTime = UBlendStackAnimNodeLibrary::GetCurrentBlendStackAnimAssetTime(Node);
	AnimAsset = UBlendStackAnimNodeLibrary::GetCurrentBlendStackAnimAsset(Node);
	UAnimSequence* NewAnimSequence = Cast<UAnimSequence>(AnimAsset);
	if (!NewAnimSequence) return;
	UAnimationWarpingLibrary::GetCurveValueFromAnimation(NewAnimSequence, "Enable_OrientationWarping",
		AnimTime, OrientationAlpha);
}
