// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/Material.h"
#include "DeathTracker.generated.h"

UCLASS()
class UNBELIEVABLE_API ADeathTracker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeathTracker(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void AddPosTracker();

	struct FTimerHandle MemberTimerHandle;

	FVector PosOne, PosTwo, PosThree, PosFour, PosFive, PosSix, PosSeven, PosEight, PosNine, PosTen, PosEleven, PosTwelve, PosThirteen, PosFourteen, PosFifteen, PosSixteen, PosSeventeen, PosEighteen, PosNineteen, PosTwenty;

	FVector PosOne2, PosTwo2, PosThree2, PosFour2, PosFive2, PosSix2, PosSeven2, PosEight2, PosNine2, PosTen2, PosEleven2, PosTwelve2, PosThirteen2, PosFourteen2, PosFifteen2, PosSixteen2, PosSeventeen2, PosEighteen2, PosNineteen2, PosTwenty2;

	UPROPERTY(VisibleAnywhere)
	bool IsTracking = true;

	UPROPERTY(VisibleAnywhere)
	bool PlayDeath = false;

	UPROPERTY()
		int CheckPlayerDeath = 0;

	UFUNCTION()
	void on_death();

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* MyCollisionSphere;

	UPROPERTY(VisibleAnywhere)
	UMaterialInstanceDynamic* OffMaterial;

	UPROPERTY(VisibleAnywhere)
	UMaterialInstanceDynamic* DeadMaterial;

	UPROPERTY(VisibleAnywhere)
	UMaterialInstanceDynamic* LivingMaterial;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MyMesh;

	UPROPERTY(VisibleAnywhere)
	float SphereRadius;

	UPROPERTY(VisibleAnywhere)
	UMaterial* StoredMaterial;

	UPROPERTY(VisibleAnywhere)
	UMaterial* StoredMaterial2;

	UPROPERTY(VisibleAnywhere)
	UMaterial* StoredMaterial3;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* box;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SphereMesh;

	UPROPERTY(VisibleAnywhere)
	FVector target;

	float RunningTime;

	UFUNCTION()
	void OnOverLapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void RetraceSteps(float FrameDeltaTime);

	UFUNCTION()
	void ChangeTarget();
};