// Fill out your copyright notice in the Description page of Project Settings.


#include "GT/Game/GTGameState.h"
#include "GT/Player/GTPlayerState.h"
#include "Net/UnrealNetwork.h"

void AGTGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGTGameState, RedTeam);
	DOREPLIFETIME(AGTGameState, BlueTeam);
}

void AGTGameState::OnRep_Team()
{
	TeamChangingNotify(RedTeam, BlueTeam);
}

