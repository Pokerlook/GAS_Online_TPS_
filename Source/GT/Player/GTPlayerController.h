// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GTPlayerController.generated.h"

class Character;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class GT_API AGTPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGTPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void SetupInputComponent() override;

private:
	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);

	// test, will be used with GAS
	void OnJumpStarted(const FInputActionValue& InputActionValue);
	void OnJumpEnded(const FInputActionValue& InputActionValue);
	void OnSprintStarted(const FInputActionValue& InputActionValue);
	void OnSprintEnded(const FInputActionValue& InputActionValue);
	void OnSneekStarted(const FInputActionValue& InputActionValue);
	void OnSneekEnded(const FInputActionValue& InputActionValue);
	void ToggleCrouch(const FInputActionValue& InputActionValue);
	void TryTraversal(const FInputActionValue& InputActionValue);

private:

	UPROPERTY(EditAnywhere, Category = "Input")
		TObjectPtr<UInputMappingContext> IMC;

	UPROPERTY(EditAnywhere, Category = "Input")
		TObjectPtr<UInputAction> MoveAction;	// WS DA

	UPROPERTY(EditAnywhere, Category = "Input")
		TObjectPtr<UInputAction> LookAction;	// Mouse

	UPROPERTY(EditAnywhere, Category = "Input")
		TObjectPtr<UInputAction> JumpAction;	// Space

	UPROPERTY(EditAnywhere, Category = "Input")
		TObjectPtr<UInputAction> SprintAction;	// Left Shift

	UPROPERTY(EditAnywhere, Category = "Input")
		TObjectPtr<UInputAction> SneekAction;	// Left Ctrl

	UPROPERTY(EditAnywhere, Category = "Input")
		TObjectPtr<UInputAction> CrouchAction;	// C

	UPROPERTY(EditAnywhere, Category = "Input")
		TObjectPtr<UInputAction> TraversalAction;	// G

};
