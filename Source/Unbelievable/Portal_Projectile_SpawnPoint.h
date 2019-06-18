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

	UPROPERTY(VisibleAnywhere,Category="Collision Stuff")
	bool inRange;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* MyCollisionSphere;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* MyMesh;

	UPROPERTY(EditAnywhere)
		float SphereRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inherited Level Selection")
		int levelSelected;

	UPROPERTY(EditAnywhere)
	FVector ourLoc;								 // Hold our spawn location

	FRotator ourRotation;                        // Hold our rotation of the object

	UPROPERTY(EditAnywhere, Category = "Our Spawning Object")
		TSubclassOf<ATrialTracking> ourSpawningObject; // Holds the blue prints of the object we want to spawn

		float spawnDelay;

	UPROPERTY(EditAnywhere, Category = "Rate in Seconds for Spawning")
		float explorationSpawnDelay;

	UPROPERTY(EditAnywhere, Category = "Rate in Seconds for Spawning")
		float coreSpawnDelay;
	
	UPROPERTY(EditAnywhere, Category = "Rate in Seconds for Spawning")
		float hardSpawnDelay;

	UPROPERTY(EditAnywhere, Category = "Rate in Seconds for Spawning")
		float shadowWalkerSpawnDelay;

	UPROPERTY(EditAnywhere, Category = "Free Roam or Bullets?")
		bool isFreeRoam;
	UFUNCTION()
		void OnOverLapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverLapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
