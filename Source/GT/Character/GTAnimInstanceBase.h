// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "GTAnimInstanceBase.generated.h"

USTRUCT()
struct FGTAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

protected:

	virtual void InitializeObjects(UAnimInstance* InAnimInstance) override;
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
	virtual void Update(float DeltaSeconds) override;

public:

	UPROPERTY(Transient)
		APawn* Owner;
	UPROPERTY(Transient)
		class AGTCharacterBase* Character;
	UPROPERTY(Transient)
		class UCharacterMovementComponent* MovementComponent;

};

/**
 * 
 */
UCLASS()
class GT_API UGTAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY(Transient)
		FGTAnimInstanceProxy Proxy;
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override { return &Proxy; }
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {}

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

};
