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

	UFUNCTION(NetMulticast, Reliable)
		void MulticastTeamChangingNotify(const TArray<AGTPlayerState*>& TeamRed, const TArray<AGTPlayerState*>& TeamBlue);

	UFUNCTION(BlueprintImplementableEvent, Category = "Team")	// Only Used(implemented) at Lobby... (at LobbyGameState)
		void TeamChangingNotify(const TArray<AGTPlayerState*> &TeamRed, const TArray<AGTPlayerState*> &TeamBlue);
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_Team, Category = "Team")// 
		TArray<AGTPlayerState*> RedTeam;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, ReplicatedUsing = OnRep_Team, Category = "Team")//  
		TArray<AGTPlayerState*> BlueTeam;
	
	UFUNCTION()
		void OnRep_Team();
};
