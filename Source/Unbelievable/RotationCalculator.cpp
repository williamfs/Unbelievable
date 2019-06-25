// Fill out your copyright notice in the Description page of Project Settings.

#include "RotationCalculator.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformMath.h"
// Sets default values
ARotationCalculator::ARotationCalculator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARotationCalculator::BeginPlay()
{
	Super::BeginPlay();
	platformRotation = GetActorRotation();
	rightMaxLook = platformRotation.Yaw + degreesOfVison;
	leftMaxLook = platformRotation.Yaw - degreesOfVison;
	player = GetWorld()->GetFirstPlayerController();
	timeElapsed = 0.0f;
	isPlayerInRange = false;
	//leftMaxLook
}

// Called every frame
void ARotationCalculator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//timeElapsed += DeltaTime;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Current value of timeElapsed: %f"),timeElapsed));
	if (isPlayerInRange==true)
	{
		//UE_LOG(LogTemp, Warning, TEXT("The player y-axis value is: %f"), playerRotation.Yaw);
		//UE_LOG(LogTemp, Warning, TEXT("The platform y-axis value is: %f"), platformRotation.Yaw);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("The player y-axis value is: %f"), playerRotation.Yaw));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("The platform y-axis value is: %f"), platformRotation.Yaw));
		playerRotation = player->GetPawn()->GetActorRotation();

		if (playerRotation.Yaw < rightMaxLook && playerRotation.Yaw != platformRotation.Yaw)
		{
			differenceBetweenRotations = playerRotation.Yaw - platformRotation.Yaw;
			finalResultNeeded = FGenericPlatformMath::Abs(differenceBetweenRotations / degreesOfVison);
			//UE_LOG(LogTemp, Warning, TEXT("The final calculated result value is: %f"), finalResultNeeded);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("The final calculated result value is: %f"), finalResultNeeded));
		}
		if (playerRotation.Yaw > leftMaxLook && playerRotation.Yaw != platformRotation.Yaw)
		{
			differenceBetweenRotations = platformRotation.Yaw - playerRotation.Yaw;
			finalResultNeeded = FGenericPlatformMath::Abs(differenceBetweenRotations / degreesOfVison);
			//UE_LOG(LogTemp, Warning, TEXT("The final calculated result value is: %f"), finalResultNeeded);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("The final calculated result value is: %f"), finalResultNeeded));
		}

		if (playerRotation.Yaw == platformRotation.Yaw)
		{
			finalResultNeeded = 0.0f;
			//UE_LOG(LogTemp, Warning, TEXT("The final calculated result value is: %f"), finalResultNeeded);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("The final calculated result value is: %f"), finalResultNeeded));
		}

		if (playerRotation.Yaw > rightMaxLook)
		{
			finalResultNeeded = 1.0f;
			//UE_LOG(LogTemp, Warning, TEXT("The final calculated result value is: %f"), finalResultNeeded);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("The final calculated result value is: %f"), finalResultNeeded));
		}

		if (playerRotation.Yaw < leftMaxLook)
		{
			finalResultNeeded = 1.0f;
			//UE_LOG(LogTemp, Warning, TEXT("The final calculated result value is: %f"), finalResultNeeded);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("The final calculated result value is: %f"), finalResultNeeded));
		}
		//timeElapsed = 0.0f;
	}
}

