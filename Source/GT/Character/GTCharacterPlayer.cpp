// Fill out your copyright notice in the Description page of Project Settings.


#include "GT/Character/GTCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GT/AbilitySystem/GTAbilitySystemComponent.h"

AGTCharacterPlayer::AGTCharacterPlayer()
{
	// Configure character movement	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	bUseControllerRotationYaw = true; 
	GetCharacterMovement()->bOrientRotationToMovement = false;
//	GetCharacterMovement()->bUseControllerDesiredRotation = false; // false? true?
//	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
//	GetCharacterMovement()->SetCrouchedHalfHeight(70.f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 150.f;	// if zoom, 75. zoom mode, change camera?
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

}

void AGTCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// Init ability actor info for the Server
	InitAbilityActorInfo();
}

void AGTCharacterPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// Init ability actor info for the Client
	InitAbilityActorInfo();
}
