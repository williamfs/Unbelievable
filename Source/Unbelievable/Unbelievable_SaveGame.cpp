// Fill out your copyright notice in the Description page of Project Settings.

#include "Unbelievable_SaveGame.h"
#include "Unbelievable.h"

UUnbelievable_SaveGame::UUnbelievable_SaveGame()
{
	PlayerLocation = FVector(0.f, 0.f, 0.f);
	gameTimeTotal = 0.0f;
	dateOfLastPlay = FString(TEXT("6/17/2019"));
	modeOfPlay = FString(TEXT("Exploration"));
	numberOfDeathes = 0;
}
