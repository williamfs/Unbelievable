// Fill out your copyright notice in the Description page of Project Settings.

#include "Data_handler_For_File_System.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
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
	//SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::LoadGameFromSlot("MySlot", 0));
	switch (index)
	{
	case 0:
		SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::LoadGameFromSlot("MySlot", 0));
		break;
	case 1:
		SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::LoadGameFromSlot("MySlot1", 0));
		break;
	case 2:
		SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::LoadGameFromSlot("MySlot2", 0));
		break;
	case 3:
		SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::LoadGameFromSlot("MySlot3", 0));
		break;
	case 4:
		SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::LoadGameFromSlot("MySlot4", 0));
		break;
	}
	//this->SetActorLocation(SaveGameInstance->PlayerLocation);
	gameTimeForFile=SaveGameInstance->gameTimeTotal;
	dateForFile = SaveGameInstance->dateOfLastPlay;
	modeForFile = SaveGameInstance->modeOfPlay;
	deathsForFile = SaveGameInstance->numberOfDeathes;
	levelForFile = SaveGameInstance->levelPlayed;
	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("Loaded Game For Main Menu"));
}