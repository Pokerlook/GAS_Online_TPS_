// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GT/GTTypes.h"
#include "GT/Interface/TraversalInterface.h"
#include "Chooser.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "Animation/AnimExecutionContext.h"
#include "Animation/AnimNodeReference.h"
#include "PoseSearch/PoseSearchDatabase.h"
#include "PoseSearch/PoseSearchLibrary.h"
#include "PoseSearch/PoseSearchTrajectoryTypes.h"
#include "PoseSearch/PoseSearchTrajectoryLibrary.h"
#include "BoneControllers/AnimNode_OrientationWarping.h"
#include "BoneControllers/AnimNode_OffsetRootBone.h"
#include "GTAnimInstanceBase.generated.h"

USTRUCT()
struct FGTAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

protected:

	virtual void InitializeObjects(UAnimInstance* InAnimInstance) override;
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
	virtual void Update(float DeltaSeconds) override;

public:

	UPROPERTY(Transient)
		APawn* Owner;
	UPROPERTY(Transient)
		class AGTCharacterBase* Character;
	UPROPERTY(Transient)
		class UCharacterMovementComponent* MovementComponent;

};

/**
 * 
 */
UCLASS()
class GT_API UGTAnimInstanceBase : public UAnimInstance, public ITraversalInterface
{
	GENERATED_BODY()
public:
	virtual void SetTraversalTransform(FTransform InTransform) override;
	virtual FTransform GetTraversalTransform() override;

protected:

	UPROPERTY(Transient)
		FGTAnimInstanceProxy Proxy;
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override { return &Proxy; }
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Locomotion", meta = (BlueprintThreadSafe))
		void UpdateEssentialValues(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = "Locomotion", meta = (BlueprintThreadSafe))
		void GenerateTrajectory(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = "Locomotion", meta = (BlueprintThreadSafe))
		void UpdateStates();

	// Movement Analysis
	UFUNCTION(BlueprintCallable, Category = "MovementAnalysis", meta = (BlueprintThreadSafe))
		bool IsMoving() const;
	UFUNCTION(BlueprintCallable, Category = "MovementAnalysis", meta = (BlueprintThreadSafe))
		bool IsStarting() const;
	UFUNCTION(BlueprintCallable, Category = "MovementAnalysis", meta = (BlueprintThreadSafe))
		bool IsPivoting() const;
	UFUNCTION(BlueprintCallable, Category = "MovementAnalysis", meta = (BlueprintThreadSafe))
		bool ShouldTurnInPlace() const;
	UFUNCTION(BlueprintCallable, Category = "MovementAnalysis", meta = (BlueprintThreadSafe))
		bool ShouldSpin() const;
	UFUNCTION(BlueprintCallable, Category = "MovementAnalysis", meta = (BlueprintThreadSafe))
		bool JustLanded_Light() const;
	UFUNCTION(BlueprintCallable, Category = "MovementAnalysis", meta = (BlueprintThreadSafe))
		bool JustLanded_Heavy() const;
	UFUNCTION(BlueprintCallable, Category = "MovementAnalysis", meta = (BlueprintThreadSafe))
		bool JustTraversed() const;
	UFUNCTION(BlueprintCallable, Category = "MovementAnalysis", meta = (BlueprintThreadSafe))
		bool ShouldSpinTransition() const;
	
	// Root Offset
	UFUNCTION(BlueprintCallable, Category = "RootOffset", meta = (BlueprintThreadSafe))
		EOffsetRootBoneMode GetOffsetRootRotationMode() const;
	UFUNCTION(BlueprintCallable, Category = "RootOffset", meta = (BlueprintThreadSafe))
		EOffsetRootBoneMode GetOffsetRootTranslationMode() const;
	UFUNCTION(BlueprintCallable, Category = "RootOffset", meta = (BlueprintThreadSafe))
		float GetOffsetRootTranslationHalfLife() const;
	UFUNCTION(BlueprintCallable, Category = "RootOffset", meta = (BlueprintThreadSafe))
		EOrientationWarpingSpace GetOrientationWarpingSpace() const;
	UFUNCTION(BlueprintCallable, Category = "RootOffset", meta = (BlueprintThreadSafe))
		void UpdateOffsetRoot(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);

	// AimOffset
	UFUNCTION(BlueprintCallable, Category = "AImOffset", meta = (BlueprintThreadSafe))
		bool EnableAO() const;
	UFUNCTION(BlueprintCallable, Category = "AImOffset", meta = (BlueprintThreadSafe))
		FRotator GetAOValue() const;
	
	// Additive Leaning
	UFUNCTION(BlueprintCallable, Category = "AdditiveLeaning", meta = (BlueprintThreadSafe))
		FVector CalculateRelativeAccelerationAmount() const;
	UFUNCTION(BlueprintCallable, Category = "AdditiveLeaning", meta = (BlueprintThreadSafe))
		float GetLeanAmountX() const;

