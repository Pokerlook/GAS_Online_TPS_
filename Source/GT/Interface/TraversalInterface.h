// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TraversalInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTraversalInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GT_API ITraversalInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION()
		virtual void SetTraversalTransform(FTransform InTransform) {}
	UFUNCTION()
		virtual FTransform GetTraversalTransform() { return FTransform(); }


	//UFUNCTION()
	//	virtual void PlayTraversalMontage(UAnimMontage* MontageToPlay, float PlayRate, float StartTime) {}
};
