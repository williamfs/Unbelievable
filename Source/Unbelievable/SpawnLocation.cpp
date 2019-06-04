#include "SpawnLocation.h"
#include "Engine/Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"

// Sets default values
ASpawnLocation::ASpawnLocation()
{
	//ZFloat = GetActorLocation().Z;
	//const int C = 2;
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
void ASpawnLocation::ReSpawn() const
{
	GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(SpawnLocation, false, nullptr, ETeleportType::TeleportPhysics);
	GetWorld()->GetFirstPlayerController()->GetPawn()->GetController()->SetControlRotation(SpawnRotation);
}

//Sets the players spawn position to current position
void ASpawnLocation::SetSpawnLocation()
{	
	FVector f_vec;
	f_vec.X = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().X;
	f_vec.Y = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().Y;
	f_vec.Z = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().Z;

	SpawnLocation = FVector(f_vec);
	SpawnRotation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetController()->GetControlRotation();
}