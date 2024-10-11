// Fill out your copyright notice in the Description page of Project Settings.


#include "GTPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "GT/Character/GTCharacterPlayer.h" // Testing... with GAS, erase

AGTPlayerController::AGTPlayerController()
{
	bReplicates = true;
}

void AGTPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(IMC);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
//	check(Subsystem);	// at client, null.
	if (Subsystem)
	{
		Subsystem->AddMappingContext(IMC, 0);
	}

}

void AGTPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AGTPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();	
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGTPlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGTPlayerController::Look);



	/////// tests, replace GAS binding
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AGTPlayerController::OnJumpStarted);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AGTPlayerController::OnJumpEnded);

	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AGTPlayerController::OnSprintStarted);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AGTPlayerController::OnSprintEnded);
	EnhancedInputComponent->BindAction(SneekAction, ETriggerEvent::Started, this, &AGTPlayerController::OnSneekStarted);
	EnhancedInputComponent->BindAction(SneekAction, ETriggerEvent::Completed, this, &AGTPlayerController::OnSneekEnded);

	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AGTPlayerController::ToggleCrouch);
	EnhancedInputComponent->BindAction(TraversalAction, ETriggerEvent::Started, this, &AGTPlayerController::TryTraversal);




}

void AGTPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	APawn* ControlledPawn = GetPawn<APawn>();
	if (IsValid(ControlledPawn))
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AGTPlayerController::Look(const FInputActionValue& InputActionValue)
{
	// maybe check is character alive

	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();
	AddYawInput(LookAxisVector.X);
	AddPitchInput(-LookAxisVector.Y);
}

void AGTPlayerController::OnJumpStarted(const FInputActionValue& InputActionValue)
{
	AGTCharacterPlayer* PlayerCharacter = GetPawn<AGTCharacterPlayer>();
	PlayerCharacter->Jump();
}

void AGTPlayerController::OnJumpEnded(const FInputActionValue& InputActionValue)
{
	AGTCharacterPlayer* PlayerCharacter = GetPawn<AGTCharacterPlayer>();
	PlayerCharacter->StopJumping();
}

void AGTPlayerController::OnSprintStarted(const FInputActionValue& InputActionValue)
{
	AGTCharacterPlayer* PlayerCharacter = GetPawn<AGTCharacterPlayer>();
	PlayerCharacter->ToggleSprint(true);
}

void AGTPlayerController::OnSprintEnded(const FInputActionValue& InputActionValue)
{
	AGTCharacterPlayer* PlayerCharacter = GetPawn<AGTCharacterPlayer>();
	PlayerCharacter->ToggleSprint(false);
}

void AGTPlayerController::OnSneekStarted(const FInputActionValue& InputActionValue)
{
	AGTCharacterPlayer* PlayerCharacter = GetPawn<AGTCharacterPlayer>();
	PlayerCharacter->ToggleSneek(true);
}

void AGTPlayerController::OnSneekEnded(const FInputActionValue& InputActionValue)
{
	AGTCharacterPlayer* PlayerCharacter = GetPawn<AGTCharacterPlayer>();
	PlayerCharacter->ToggleSneek(false);
}

void AGTPlayerController::ToggleCrouch(const FInputActionValue& InputActionValue)
{
	AGTCharacterPlayer* PlayerCharacter = GetPawn<AGTCharacterPlayer>();
	PlayerCharacter->ToggleCrouch();
}

void AGTPlayerController::TryTraversal(const FInputActionValue& InputActionValue)
{
	AGTCharacterPlayer* PlayerCharacter = GetPawn<AGTCharacterPlayer>();
	PlayerCharacter->TryTraversal();
}
