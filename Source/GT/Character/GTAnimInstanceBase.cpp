// Fill out your copyright notice in the Description page of Project Settings.


#include "GT/Character/GTAnimInstanceBase.h"
#include "GTCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
	UPoseSearchTrajectoryLibrary::HandleTrajectoryWorldCollisions(Proxy.Character, this, OutTrajectory, true, 0.01f, Trajectory, CollisionResult,
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
	return false;
}
