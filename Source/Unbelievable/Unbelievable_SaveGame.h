// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Unbelievable_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class UNBELIEVABLE_API UUnbelievable_SaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UUnbelievable_SaveGame();

	UPROPERTY(EditAnywhere)
		FVector PlayerLocation;
};
