// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GTAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class GT_API UGTAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UGTAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Attribute

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGTAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
		FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGTAttributeSet, MaxHealth);


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Shield, Category = "Vital Attributes")
		FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UGTAttributeSet, Shield);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxShield, Category = "Vital Attributes")
		FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UGTAttributeSet, MaxShield);


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Vital Attributes")
		FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UGTAttributeSet, Stamina);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "Vital Attributes")
		FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UGTAttributeSet, MaxStamina);


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HeadArmor, Category = "Secondary Attributes")
		FGameplayAttributeData HeadArmor;
	ATTRIBUTE_ACCESSORS(UGTAttributeSet, HeadArmor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BodyArmor, Category = "Secondary Attributes")
		FGameplayAttributeData BodyArmor;
	ATTRIBUTE_ACCESSORS(UGTAttributeSet, BodyArmor);


	// OnRep_

	UFUNCTION()
		void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
		void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
		void OnRep_Shield(const FGameplayAttributeData& OldShield) const;
	UFUNCTION()
		void OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield) const;

	UFUNCTION()
		void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const;
	UFUNCTION()
		void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const;

	UFUNCTION()
		void OnRep_HeadArmor(const FGameplayAttributeData& OldHeadArmor) const;
	UFUNCTION()
		void OnRep_BodyArmor(const FGameplayAttributeData& OldBodyArmor) const;

};
