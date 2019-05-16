// Fill out your copyright notice in the Description page of Project Settings.

#include "TrialTracking.h"
#include "Engine/Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
// Sets default values
ATrialTracking::ATrialTracking()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATrialTracking::BeginPlay()
{
	Super::BeginPlay();
	
}


void ATrialTracking::UpdateXposition(FVector projectileLocation, FVector playerposition)
{
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

	if (projectileLocation.X > playerposition.X)
	{
		projectileLocation.X -= projectileSpeed;
		//SetActorLocation(projectileLocation);
	}
	else if (projectileLocation.X < playerposition.X)
	{
		projectileLocation.X += projectileSpeed;
		//SetActorLocation(projectileLocation);
	}

	if (projectileLocation.Y > playerposition.Y)
	{
		projectileLocation.Y -= projectileSpeed;
		//SetActorLocation(projectileLocation);
	}
	else if (projectileLocation.Y < playerposition.Y)
	{
		projectileLocation.Y += projectileSpeed;
		//SetActorLocation(projectileLocation);
	}

	if (projectileLocation.Z > playerposition.Z)
	{
		projectileLocation.Z -= projectileSpeed;
		//SetActorLocation(projectileLocation);
	}
	else if (projectileLocation.Z < playerposition.Z)
	{
		projectileLocation.Z += projectileSpeed;
		//SetActorLocation(projectileLocation);
	}

	SetActorLocation(projectileLocation);
	
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

