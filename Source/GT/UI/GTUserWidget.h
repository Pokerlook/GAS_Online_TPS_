// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GTUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class GT_API UGTUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void RefreshWdiget();

	UFUNCTION(BlueprintImplementableEvent, Category = "HandleKey")
		void HandleEnterKey();
	UFUNCTION(BlueprintImplementableEvent, Category = "HandleKey")
		void HandleEscKey();

protected:
	virtual FReply NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

};
