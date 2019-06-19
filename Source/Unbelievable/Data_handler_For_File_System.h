// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data_handler_For_File_System.generated.h"

UCLASS()
class UNBELIEVABLE_API AData_handler_For_File_System : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AData_handler_For_File_System();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float gameTimeForFile;

	//UPROPERTY(EditAnywhere,)
};