// Fill out your copyright notice in the Description page of Project Settings.

#include "Portal_Projectile_SpawnPoint.h"
#include "Engine/Engine.h"
#include "CoreMinimal.h"
#include "PlayerDeath.h"
#include "SpawnLocation.h"
#include "DrawDebugHelpers.h"
#include "UnbelievableCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObjectIterator.h"
#include "Engine.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
APortal_Projectile_SpawnPoint::APortal_Projectile_SpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Creates the Trigger area for collision
	//MyCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("My Sphere Component"));
	//MyCollisionSphere->InitSphereRadius(SphereRadius);
	//MyCollisionSphere->SetCollisionProfileName("Trigger");
	//RootComponent = MyCollisionSphere;
	//MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MY MESH"));
	//MyMesh->SetupAttachment(RootComponent);

	////Creates the Collision Detection dynamic
	//MyCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APortal_Projectile_SpawnPoint::OnOverLapBegin);
	//MyCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &APortal_Projectile_SpawnPoint::OnOverLapEnd);
}

APortal_Projectile_SpawnPoint::APortal_Projectile_SpawnPoint(const FObjectInitializer & ObjectInitializer):Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	currentTime = 0.f;

	MyCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("My Sphere Component"));
	MyCollisionSphere->InitSphereRadius(SphereRadius);
	MyCollisionSphere->SetCollisionProfileName("Trigger");
	RootComponent = MyCollisionSphere;
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MY MESH"));
	MyMesh->SetupAttachment(RootComponent);

	//Creates the Collision Detection dynamic
	MyCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APortal_Projectile_SpawnPoint::OnOverLapBegin);
	//MyCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &APortal_Projectile_SpawnPoint::OnOverLapEnd);

}

// Called when the game starts or when spawned
void APortal_Projectile_SpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("We are alive"));
	switch (levelSelected)
	{
		case 1: 
			spawnDelay = explorationSpawnDelay;
			isFreeRoam = true;
			break;
		case 2: 
			spawnDelay = coreSpawnDelay;
			break;
		case 3: 
			spawnDelay = hardSpawnDelay;
			break;
		case 4:
			spawnDelay = shadowWalkerSpawnDelay;
			break;
		default:
			spawnDelay = explorationSpawnDelay;
			isFreeRoam=true;
			break;
	}
}

// Called every frame
void APortal_Projectile_SpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currentTime = currentTime + 1 * DeltaTime;

	FString ourString = FString::SanitizeFloat(currentTime);
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Our Current Time: ") + ourString);

	if (currentTime >= spawnDelay && isFreeRoam==false)
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

void APortal_Projectile_SpawnPoint::OnOverLapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OverlapBegin has been called."));
	if ((OtherActor != nullptr) && (OtherActor != this) && OtherComp != nullptr)
	{
		if (OtherActor->GetName() == "FirstPersonCharacter2")
		{
			UE_LOG(LogTemp, Warning, TEXT("You are in range of the turret"));
			inRange = true;
		}
	}
}

void APortal_Projectile_SpawnPoint::OnOverLapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && OtherComp != nullptr)
	{
		if (OtherActor->GetName() == "FirstPersonCharacter2")
		{
			UE_LOG(LogTemp, Warning, TEXT("You are not in range of the turret"));
			inRange = false;
		}
	}
}

