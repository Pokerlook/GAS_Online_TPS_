// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GTTypes.generated.h"

UENUM(BlueprintType)
enum class EGameMode : uint8
{
	FreeForAll UMETA(DisplayName = "FreeForAll"),
	TeamMatch UMETA(DisplayName = "TeamMatch"),
	CaptureFlag UMETA(DisplayName = "CaptureFlag"),
	Bomb UMETA(DisplayName = "Bomb"),
	HostageRescue UMETA(DisplayName = "HostageRescue"),

	MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EGameMap : uint8
{
	Map1 UMETA(DisplayName = "Map1"),
	Map2 UMETA(DisplayName = "Map2"),

	MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class ETeam : uint8
{
	RedTeam UMETA(DisplayName = "RedTeam"),
	BlueTeam UMETA(DisplayName = "BlueTeam"),
	NoTeam UMETA(DisplayName = "NoTeam"),

	MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EGaits : uint8
{
	Sneeking      UMETA(DisplayName = "Sneeking"),
	Jogging      UMETA(DisplayName = "Jogging"),
	Sprinting    UMETA(DisplayName = "Sprinting")
};

UENUM(BlueprintType)
enum class EMovementType : uint8
{
	OnGround     UMETA(DisplayName = "OnGround"),
	InAir        UMETA(DisplayName = "InAir")
};

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Idle         UMETA(DisplayName = "Idle"),
	Moving       UMETA(DisplayName = "Moving")
};

UENUM(BlueprintType)
enum class ERotationMode : uint8
{
	OrientToMovement     UMETA(DisplayName = "OrientToRotation"),
	Strafe               UMETA(DisplayName = "Strafe")
};

UENUM(BlueprintType)
enum class EStance : uint8
{
	Stand         UMETA(DisplayName = "Stand"),
	Crouch        UMETA(DisplayName = "Crouch")
};

UENUM(BlueprintType)
enum class ETraversalActionType : uint8
{
	None          UMETA(DisplayName = "None"),
	Hurdle        UMETA(DisplayName = "Hurdle"),
	Vault         UMETA(DisplayName = "Vault"),
	Mantle        UMETA(DisplayName = "Mantle")
};


USTRUCT(BlueprintType)
struct FTraversalCheckResult : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = "Traversal Check Result")
		ETraversalActionType ActionType;

	UPROPERTY(BlueprintReadWrite, Category = "Traversal Check Result")
		bool HasFrontLedge = false;
	UPROPERTY(BlueprintReadWrite, Category = "Traversal Check Result")
		FVector FrontLedgeLocation = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, Category = "Traversal Check Result")
		FVector FrontLedgeNormal = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = "Traversal Check Result")
		bool HasBackLedge = false;
	UPROPERTY(BlueprintReadWrite, Category = "Traversal Check Result")
		FVector BackLedgeLocation = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite, Category = "Traversal Check Result")
		FVector BackLedgeNormal = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = "Traversal Check Result")
		bool HasBackFloor = false;
	UPROPERTY(BlueprintReadWrite, Category = "Traversal Check Result")
		FVector BackFloorLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = "Traversal Check Result")
		float ObstacleHeight = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = "Traversal Check Result")
		float ObstacleDepth = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = "Traversal Check Result")
		float BackLedgeHeight = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Traversal Check Result")
		UPrimitiveComponent* HitComponent = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Traversal Check Result")
		const UAnimMontage* ChosenMontage = nullptr;
	UPROPERTY(BlueprintReadWrite, Category = "Traversal Check Result")
		float StartTime = 0.f;
	UPROPERTY(BlueprintReadWrite, Category = "Traversal Check Result")
		float PlayRate = 0.f;

	UPROPERTY(BlueprintReadWrite, Category = "Traversal Check Result")
		float Speed = 0.f;
};