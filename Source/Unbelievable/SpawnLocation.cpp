#include "SpawnLocation.h"
#include "Engine/Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ASpawnLocation::ASpawnLocation()
{
}

// Called when the game starts or when spawned
void ASpawnLocation::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawnLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}	

//Sets player back to the last spawn point
void ASpawnLocation::Respawn()
{
	FVector NewLocation = SpawnLocation + FVector(0.f, 70.f, 0.f);

	GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(NewLocation, false, 0, ETeleportType::None);
}

//Sets the players spawn position to current position
void ASpawnLocation::SetSpawnLocation()
{
	SpawnLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
}