// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GT/GTTypes.h"
#include "TraversableActor.generated.h"

class USplineComponent;

UCLASS()
class GT_API ATraversableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATraversableActor();

	UFUNCTION(BlueprintCallable)
		USplineComponent* FindLedgeClosestToActor(FVector ActorLocation);

	UFUNCTION(BlueprintCallable)
		void GetLedgeTransform(FVector HitLocation, FVector ActorLocation, FTraversalCheckResult& TraversalParamInOut);
protected:

public:
	UPROPERTY(BlueprintReadWrite, Category = "Traversable")
		TArray<USplineComponent*> Ledges;
	UPROPERTY(BlueprintReadWrite, Category = "Traversable")
		TMap<USplineComponent*, USplineComponent*> OppositeLedges;
	UPROPERTY(BlueprintReadWrite, Category = "Traversable")
		float MinLedgeWidth = 60.f;

};
