// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GTUserWidget.h"
#include "LobbyScreen.generated.h"

class UScrollBox;
class UTextBlock;
class AGTPlayerState;
/**
 * 
 */
UCLASS()
class GT_API ULobbyScreen : public UGTUserWidget
{
	GENERATED_BODY()
public:
	ULobbyScreen();

	UFUNCTION(BlueprintCallable)
		void UpdateTeamChange(const TArray<AGTPlayerState*>& TeamRed, const TArray<AGTPlayerState*>& TeamBlue);

protected:

	UPROPERTY(EditAnywhere, Category = "PlayerCard")
		TSubclassOf<UUserWidget> PlayerCardClass;

private:

	UPROPERTY(Meta = (BindWidget))
		UScrollBox* SB_RedTeamList;
	UPROPERTY(Meta = (BindWidget))
		UScrollBox* SB_BlueTeamList;

	UPROPERTY(Meta = (BindWidget))
		UTextBlock* TXT_RedTeamNumber;
	UPROPERTY(Meta = (BindWidget))
		UTextBlock* TXT_BlueTeamNumber;


};
