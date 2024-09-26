// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GT/GTTypes.h"
#include "GTPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GT_API AGTPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AGTPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Replicated)
		ETeam Team = ETeam::NoTeam;
private:
};
