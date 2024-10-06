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