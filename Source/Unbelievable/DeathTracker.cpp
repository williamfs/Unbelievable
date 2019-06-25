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
	MyMesh->SetCollisionObjectType(ECC_WorldDynamic);

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
		SphereMesh->SetCollisionObjectType(ECC_WorldDynamic);
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

	if (!PlayDeath)
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
	if (((GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - PosOne).Size() > 600) && PosOne != FVector(0,0,0) && !PlayDeath)
	{
		on_death();
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
		//IsTracking = false;
		PosTwenty2 = PosTwenty;
		PosNineteen2 = PosNineteen;
		PosEighteen2 = PosEighteen;
		PosSeventeen2 = PosSeventeen;
		PosSixteen2 = PosSixteen;
		PosFifteen2 = PosFifteen;
		PosFourteen2 = PosFourteen;
		PosThirteen2 = PosThirteen;
		PosTwelve2 = PosTwelve;
		PosEleven2 = PosEleven;
		PosTen2 = PosTen;
		PosNine2 = PosNine;
		PosEight2 = PosEight;
		PosSeven2 = PosSeven;
		PosSix2 = PosSix;
		PosFive2 = PosFive;
		PosFour2 = PosFour;
		PosThree2 = PosThree;
		PosTwo2 = PosTwo;
		PosOne2 = PosOne;
		SetActorLocation(PosTwenty2);

		PosTwenty = FVector(0, 0, 0);
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
		//GetWorld()->SpawnActor<ADeathTracker>(ADeathTracker::StaticClass(), PosOne, FRotator::ZeroRotator);
	}
}

void ADeathTracker::OnOverLapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherActor->GetName().Contains("FirstPersonCharacter"))
		{
			PlayDeath = true;
		}
	}
}

void ADeathTracker::RetraceSteps(float FrameDeltaTime)
{
	if (target == FVector(0, 0, 0))
		target = PosTwenty2;

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
	if (target == PosOne2)
	{
		target = FVector(0, 0, 0);
		SetActorLocation(PosTwenty2);
		PlayDeath = false;
		SphereMesh->SetMaterial(0, DeadMaterial);
	}
	else if (target == PosTwo2)
		target = PosOne2;
	else if (target == PosThree2)
		target = PosTwo2;
	else if (target == PosFour2)
		target = PosThree2;
	else if (target == PosFive2)
		target = PosFour2;
	else if (target == PosSix2)
		target = PosFive2;
	else if (target == PosSeven2)
		target = PosSix2;
	else if (target == PosEight2)
		target = PosSeven2;
	else if (target == PosNine2)
		target = PosEight2;
	else if (target == PosTen2)
		target = PosNine2;
	else if (target == PosEleven2)
		target = PosTen2;
	else if (target == PosTwelve2)
		target = PosEleven2;
	else if (target == PosThirteen2)
		target = PosTwelve2;
	else if (target == PosFourteen2)
		target = PosThirteen2;
	else if (target == PosFifteen2)
		target = PosFourteen2;
	else if (target == PosSixteen2)
		target = PosFifteen2;
	else if (target == PosSeventeen2)
		target = PosSixteen2;
	else if (target == PosEighteen2)
		target = PosSeventeen2;
	else if (target == PosNineteen2)
		target = PosEighteen2;
	else if (target == PosTwenty2)
		target = PosNineteen2;
}