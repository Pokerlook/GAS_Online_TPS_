// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GT/GTTypes.h"
#include "GTGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GT_API UGTGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//	UDataTable* ItemDT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Select)
		EGameMode SelectedGameMode = EGameMode::FreeForAll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Select)
		EGameMap SelectedGameMap = EGameMap::Map1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Team)
		ETeam Team = ETeam::ET_NoTeam;
};
