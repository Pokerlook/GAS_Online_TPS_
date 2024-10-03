// Fill out your copyright notice in the Description page of Project Settings.


#include "GT/Character/GTCharacterBase.h"

// Sets default values
AGTCharacterBase::AGTCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGTCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGTCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGTCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

