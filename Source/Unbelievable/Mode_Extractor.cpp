// Fill out your copyright notice in the Description page of Project Settings.

#include "Mode_Extractor.h"

// Sets default values
AMode_Extractor::AMode_Extractor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMode_Extractor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMode_Extractor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	 UE_LOG(LogTemp, Warning, TEXT("playerposition is %d"), datapoint);
}

void AMode_Extractor::setDataPoint_Implementation(int h)
{
	datapoint = h;
}

