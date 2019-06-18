// Fill out your copyright notice in the Description page of Project Settings.

#include "TrialTracking.h"
#include "Engine/Engine.h"
#include "CoreMinimal.h"
#include "PlayerDeath.h"
#include "SpawnLocation.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObjectIterator.h"
#include "UnbelievableCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
ATrialTracking::ATrialTracking()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creates the Trigger area for collision
	MyCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("My Sphere Component"));
	MyCollisionSphere->InitSphereRadius(SphereRadius);
	MyCollisionSphere->SetCollisionProfileName("Trigger");
	RootComponent = MyCollisionSphere;
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MY MESH"));
	MyMesh->SetupAttachment(RootComponent);

	//Creates the Collision Detection dynamic
	MyCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATrialTracking::OnOverLapBegin);
	//MyCollisionSphere->OnComponentEndOverlap.AddDynamic(this, &ATrialTracking::OnOverLapEnd);
}

// Called when the game starts or when spawned
void ATrialTracking::BeginPlay()
{
	Super::BeginPlay();

	switch (levelSelected)
	{
		case 1:
			damageToDeal = 0;
			break;
		case 2:
			damageToDeal = coreModeDamage;
			break;
		case 3:
			damageToDeal = hardModeDamage;
			break;
		case 4: 
			damageToDeal = shadowModeDamage;
			break;
	}
	
}


void ATrialTracking::UpdateXposition(FVector projectileLocation, FVector playerposition)
{
	DrawDebugSphere(GetWorld(), GetActorLocation(), SphereRadius, 20, FColor::Purple, false, -1, 0, 1);
	//UE_LOG(LogTemp, Warning, TEXT("UpdateXposition is runnng"));
	//UE_LOG(LogTemp, Warning, TEXT("projectileLocation is %f"),projectileLocation.X);
	//UE_LOG(LogTemp, Warning, TEXT("playerposition is %f"), playerposition.X);
	if (projectileLocation.X > playerposition.X)
	{
		projectileLocation.X -= projectileSpeed;
		SetActorLocation(projectileLocation);
	}
	else if (projectileLocation.X < playerposition.X)
	{
		projectileLocation.X += projectileSpeed;
		SetActorLocation(projectileLocation);
	}
}

void ATrialTracking::UpdateYPosition(FVector projectileLocation, FVector playerposition)
{

	//if (projectileLocation.X > playerposition.X)
	//{
	//	projectileLocation.X -= projectileSpeed;
	//	//SetActorLocation(projectileLocation);
	//}
	//else if (projectileLocation.X < playerposition.X)
	//{
	//	projectileLocation.X += projectileSpeed;
	//	//SetActorLocation(projectileLocation);
	//}

	//if (projectileLocation.Y > playerposition.Y)
	//{
	//	projectileLocation.Y -= projectileSpeed;
	//	//SetActorLocation(projectileLocation);
	//}
	//else if (projectileLocation.Y < playerposition.Y)
	//{
	//	projectileLocation.Y += projectileSpeed;
	//	//SetActorLocation(projectileLocation);
	//}

	//if (projectileLocation.Z > playerposition.Z)
	//{
	//	projectileLocation.Z -= projectileSpeed;
	//	//SetActorLocation(projectileLocation);
	//}
	//else if (projectileLocation.Z < playerposition.Z)
	//{
	//	projectileLocation.Z += projectileSpeed;
	//	//SetActorLocation(projectileLocation);
	//}

	//SetActorLocation(projectileLocation);

	if (projectileLocation.Y > playerposition.Y)
		{
			projectileLocation.Y -= projectileSpeed;
			SetActorLocation(projectileLocation);
		}
		else if (projectileLocation.Y < playerposition.Y)
		{
			projectileLocation.Y += projectileSpeed;
			SetActorLocation(projectileLocation);
		}
	
}

void ATrialTracking::UpdateZPosition(FVector projectileLocation, FVector playerposition)
{
	if (projectileLocation.Z > playerposition.Z)
	{
		projectileLocation.Z -= projectileSpeed;
		SetActorLocation(projectileLocation);
	}
	else if (projectileLocation.Z < playerposition.Z)
	{
		projectileLocation.Z += projectileSpeed;
		SetActorLocation(projectileLocation);
	}
}

// Called every frame
void ATrialTracking::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector projectileLocation = GetActorLocation();
	/*projectileLocation.Z += 50 *DeltaTime;
	SetActorLocation(projectileLocation);*/
	FVector playerposition= GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(); 
	//UpdateXposition(projectileLocation,playerposition);
	UpdateYPosition(projectileLocation, playerposition);
	//UpdateZPosition(projectileLocation, playerposition);

}

//Detects collision and runs code when it collides
void ATrialTracking::OnOverLapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && OtherComp != nullptr)
	{
		if (OtherActor->GetName() == "FirstPersonCharacter2")
		{
			//Calls function from ASpawnLocation script
			((AUnbelievableCharacter*)OtherActor)->GetCharacterMovement()->StopMovementImmediately();
			((AUnbelievableCharacter*)OtherActor)->takeDamage(damageToDeal);
			((ASpawnLocation*)OtherActor)->ReSpawn();
			Destroy();
		}
	}
}

