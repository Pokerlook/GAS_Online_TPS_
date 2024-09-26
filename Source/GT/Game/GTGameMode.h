// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GTGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GT_API AGTGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override; 
//	virtual void SwapPlayerControllers(APlayerController* OldPC, APlayerController* NewPC) override;

	UFUNCTION(BlueprintCallable, Category = "Team")
		void ChangeTeam(class AGTPlayerState* PS, ETeam TeamtoChange);

	//UFUNCTION(BlueprintImplementableEvent, Category = "Team")
	//	void TeamChangingNotify(const TArray<AGTPlayerState*>& InRedTeam, const TArray<AGTPlayerState*>& InBlueTeam);


};
