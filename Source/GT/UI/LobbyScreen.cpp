// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyScreen.h"
#include "PlayerCard.h"
#include "GT/Player/GTPlayerState.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "AdvancedSessionsLibrary.h"
#include "AdvancedSteamFriendsLibrary.h"

ULobbyScreen::ULobbyScreen()
{
	ConstructorHelpers::FClassFinder<UUserWidget> 
		PLAYERCARD_CLASS(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprint/MenuLobby/Lobby/WBP_PlayerCard.WBP_PlayerCard_C'"));
	if (PLAYERCARD_CLASS.Class) PlayerCardClass = PLAYERCARD_CLASS.Class;

}

void ULobbyScreen::UpdateTeamChange(const TArray<AGTPlayerState*>& TeamRed, const TArray<AGTPlayerState*>& TeamBlue)
{
	if (!GetWorld() || GetWorld()->bIsTearingDown)
	{
		return;
	}
	SB_RedTeamList->ClearChildren();
	SB_BlueTeamList->ClearChildren();

	if (TeamRed.Num() > 0)
	{
		for (int i = 0; i < TeamRed.Num(); i++)
		{
			if (!IsValid(TeamRed[i])) break;
			FText PlayerName = FText::FromString(TeamRed[i]->GetPlayerName());

			FBPUniqueNetId NetID;
			UAdvancedSessionsLibrary::GetUniqueNetID(TeamRed[i]->GetPlayerController(), NetID);

			EBlueprintAsyncResultSwitch ResultSwitch;
			UTexture2D* Icon = UAdvancedSteamFriendsLibrary::GetSteamFriendAvatar(NetID, ResultSwitch, SteamAvatarSize::SteamAvatar_Medium);

			UPlayerCard* PlayerCardWidget = CreateWidget<UPlayerCard>(this, PlayerCardClass);
			PlayerCardWidget->PlayerName = PlayerName;
			PlayerCardWidget->SteamIcon = Icon;
			PlayerCardWidget->PlayerState = TeamRed[i];

			SB_RedTeamList->AddChild(PlayerCardWidget);
		}
	}
	
	if (TeamBlue.Num()>0)
	{
		for (int i = 0; i < TeamBlue.Num(); i++)
		{
			if (!IsValid(TeamBlue[i])) break;
			FText PlayerName = FText::FromString(TeamBlue[i]->GetPlayerName());

			FBPUniqueNetId NetID;
			UAdvancedSessionsLibrary::GetUniqueNetID(TeamBlue[i]->GetPlayerController(), NetID);

			EBlueprintAsyncResultSwitch ResultSwitch;
			UTexture2D* Icon = UAdvancedSteamFriendsLibrary::GetSteamFriendAvatar(NetID, ResultSwitch, SteamAvatarSize::SteamAvatar_Medium);

			UPlayerCard* PlayerCardWidget = CreateWidget<UPlayerCard>(this, PlayerCardClass);
			PlayerCardWidget->PlayerName = PlayerName;
			PlayerCardWidget->SteamIcon = Icon;
			PlayerCardWidget->PlayerState = TeamBlue[i];

			SB_BlueTeamList->AddChild(PlayerCardWidget);
		}
	}
	
	FString RedTeamText = FString::FromInt(TeamRed.Num()) + " / 8";
	FString BlueTeamText = FString::FromInt(TeamBlue.Num()) + " / 8";
	TXT_RedTeamNumber->SetText(FText::FromString(RedTeamText));
	TXT_BlueTeamNumber->SetText(FText::FromString(BlueTeamText));
}
