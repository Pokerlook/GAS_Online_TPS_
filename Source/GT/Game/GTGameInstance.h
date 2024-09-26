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

	UFUNCTION(BlueprintCallable, Category = "MainMenu")
		void ClientReturnToMainMenu();

public:

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//	UDataTable* ItemDT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EGameMode SelectedGameMode = EGameMode::FreeForAll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EGameMap SelectedGameMap = EGameMap::Map1;


};