	//State Node Functions
	UFUNCTION(Category = "MotionMatching", BlueprintCallable, meta = (BlueprintThreadSafe))
		void UpdateMotionMatching(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);
	UFUNCTION(Category = "MotionMatching", BlueprintCallable, meta = (BlueprintThreadSafe))
		void MotionMatchingPostSelection(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);
	UFUNCTION(BlueprintCallable, Category = "MotionMatching", meta = (BlueprintThreadSafe))
		float GetMMBlendTime() const;
	UFUNCTION(BlueprintCallable, Category = "MotionMatching", meta = (BlueprintThreadSafe))
		EPoseSearchInterruptMode GetMMInterupMode() const;

	// Steering
	UFUNCTION(BlueprintCallable, Category = "BlendStack", meta = (BlueprintThreadSafe))
		bool EnableSteering() const;
	UFUNCTION(BlueprintCallable, Category = "BlendStack", meta = (BlueprintThreadSafe))
		FQuat GetDesiredFacing() const;
	UFUNCTION(Category = "BlendStack", BlueprintCallable, meta = (BlueprintThreadSafe))
		void UpdateBlendStack(const FAnimUpdateContext& Context, const FAnimNodeReference& Node);
		
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "AimOffset")
		float MaxTurnAngle = 90.f;

	UPROPERTY(BlueprintReadOnly, Category = "EssentialValues")
		FTransform CharacterTransform;
	UPROPERTY(BlueprintReadOnly, Category = "EssentialValues")
		FTransform RootTransform;

	UPROPERTY(BlueprintReadOnly, Category = "EssentialValues")
		bool HasAcceleration = false;
	UPROPERTY(BlueprintReadOnly, Category = "EssentialValues")
		FVector Acceleration = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "EssentialValues")
		float AccelerationAmount = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "EssentialValues")
		bool HasVelocity = false;
	UPROPERTY(BlueprintReadOnly, Category = "EssentialValues")
		FVector Velocity = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "EssentialValues")
		FVector VelocityLastFrame = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "EssentialValues")
		FVector VelocityAcceleration = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "EssentialValues")
		FVector LastNonZeroVelocity = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "EssentialValues")
		float Speed2D = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "EssentialValues")
		int MMDatabaseLOD;
	UPROPERTY(BlueprintReadOnly, Category = "EssentialValues")
		TWeakObjectPtr<const UPoseSearchDatabase> CurrentSelectedDatabase;
	UPROPERTY(BlueprintReadOnly, Category = "EssentialValues")
		TArray<FName> CurrentDatabaseTags;
	UPROPERTY(BlueprintReadOnly, Category = "EssentialValues")
		float HeavyLandSpeedThreshold = 700.f;

	UPROPERTY(BlueprintReadOnly, Category = "EssentialValues")
		FTransform TraversalInteractionTransform;
	UPROPERTY(BlueprintReadOnly, Category = "EssentialValues")
		bool OffsetRootBoneEnabled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trajectory")
		FPoseSearchTrajectoryData TrajectoryGenerationData_Idle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trajectory")
		FPoseSearchTrajectoryData TrajectoryGenerationData_Moving;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trajectory")
		FPoseSearchQueryTrajectory Trajectory;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trajectory")
		FPoseSearchTrajectory_WorldCollisionResults TrajectoryCollision;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trajectory")
		float PreviousDesiredControllYaw = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trajectory")
		FVector FutureVelocity = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "States")
		EMovementType MovementMode;
	UPROPERTY(BlueprintReadOnly, Category = "States")
		ERotationMode RotationMode;
	UPROPERTY(BlueprintReadOnly, Category = "States")
		EMovementState MovementState;
	UPROPERTY(BlueprintReadOnly, Category = "States")
		EGaits Gait;
	UPROPERTY(BlueprintReadOnly, Category = "States")
		EStance Stance;

	UPROPERTY(BlueprintReadOnly, Category = "States")
		EMovementType MovementModeLastFrame;
	UPROPERTY(BlueprintReadOnly, Category = "States")
		ERotationMode RotationModeLastFrame;
	UPROPERTY(BlueprintReadOnly, Category = "States")
		EMovementState MovementStateLastFrame;
	UPROPERTY(BlueprintReadOnly, Category = "States")
		EGaits GaitLastFrame;
	UPROPERTY(BlueprintReadOnly, Category = "States")
		EStance StanceLastFrame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Motion Matching")
		const UChooserTable* PoseSearchDatabases;
	UPROPERTY(BlueprintReadOnly, Category = "Motion Matching")
		float OrientationAlpha = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Motion Matching")
		float AnimTime = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Motion Matching")
		UAnimationAsset* AnimAsset = nullptr;
		
};
