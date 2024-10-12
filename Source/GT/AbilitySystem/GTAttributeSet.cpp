// Fill out your copyright notice in the Description page of Project Settings.


#include "GT/AbilitySystem/GTAttributeSet.h"
#include "Net/UnrealNetwork.h"

UGTAttributeSet::UGTAttributeSet()
{
}

void UGTAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGTAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGTAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UGTAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGTAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UGTAttributeSet, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGTAttributeSet, MaxShield, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UGTAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGTAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UGTAttributeSet, HeadArmor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGTAttributeSet, BodyArmor, COND_None, REPNOTIFY_Always);
}

void UGTAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGTAttributeSet, Health, OldHealth);
}

void UGTAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGTAttributeSet, MaxHealth, OldMaxHealth);
}

void UGTAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldShield) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGTAttributeSet, Shield, OldShield);
}

void UGTAttributeSet::OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGTAttributeSet, MaxShield, OldMaxShield);
}

void UGTAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGTAttributeSet, Stamina, OldStamina);
}

void UGTAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGTAttributeSet, MaxStamina, OldMaxStamina);
}

void UGTAttributeSet::OnRep_HeadArmor(const FGameplayAttributeData& OldHeadArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGTAttributeSet, HeadArmor, OldHeadArmor);
}

void UGTAttributeSet::OnRep_BodyArmor(const FGameplayAttributeData& OldBodyArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGTAttributeSet, BodyArmor, OldBodyArmor);
}
