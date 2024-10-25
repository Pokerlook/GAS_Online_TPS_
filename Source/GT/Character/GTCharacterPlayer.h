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
	//virtual void OnRep_PlayerState() override; // PS�� ASC�� �ִ°� �ƴϴϱ� ������ �� ������, Ŭ���̾�Ʈ���� ���� ������ ���⼭�� InitAbilityActorInfo

protected:

private:

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* FollowCamera;

};
