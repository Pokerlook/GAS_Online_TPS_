// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GT/GTTypes.h"
#include "GameFramework/Character.h"
#include "GTCharacterBase.generated.h"

USTRUCT(BlueprintType)
struct FGaitSettings : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxWalkSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxAcceleration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BrakingDeceleration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BrakingFrictionFactor = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool UseSeperateBrakingFriction = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BrakingFriction = 0.f;

};

UCLASS()
class GT_API AGTCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AGTCharacterBase();

	virtual void Tick(float DeltaTime) override;
	virtual void Jump() override;

	UFUNCTION(BlueprintCallable)
		void ToggleSneek(bool Newbool);

	UFUNCTION(BlueprintCallable)
		void ToggleSprint(bool Newbool);

	UFUNCTION(BlueprintCallable)
		void ToggleCrouch();

	UFUNCTION(BlueprintCallable)
		void ToggleStrafe(bool Newbool);

	UFUNCTION(BlueprintCallable)
		void ToggleAim(bool Newbool);

	UFUNCTION(BlueprintImplementableEvent)
		void PlayJumpLandAudio(float NewVolume, bool Jumped);

	// Traversal
	void TryTraversal(); 
	float GetTraversalForwardTraceDistance() const; 
	void TraversalUpdateWarpTargets();
	void PerformTraversalAction(FTraversalCheckResult TraversalCheckResult);

protected:
	virtual void BeginPlay() override;
	virtual void Landed(const FHitResult& Hit) override;

	float CalculateMaxSpeed() const;

private:


public:
	UPROPERTY(BlueprintReadOnly, Category = "Traversal")
		FTraversalCheckResult TraversalResult;
	UPROPERTY(BlueprintReadOnly, Category = "Traversal")
		bool DoingTraversalAction = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Traversal")
		const class UChooserTable* TraversalTable;


	UPROPERTY(BlueprintReadOnly, Category = "Character")
		EMovementType MovementMode;
	UPROPERTY(BlueprintReadOnly, Category = "Character")
		EGaits Gait = EGaits::Jogging;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
		bool bWantsToAim = false;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
		bool bWantsToStrafe = false;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
		bool bWantsToRun = false;

	UPROPERTY(BlueprintReadWrite, Category = "Character")
		bool JustLanded = false;

	UPROPERTY(BlueprintReadWrite, Category = "Character")
		FVector LandVelocity = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = "Character")
		UCurveFloat* StrafeSpeedCurve = nullptr;


	UPROPERTY(BlueprintReadWrite, Category = "Character")
		FVector WalkSpeed = FVector(250.f, 200.f, 175.f);

	UPROPERTY(BlueprintReadWrite, Category = "Character")
		FVector JogSpeed = FVector(500.f, 350.f, 300.f);

	UPROPERTY(BlueprintReadWrite, Category = "Character")
		FVector SprintSpeed = FVector(700.f, 700.f, 700.f);

	UPROPERTY(BlueprintReadWrite, Category = "Character")
		FVector CrouchSpeed = FVector(250.f, 200.f, 175.f);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = true))
		TMap<EGaits, FGaitSettings> GaitSettings;
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation")
		class UMotionWarpingComponent* MotionWarpingComponent;

private:

};
