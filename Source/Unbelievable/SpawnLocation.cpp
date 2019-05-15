#include "SpawnLocation.h"
#include "Engine/Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"

// Sets default values
ASpawnLocation::ASpawnLocation()
{
	SpawnRotation = FRotator::ZeroRotator;

	ZFloat = GetActorLocation().Z;
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
	FVector NewLocation = FVector(XFloat, YFloat, ZFloat + 300);

	GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(NewLocation, false, 0, ETeleportType::TeleportPhysics);
	GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorRotation(SpawnRotation, ETeleportType::TeleportPhysics);
}

//Sets the players spawn position to current position
void ASpawnLocation::SetSpawnLocation()
{	
	XFloat = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().X;
	YFloat = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().Y;

	SpawnRotation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorRotation();
}