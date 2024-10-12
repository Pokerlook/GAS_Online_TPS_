// Fill out your copyright notice in the Description page of Project Settings.


#include "GT/Player/GTPlayerState.h"
#include "Net/UnrealNetwork.h"

AGTPlayerState::AGTPlayerState()
{
	//NetUpdateFrequency = 100.f;
}

void AGTPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGTPlayerState, Team);
}
