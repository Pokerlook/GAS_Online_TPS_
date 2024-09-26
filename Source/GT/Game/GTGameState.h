// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GT/GTTypes.h"
#include "GTGameState.generated.h"

class AGTPlayerState;

/**
 * 
 */
UCLASS()
class GT_API AGTGameState : public AGameState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Team")
		void TeamChangingNotify(const TArray<AGTPlayerState*> &InRedTeam, const TArray<AGTPlayerState*> &InBlueTeam);
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_RedTeam, Category = "Team")// 
		TArray<AGTPlayerState*> RedTeam;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_BlueTeam, Category = "Team")//  
		TArray<AGTPlayerState*> BlueTeam;
	
	UFUNCTION()
		void OnRep_RedTeam();
	UFUNCTION()
		void OnRep_BlueTeam();
};
