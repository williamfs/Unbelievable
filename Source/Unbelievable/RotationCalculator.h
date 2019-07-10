// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotationCalculator.generated.h"

UCLASS()
class UNBELIEVABLE_API ARotationCalculator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotationCalculator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite,Category="Reference to scene component")
	USceneComponent* currentScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float degreesOfVison;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		APlayerController* player;

	UPROPERTY(VisibleAnywhere)
		float leftMaxLook;

	UPROPERTY(VisibleAnywhere)
		float rightMaxLook;

	UPROPERTY(VisibleAnywhere)
		FRotator playerRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Reference to platform rotation")
		FRotator platformRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="The final result we need")
		float finalResultNeeded;

	UPROPERTY(VisibleAnywhere)
		float differenceBetweenRotations;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isPlayerInRange;

	UPROPERTY()
		float timeElapsed;
};
