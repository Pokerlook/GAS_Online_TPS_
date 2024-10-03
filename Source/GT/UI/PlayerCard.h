// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GTUserWidget.h"
#include "PlayerCard.generated.h"

/**
 * 
 */
UCLASS()
class GT_API UPlayerCard : public UGTUserWidget
{
	GENERATED_BODY()
	
public:

    UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ExposeOnSpawn = true) )
        FText PlayerName;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ExposeOnSpawn = true) )
        UTexture2D* SteamIcon;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Meta = (ExposeOnSpawn = true) )
        APlayerState* PlayerState;
};
