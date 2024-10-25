// Fill out your copyright notice in the Description page of Project Settings.


#include "GT/Character/GTCharacterAI.h"

#include "GT/AbilitySystem/GTAbilitySystemComponent.h"

AGTCharacterAI::AGTCharacterAI()
{
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

void AGTCharacterAI::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();
}
