// Fill out your copyright notice in the Description page of Project Settings.

#include "Portal_Projectile_SpawnPoint.h"
#include "Engine.h"
// Sets default values
APortal_Projectile_SpawnPoint::APortal_Projectile_SpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

APortal_Projectile_SpawnPoint::APortal_Projectile_SpawnPoint(const FObjectInitializer & ObjectInitializer):Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	currentTime = 0.f;

	

}

// Called when the game starts or when spawned
void APortal_Projectile_SpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("We are alive"));
}

// Called every frame
void APortal_Projectile_SpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currentTime = currentTime + 1 * DeltaTime;

	FString ourString = FString::SanitizeFloat(currentTime);
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Our Current Time: ") + ourString);

	if (currentTime >= spawnDelay)
	{
		

		// Create Spawn params
		FActorSpawnParameters spawnParams;

		// who did the spawning
		spawnParams.Owner = this;
		spawnParams.Instigator = Instigator;

		// currentTime back to 0
		currentTime = 0.f;

		ATrialTracking* ourNewObject = GetWorld()->SpawnActor<ATrialTracking>(ourSpawningObject, ourLoc, ourRotation, spawnParams);
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("We spawned an object. "));
	}
}

