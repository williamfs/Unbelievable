// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrialTracking.generated.h"

UCLASS()
class UNBELIEVABLE_API ATrialTracking : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrialTracking();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category = "Speed of follow")
		float projectileSpeed;
	void UpdateXposition(FVector projectileLocation, FVector playerposition);
	void UpdateYPosition(FVector projectileLocation, FVector playerposition);
	void UpdateZPosition(FVector projectileLocation, FVector playerposition);

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* MyCollisionSphere;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* MyMesh;
	UPROPERTY(EditAnywhere)
		float SphereRadius;
	UPROPERTY(EditAnywhere, Category = "Damage based on Mode")
		int coreModeDamage;
	UPROPERTY(EditAnywhere, Category = "Damage based on Mode")
		int hardModeDamage;
	UPROPERTY(EditAnywhere, Category = "Damage based on Mode")
		int shadowModeDamage;
	UPROPERTY(VisibleAnywhere)
		int damageToDeal;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inherited Level Selection")
		int levelSelected;

	UFUNCTION()
		void OnOverLapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
