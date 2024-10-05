// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GT/Character/GTCharacterBase.h"
#include "GTCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class GT_API AGTCharacterPlayer : public AGTCharacterBase
{
	GENERATED_BODY()
	
public:
	AGTCharacterPlayer();

protected:

private:

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* FollowCamera;

};
