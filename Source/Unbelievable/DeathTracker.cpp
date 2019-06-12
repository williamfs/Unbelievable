#include "DeathTracker.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
ADeathTracker::ADeathTracker(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereRadius = 100.f;
	MyCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("My Sphere Component"));
	MyCollisionSphere->Mobility = EComponentMobility::Movable;
	MyCollisionSphere->InitSphereRadius(SphereRadius);
	MyCollisionSphere->SetCollisionProfileName("Trigger");
	RootComponent = MyCollisionSphere;
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MY MESH"));
	MyMesh->SetupAttachment(RootComponent);

	IsTracking = true;
	//Creates the Collision Detection dynamic
	MyCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ADeathTracker::OnOverLapBegin);

	box = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, FName("My Box"));
	box->bHiddenInGame = false;
	box->Mobility = EComponentMobility::Movable;
	//RootComponent = box;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereMesh->AttachTo(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereMeshAsset.Succeeded()) {
		SphereMesh->SetStaticMesh(SphereMeshAsset.Object);
		SphereMesh->SetRelativeLocation(PosTwenty);
		SphereMesh->SetWorldScale3D(FVector(0.4f));
		SphereMesh->SetCollisionProfileName("NoCollision");
	}

	ConstructorHelpers::FObjectFinder<UMaterial>mat(TEXT("/Game/StarterContent/Materials/OffMaterial.OffMaterial"));
	if (mat.Succeeded())
	{
		StoredMaterial = mat.Object;
	}
	OffMaterial = UMaterialInstanceDynamic::Create(StoredMaterial, SphereMesh);

	static ConstructorHelpers::FObjectFinder<UMaterial>mat2(TEXT("/Game/StarterContent/Materials/DeadMaterial.DeadMaterial"));
	if (mat2.Succeeded())
	{
		StoredMaterial2 = mat2.Object;
	}
	DeadMaterial = UMaterialInstanceDynamic::Create(StoredMaterial2, SphereMesh);

	static ConstructorHelpers::FObjectFinder<UMaterial>mat3(TEXT("/Game/StarterContent/Materials/LivingMaterial.LivingMaterial"));
	if (mat3.Succeeded())
	{
		StoredMaterial3 = mat3.Object;
	}
	LivingMaterial = UMaterialInstanceDynamic::Create(StoredMaterial3, SphereMesh);
}

// Called when the game starts or when spawned
void ADeathTracker::BeginPlay()
{
	Super::BeginPlay();

	SphereMesh->SetMaterial(0, OffMaterial);

	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &ADeathTracker::AddPosTracker, 0.25f, true, 0.25f);
}

// Called every frame
void ADeathTracker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayDeath)
	{
		SphereMesh->SetMaterial(0, LivingMaterial);

		RetraceSteps(DeltaTime);
	}

	if (!IsTracking && !PlayDeath)
	{
		FVector NewLocation = GetActorLocation();
		const float delta_height = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
		
		NewLocation.Z += delta_height * 12;

		RunningTime += DeltaTime;
		SetActorLocation(NewLocation);
	}
}

void ADeathTracker::AddPosTracker()
{
	if (((GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - PosOne).Size() > 600) && PosOne != FVector(0,0,0))
	{
		if (IsTracking)
		{
			CheckPlayerDeath++;
			if (CheckPlayerDeath >= 3)
			{
				on_death();
				CheckPlayerDeath = 0;
			}
			else
			{
				PosTwenty = FVector(0,0,0);
				PosNineteen = FVector(0, 0, 0);
				PosEighteen = FVector(0, 0, 0);
				PosSeventeen = FVector(0, 0, 0);
				PosSixteen = FVector(0, 0, 0);
				PosFifteen = FVector(0, 0, 0);
				PosFourteen = FVector(0, 0, 0);
				PosThirteen = FVector(0, 0, 0);
				PosTwelve = FVector(0, 0, 0);
				PosEleven = FVector(0, 0, 0);
				PosTen = FVector(0, 0, 0);
				PosNine = FVector(0, 0, 0);
				PosEight = FVector(0, 0, 0);
				PosSeven = FVector(0, 0, 0);
				PosSix = FVector(0, 0, 0);
				PosFive = FVector(0, 0, 0);
				PosFour = FVector(0, 0, 0);
				PosThree = FVector(0, 0, 0);
				PosTwo = FVector(0, 0, 0);
				PosOne = FVector(0, 0, 0);
			}
		}
	}
	else if (IsTracking)
	{
		//SetActorLocation(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());

		PosTwenty = PosNineteen;
		PosNineteen = PosEighteen;
		PosEighteen = PosSeventeen;
		PosSeventeen = PosSixteen;
		PosSixteen = PosFifteen;
		PosFifteen = PosFourteen;
		PosFourteen = PosThirteen;
		PosThirteen = PosTwelve;
		PosTwelve = PosEleven;
		PosEleven = PosTen;
		PosTen = PosNine;
		PosNine = PosEight;
		PosEight = PosSeven;
		PosSeven = PosSix;
		PosSix = PosFive;
		PosFive = PosFour;
		PosFour = PosThree;
		PosThree = PosTwo;
		PosTwo = PosOne;
		PosOne = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	}
}

void ADeathTracker::on_death()
{
	if (IsTracking)
	{
		SphereMesh->SetMaterial(0, DeadMaterial);
		IsTracking = false;
		SetActorLocation(PosTwenty);
		GetWorld()->SpawnActor<ADeathTracker>(ADeathTracker::StaticClass(), PosOne, FRotator::ZeroRotator);
	}
}

void ADeathTracker::OnOverLapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherActor->GetName().Contains("FirstPersonCharacter") && !IsTracking)
		{
			PlayDeath = true;
		}
	}
}

void ADeathTracker::RetraceSteps(float FrameDeltaTime)
{
	if (target == FVector(0, 0, 0))
		target = PosTwenty;

	const float my_distance_from = (GetActorLocation() - target).Size();

	if (my_distance_from <= 0.02f)
	{
		SetActorLocation(target);
		ChangeTarget();
	}
	else
		SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), target, 3, 3), true);//Set the player's position to the calculated value
}

void ADeathTracker::ChangeTarget()
{
	if (target == PosOne)
	{
		target = PosTwenty;
		SetActorLocation(target);
		PlayDeath = false;
		SphereMesh->SetMaterial(0, DeadMaterial);
	}
	else if (target == PosTwo)
		target = PosOne;
	else if (target == PosThree)
		target = PosTwo;
	else if (target == PosFour)
		target = PosThree;
	else if (target == PosFive)
		target = PosFour;
	else if (target == PosSix)
		target = PosFive;
	else if (target == PosSeven)
		target = PosSix;
	else if (target == PosEight)
		target = PosSeven;
	else if (target == PosNine)
		target = PosEight;
	else if (target == PosTen)
		target = PosNine;
	else if (target == PosEleven)
		target = PosTen;
	else if (target == PosTwelve)
		target = PosEleven;
	else if (target == PosThirteen)
		target = PosTwelve;
	else if (target == PosFourteen)
		target = PosThirteen;
	else if (target == PosFifteen)
		target = PosFourteen;
	else if (target == PosSixteen)
		target = PosFifteen;
	else if (target == PosSeventeen)
		target = PosSixteen;
	else if (target == PosEighteen)
		target = PosSeventeen;
	else if (target == PosNineteen)
		target = PosEighteen;
	else if (target == PosTwenty)
		target = PosNineteen;
}