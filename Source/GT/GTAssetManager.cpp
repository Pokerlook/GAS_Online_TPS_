// Fill out your copyright notice in the Description page of Project Settings.


#include "GTAssetManager.h"
#include "GT/GTTagSingleton.h"
#include "AbilitySystemGlobals.h"

UGTAssetManager& UGTAssetManager::Get()
{
	check(GEngine);
	UGTAssetManager* GTAssetManager = Cast<UGTAssetManager>(GEngine->AssetManager);
	return *GTAssetManager;
}

void UGTAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();



	// This is required to use Target Data!
	UAbilitySystemGlobals::Get().InitGlobalData();
}
