// Fill out your copyright notice in the Description page of Project Settings.


#include "GT/Game/GTGameMode.h"
#include "GTGameState.h"
#include "GT/Player/GTPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GT/Player/GTPlayerController.h"


void AGTGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	AGTGameState* GTGameState = Cast<AGTGameState>(UGameplayStatics::GetGameState(this));
	if (GTGameState)
	{
		AGTPlayerState* GTPState = NewPlayer->GetPlayerState<AGTPlayerState>();
		if (GTGameState && GTPState->Team == ETeam::NoTeam)
		{
			if (GTGameState->BlueTeam.Num() >= GTGameState->RedTeam.Num())
			{
				GTGameState->RedTeam.AddUnique(GTPState);
				GTPState->Team = ETeam::RedTeam;
			}
			else
			{
				GTGameState->BlueTeam.AddUnique(GTPState);
				GTPState->Team = ETeam::BlueTeam;
			}
		}
		GTGameState->TeamChangingNotify(GTGameState->RedTeam, GTGameState->BlueTeam);
		
		
	}

}

void AGTGameMode::Logout(AController* Exiting)
{
	AGTGameState* GTGameState = Cast<AGTGameState>(UGameplayStatics::GetGameState(this));
	AGTPlayerState* GTPState = Exiting->GetPlayerState<AGTPlayerState>();
	if (GTGameState && GTPState)
	{
		if (GTGameState->RedTeam.Contains(GTPState))
		{
			GTGameState->RedTeam.Remove(GTPState);
		}
		if (GTGameState->BlueTeam.Contains(GTPState))
		{
			GTGameState->BlueTeam.Remove(GTPState);
		}
		GTGameState->TeamChangingNotify(GTGameState->RedTeam, GTGameState->BlueTeam);
	}
	Super::Logout(Exiting);
}

void AGTGameMode::ChangeTeam(AGTPlayerState* PS, ETeam TeamtoChange)
{
	AGTGameState* GTGameState = Cast<AGTGameState>(UGameplayStatics::GetGameState(this));
	check(PS && GTGameState);

	//if (GTGameState->RedTeam.Contains(PS))
	//{
	//	GTGameState->RedTeam.Remove(PS);
	//}
	//if (GTGameState->BlueTeam.Contains(PS))
	//{
	//	GTGameState->BlueTeam.Remove(PS);
	//}

	//if (TeamtoChange == ETeam::RedTeam)
	//{
	//	GTGameState->RedTeam.AddUnique(PS);
	//}
	//else if (TeamtoChange == ETeam::BlueTeam)
	//{
	//	GTGameState->BlueTeam.AddUnique(PS);
	//}

	if (TeamtoChange == ETeam::RedTeam && !GTGameState->RedTeam.Contains(PS))
	{
		if (GTGameState->BlueTeam.Contains(PS))
		{
			GTGameState->BlueTeam.Remove(PS);
		}
		GTGameState->RedTeam.AddUnique(PS);		
	}
	else if (TeamtoChange == ETeam::BlueTeam && !GTGameState->BlueTeam.Contains(PS))
	{
		if (GTGameState->RedTeam.Contains(PS))
		{
			GTGameState->RedTeam.Remove(PS);
		}
		GTGameState->BlueTeam.AddUnique(PS);
	}

	PS->Team = TeamtoChange;

	GTGameState->TeamChangingNotify(GTGameState->RedTeam, GTGameState->BlueTeam);

	FString RedTeamMembers;
	// RedTeam �迭�� ��� ��Ҹ� ����
    for (AGTPlayerState* PlayerState : GTGameState->RedTeam)
    {
        if (PlayerState)
        {
            // PlayerName�� �����ͼ� RedTeamMembers ���ڿ��� �߰�
            RedTeamMembers.Append(PlayerState->GetPlayerName() + TEXT(", "));
        }
    }
    // �������� ���� ", " ���� (���ʿ��� ���)
    if (RedTeamMembers.Len() > 0)
    {
        RedTeamMembers.RemoveFromEnd(TEXT(", "));
    }
    // RedTeam �迭�� �ִ� �÷��̾���� �̸��� �� �ٷ� ���
    UE_LOG(LogTemp, Log, TEXT("Red Team Members: %s"), *RedTeamMembers);


    FString BlueTeamMembers;
    // BlueTeam �迭�� ��� ��Ҹ� ����
    for (AGTPlayerState* PlayerState : GTGameState->BlueTeam)
    {
        if (PlayerState)
        {
            // PlayerName�� �����ͼ� BlueTeamMembers ���ڿ��� �߰�
            BlueTeamMembers.Append(PlayerState->GetPlayerName() + TEXT(", "));
        }
    }
    // �������� ���� ", " ���� (���ʿ��� ���)
    if (BlueTeamMembers.Len() > 0)
    {
        BlueTeamMembers.RemoveFromEnd(TEXT(", "));
    }
    // BlueTeam �迭�� �ִ� �÷��̾���� �̸��� �� �ٷ� ���
    UE_LOG(LogTemp, Log, TEXT("Blue Team Members: %s"), *BlueTeamMembers);
}
