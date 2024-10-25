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

	virtual void PossessedBy(AController* NewController) override;
	//virtual void OnRep_PlayerState() override; // PS에 ASC가 있는게 아니니까 괜찮을 것 같은데, 클라이언트에서 문제 있으면 여기서도 InitAbilityActorInfo

protected:

private:

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* FollowCamera;

};
