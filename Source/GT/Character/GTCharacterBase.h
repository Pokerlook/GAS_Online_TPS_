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

	UFUNCTION(BlueprintCallable)
		void ToggleSprint(bool Newbool);

	UFUNCTION(BlueprintCallable)
		void ToggleCrouch(bool Newbool);

	UFUNCTION(BlueprintCallable)
		void ToggleStrafe(bool Newbool);

	UFUNCTION(BlueprintCallable)
		void ToggleAim(bool Newbool);

protected:
	virtual void BeginPlay() override;


public:

	UPROPERTY(BlueprintReadOnly, Category = "Character")
		bool bWantsToStrafe = false;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
		bool bWantsToRun = false;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = true))
		TMap<EGaits, FGaitSettings> GaitSettings;

};
