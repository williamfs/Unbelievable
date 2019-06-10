// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mode_Extractor.generated.h"

UCLASS()
class UNBELIEVABLE_API AMode_Extractor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMode_Extractor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Switch Components")
		int datapoint;

	UFUNCTION(BlueprintNativeEvent, Category = "Switch Components")
		void setDataPoint(int h);
	void setDataPoint_Implementation(int h);


};
