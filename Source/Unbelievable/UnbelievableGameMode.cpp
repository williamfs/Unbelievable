// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UnbelievableGameMode.h"
#include "UnbelievableHUD.h"
#include "UnbelievableCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnbelievableGameMode::AUnbelievableGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AUnbelievableHUD::StaticClass();
}
