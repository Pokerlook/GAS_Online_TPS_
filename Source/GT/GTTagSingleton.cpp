// Fill out your copyright notice in the Description page of Project Settings.


#include "GTTagSingleton.h"
#include "GameplayTagsManager.h"

FGTGameplayTags FGTGameplayTags::GameplayTags;

void FGTGameplayTags::InitializeNativeGameplayTags()
{
	/// Input
	GameplayTags.InputTag_Jump = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Jump"),
		FString("Input Tag for Jump, Default is Space")
	);
	GameplayTags.InputTag_Crouch = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Crouch"),
		FString("Input Tag for Crouch, Default is C")
	); GameplayTags.InputTag_Sneek = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Sneek"),
		FString("Input Tag for Sneek, Default is Left Ctrl")
	);
	GameplayTags.InputTag_Sprint = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Sprint"),
		FString("Input Tag for Sprint, Default is Left Shift")
	);
	GameplayTags.InputTag_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Fire"),
		FString("Input Tag for Fire, Default is Left Mouse Button")
	);
	GameplayTags.InputTag_Aim = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Aim"),
		FString("Input Tag for Aim, Default is Right Mouse Button")
	);
	GameplayTags.InputTag_Reload = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Reload"),
		FString("Input Tag for Reload, Default is R")
	);
	GameplayTags.InputTag_HolsterWeapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.HolsterWeapon"),
		FString("Input Tag for HolsterWeapon, Default is X")
	);
	GameplayTags.InputTag_Wheel = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Wheel"),
		FString("Input Tag for Wheel, Default is Mouse Wheel ")
	);
	GameplayTags.InputTag_Weapon1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Weapon1"),
		FString("Input Tag for Weapon1, Default is 1")
	);
	GameplayTags.InputTag_Weapon2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Weapon2"),
		FString("Input Tag for Weapon2, Default is 2")
	);
	GameplayTags.InputTag_Weapon3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Weapon3"),
		FString("Input Tag for Weapon3, Default is 3")
	);
	GameplayTags.InputTag_Weapon4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Weapon4"),
		FString("Input Tag for Weapon4, Default is 4")
	); GameplayTags.InputTag_LeanLeft = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LeanLeft"),
		FString("Input Tag for LeanLeft, Default is Q")
	);
	GameplayTags.InputTag_LeanRight = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LeanRight"),
		FString("Input Tag for LeanRight, Default is E")
	);
	GameplayTags.InputTag_Interact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Interact"),
		FString("Input Tag for Interact, Default is F")
	);
	GameplayTags.InputTag_ShootMode = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.ShootMode"),
		FString("Input Tag for ShootMode, Default is B")
	);
	GameplayTags.InputTag_ViewMode = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.ViewMode"),
		FString("Input Tag for ViewMode, Default is V")
	);
	GameplayTags.InputTag_Traversal = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Vault"),
		FString("Input Tag for Vault, Default is G")
	);
	GameplayTags.InputTag_FreeLook = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.FreeLook"),
		FString("Input Tag for FreeLook, Default is Left Alt")
	);


}
