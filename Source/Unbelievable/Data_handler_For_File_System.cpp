// Fill out your copyright notice in the Description page of Project Settings.

#include "Data_handler_For_File_System.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformFile.h"
#include "HAL/PlatformFilemanager.h"
#include "Unbelievable_SaveGame.h"
#include "Engine/Engine.h"

// Sets default values
AData_handler_For_File_System::AData_handler_For_File_System()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AData_handler_For_File_System::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AData_handler_For_File_System::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AData_handler_For_File_System::LoadGame(int index)
{
	UUnbelievable_SaveGame* SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::CreateSaveGameObject(UUnbelievable_SaveGame::StaticClass()));
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	switch (index)
	{
	case 0:
		//filePath = FString("C:/Users/Brennan Kersey/Desktop/Dev Work/Unbelievable file location/Unbelievable/Saved/SaveGames/MySlot.sav");
		SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::LoadGameFromSlot("MySlot0", 0));
		break;
	case 1:
		//filePath = FString("C:/Users/Brennan Kersey/Desktop/Dev Work/Unbelievable file location/Unbelievable/Saved/SaveGames/MySlot1.sav");
		SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::LoadGameFromSlot("MySlot1", 0));
		break;
	case 2:
		//filePath = FString("C:/Users/Brennan Kersey/Desktop/Dev Work/Unbelievable file location/Unbelievable/Saved/SaveGames/MySlot2.sav");
		SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::LoadGameFromSlot("MySlot2", 0));
		break;
	case 3:
		//filePath = FString("C:/Users/Brennan Kersey/Desktop/Dev Work/Unbelievable file location/Unbelievable/Saved/SaveGames/MySlot3.sav");
		SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::LoadGameFromSlot("MySlot3", 0));
		break;
	case 4:
		//filePath = FString("C:/Users/Brennan Kersey/Desktop/Dev Work/Unbelievable file location/Unbelievable/Saved/SaveGames/MySlot4.sav");
		SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::LoadGameFromSlot("MySlot4", 0));
		//IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		//filePath = FString("C:/Users/Brennan Kersey/Desktop/Dev Work/Unbelievable file location/Unbelievable/Saved/SaveGames/MySlot4.sav");
		
		break;
	}
	//this->SetActorLocation(SaveGameInstance->PlayerLocation);
	gameTimeForFile=SaveGameInstance->gameTimeTotal;
	dateForFile = SaveGameInstance->dateOfLastPlay;
	modeForFile = SaveGameInstance->modeOfPlay;
	deathsForFile = SaveGameInstance->numberOfDeathes;
	levelForFile = SaveGameInstance->levelPlayed;
	calculateStringForTime();
	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("Loaded Game For Main Menu"));
}

void AData_handler_For_File_System::calculateStringForTime()
{
	seconds = ((int)gameTimeForFile) % 60;
	minutes = (int)(gameTimeForFile / 60) % 60;
	hours = (int)(gameTimeForFile / 3600) % 24;

	stringOfTime = "";
	if (hours < 10)
	{
		stringOfTime.Append("0");
	}
	stringOfTime.Append(FString::FromInt(hours));
	stringOfTime.Append(":");
	if (minutes < 10)
	{
		stringOfTime.Append("0");
	}
	stringOfTime.Append(FString::FromInt(minutes));
	stringOfTime.Append(":");
	if (seconds < 10)
	{
		stringOfTime.Append("0");
	}
	stringOfTime.Append(FString::FromInt(seconds));

}
