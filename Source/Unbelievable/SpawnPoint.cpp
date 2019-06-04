#include "SpawnPoint.h"
#include "SpawnLocation.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObjectIterator.h"
#include "Engine/Engine.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creates the Trigger area for collision
	SphereRadius = 200.f;
	MyCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("My Sphere Component"));
	MyCollisionSphere->InitSphereRadius(SphereRadius);
	MyCollisionSphere->SetCollisionProfileName("Trigger");
	RootComponent = MyCollisionSphere;
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MY MESH"));
	MyMesh->SetupAttachment(RootComponent);

	//Creates the Collision Detection dynamic
	MyCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASpawnPoint::OnOverLapBegin);
}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Shows radius of trigger for debug purposes
	DrawDebugSphere(GetWorld(), GetActorLocation(), SphereRadius, 20, FColor::Green, false, -1, 0, 1);
}

//Detects collision and runs code when it collides
void ASpawnPoint::OnOverLapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (isCheckPoint == false) {
		isCheckPoint = true;
		if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
		{
			if (OtherActor->GetName() == "FirstPersonCharacter2")
			{
				//Calls function from ASpawnLocation script
				static_cast<ASpawnLocation*>(OtherActor)->SetSpawnLocation();
			}
		}
	}
}