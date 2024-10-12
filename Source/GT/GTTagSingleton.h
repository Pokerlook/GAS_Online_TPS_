// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"


struct FGTGameplayTags
{
public:
	static const FGTGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();	// in AssetManager

	/*
		InputTags
	*/
	FGameplayTag InputTag_Jump;		// space
	FGameplayTag InputTag_Crouch;	// c
	FGameplayTag InputTag_Sneek;	// left ctrl
	FGameplayTag InputTag_Sprint;	// left shift
	FGameplayTag InputTag_Fire;		// lmb
	FGameplayTag InputTag_Aim;		// rmb
	FGameplayTag InputTag_Reload;	// r
	FGameplayTag InputTag_HolsterWeapon;	// x
	FGameplayTag InputTag_Wheel;	//  mouse wheel
	FGameplayTag InputTag_Weapon1;
	FGameplayTag InputTag_Weapon2;
	FGameplayTag InputTag_Weapon3;
	FGameplayTag InputTag_Weapon4;
	FGameplayTag InputTag_LeanLeft;		// q
	FGameplayTag InputTag_LeanRight;	// e
	FGameplayTag InputTag_Interact;		// f
	FGameplayTag InputTag_ShootMode;	// b
	FGameplayTag InputTag_ViewMode;	// v
	FGameplayTag InputTag_Traversal;	// g
	FGameplayTag InputTag_FreeLook;	// left alt

private:
	static FGTGameplayTags GameplayTags;
};