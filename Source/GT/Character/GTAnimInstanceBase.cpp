// Fill out your copyright notice in the Description page of Project Settings.


#include "GT/Character/GTAnimInstanceBase.h"
#include "GTCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void FGTAnimInstanceProxy::InitializeObjects(UAnimInstance* InAnimInstance)
{
	FAnimInstanceProxy::InitializeObjects(InAnimInstance);

	Owner = InAnimInstance->TryGetPawnOwner();
	if (!IsValid(Owner))return;

	Character = Cast<AGTCharacterBase>(Owner);
	MovementComponent = Cast<UCharacterMovementComponent>(Owner->GetMovementComponent());
}

void FGTAnimInstanceProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
	FAnimInstanceProxy::PreUpdate(InAnimInstance, DeltaSeconds);
}

void FGTAnimInstanceProxy::Update(float DeltaSeconds)
{
	FAnimInstanceProxy::Update(DeltaSeconds);
}

void UGTAnimInstanceBase::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
}

void UGTAnimInstanceBase::UpdateEssentialValues(float DeltaSeconds)
{
}

void UGTAnimInstanceBase::GenerateTrajectory(float DeltaSeconds)
{
}

void UGTAnimInstanceBase::UpdateStates()
{
}
