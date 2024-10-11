// Fill out your copyright notice in the Description page of Project Settings.


#include "TraversableActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SplineComponent.h"

ATraversableActor::ATraversableActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

USplineComponent* ATraversableActor::FindLedgeClosestToActor(FVector ActorLocation)
{
	if (Ledges.IsEmpty()) return nullptr;

	float ClosestDistance = 0.f;
	int32 ClosestIndex = 0;
	for (int i = 0; i < Ledges.Num(); i++)
	{
		float CurrentDistance = UKismetMathLibrary::Vector_Distance(
			(Ledges[i]->FindLocationClosestToWorldLocation(ActorLocation, ESplineCoordinateSpace::World) +
				Ledges[i]->FindUpVectorClosestToWorldLocation(ActorLocation, ESplineCoordinateSpace::World) * 10.f), 
			ActorLocation);

		if (CurrentDistance < ClosestDistance || i == 0)
		{
			ClosestDistance = CurrentDistance;
			ClosestIndex = i;
		}
	}
	return Ledges[ClosestIndex];
}

void ATraversableActor::GetLedgeTransform(FVector HitLocation, FVector ActorLocation, FTraversalCheckResult& TraversalParamInOut)
{
	USplineComponent* ClosestLedge = FindLedgeClosestToActor(ActorLocation);
	if (!IsValid(ClosestLedge))
	{
		TraversalParamInOut.HasFrontLedge = false;
		return;
	}

	if (ClosestLedge->GetSplineLength() >= MinLedgeWidth)
	{
		float ClampingValue = ClosestLedge->GetDistanceAlongSplineAtLocation(ClosestLedge->
			FindLocationClosestToWorldLocation(HitLocation, ESplineCoordinateSpace::Local)
			, ESplineCoordinateSpace::Local);
		float Distance = UKismetMathLibrary::
			FClamp(ClampingValue, MinLedgeWidth / 2.f, ClosestLedge->GetSplineLength() - MinLedgeWidth / 2.f);
		FTransform NewTransform = ClosestLedge->GetTransformAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

		TraversalParamInOut.HasFrontLedge = true;
		TraversalParamInOut.FrontLedgeLocation = NewTransform.GetLocation();
		TraversalParamInOut.FrontLedgeNormal = UKismetMathLibrary::GetUpVector(NewTransform.Rotator());


		USplineComponent* OppositeLedge = *OppositeLedges.Find(ClosestLedge);
		if (!IsValid(OppositeLedge))
		{
			TraversalParamInOut.HasBackLedge = false;
			return;
		}

		FTransform NewTransform2 = OppositeLedge->FindTransformClosestToWorldLocation(TraversalParamInOut.FrontLedgeLocation, 
			ESplineCoordinateSpace::World, false);

		TraversalParamInOut.HasBackLedge = true;
		TraversalParamInOut.BackLedgeLocation = NewTransform2.GetLocation();
		TraversalParamInOut.BackLedgeNormal = UKismetMathLibrary::GetUpVector(NewTransform2.Rotator());

		return;
	}
	else
	{
		{
			TraversalParamInOut.HasFrontLedge = false;
			return;
		}
	}

}
