#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnLocation.generated.h"

UCLASS()
class ASpawnLocation : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnLocation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = Location)
		FVector SpawnLocation;

	UPROPERTY(EditAnywhere, Category = Rotation)
		FRotator SpawnRotation;

	void ReSpawn() const;

	//UFUNCTION(BlueprintCallable, Category = "category")
	void SetSpawnLocation();
};