#include "PlayerDeath.h"
#include "SpawnLocation.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObjectIterator.h"
#include "Engine/Engine.h"
#include "UnbelievableCharacter.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
APlayerDeath::APlayerDeath()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creates the Trigger area for collision
	SphereRadius = 200.f;
	MyCollisionSphere = CreateDefaultSubobject<UBoxComponent>(TEXT("My Sphere Component"));
	//MyCollisionSphere->InitSphereRadius(SphereRadius);
	MyCollisionSphere->SetBoxExtent(FVector(200.f, 200.f, 96.f));

	MyCollisionSphere->SetCollisionProfileName("Trigger");
	RootComponent = MyCollisionSphere;
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MY MESH"));
	MyMesh->SetupAttachment(RootComponent);

	//Creates the Collision Detection dynamic
	MyCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerDeath::OnOverLapBegin);
}

// Called when the game starts or when spawned
void APlayerDeath::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerDeath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Shows radius of trigger for debug purposes
	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(200.f, 200.f, 96.f), FColor::Purple, false, -1, 0, 1);
}

//Detects collision and runs code when it collides
void APlayerDeath::OnOverLapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && OtherComp != nullptr)
	{
		if (OtherActor->GetName() == "FirstPersonCharacter2")
		{
			//Calls function from ASpawnLocation script
			((AUnbelievableCharacter*)OtherActor)->GetCharacterMovement()->StopMovementImmediately();
			((ASpawnLocation*)OtherActor)->ReSpawn();
		}
	}
}