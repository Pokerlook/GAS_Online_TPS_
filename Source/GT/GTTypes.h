// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

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
