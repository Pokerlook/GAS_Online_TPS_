// Fill out your copyright notice in the Description page of Project Settings.


#include "GT/Character/GTCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

AGTCharacterBase::AGTCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

}

void AGTCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGTCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGTCharacterBase::ToggleSprint(bool Newbool)
{
}

void AGTCharacterBase::ToggleCrouch(bool Newbool)
{
}

void AGTCharacterBase::ToggleStrafe(bool Newbool)
{
}

void AGTCharacterBase::ToggleAim(bool Newbool)
{
}
