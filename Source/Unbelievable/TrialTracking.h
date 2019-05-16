// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrialTracking.generated.h"

UCLASS()
class UNBELIEVABLE_API ATrialTracking : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrialTracking();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category = "Speed of follow")
		float projectileSpeed;
	void UpdateXposition(FVector projectileLocation, FVector playerposition);
	void UpdateYPosition(FVector projectileLocation, FVector playerposition);
	void UpdateZPosition(FVector projectileLocation, FVector playerposition);

};
