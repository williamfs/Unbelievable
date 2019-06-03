#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerDeath.generated.h"

UCLASS()
class APlayerDeath : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerDeath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* MyCollisionSphere;
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* MyMesh;
	float SphereRadius;

	UFUNCTION()
		void OnOverLapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};