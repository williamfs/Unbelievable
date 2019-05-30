// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrialTracking.h"
#include "Portal_Projectile_SpawnPoint.generated.h"
UCLASS()
class UNBELIEVABLE_API APortal_Projectile_SpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal_Projectile_SpawnPoint();

protected:
	APortal_Projectile_SpawnPoint(const FObjectInitializer & ObjectInitializer);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float currentTime;							 // Holds the current alive time of GameObject

	UPROPERTY(EditAnywhere)
	FVector ourLoc;								 // Hold our spawn location

	FRotator ourRotation;                        // Hold our rotation of the object

	UPROPERTY(EditAnywhere, Category = "Our Spawning Object")
		TSubclassOf<ATrialTracking> ourSpawningObject; // Holds the blue prints of the object we want to spawn

	UPROPERTY(EditAnywhere, Category = "Rate in Seconds for Spawning")
		float spawnDelay;

};
