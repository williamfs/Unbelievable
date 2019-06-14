#include "UnbelievableCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Unbelievable_SaveGame.h"
#include "GameFramework/PlayerController.h"
#include "DeathTracker.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);
AUnbelievableCharacter::AUnbelievableCharacter()
{
	//Sets up Collision
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	//Sets up the camera functionality
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	//Sets up Jump variables
	JumpHeight = 600.f;
	GetCharacterMovement()->AirControl = 1.0f;
	WallJumpTraces = 20;
	WalljumpHorizontalStrenght = 1200;
	WalljumpUpwardsStrength = 1500;
	WallJumpTraceDistance = 100;
	id = this;
	healthPoints = 10;
}

//Called on start
void AUnbelievableCharacter::BeginPlay()
{
	Super::BeginPlay();

	UClass* player = ADeathTracker::StaticClass();

	GetWorld()->SpawnActor<ADeathTracker>(player, GetActorLocation(), FRotator::ZeroRotator);
}

void AUnbelievableCharacter::takeDamage(int damageAmount)
{
	healthPoints -= damageAmount;
}

//Updates every frame
void AUnbelievableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Debug();

	if (isheld == true)
		CanWallRun = true;
	else
		CanWallRun = false;

	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
		GetCharacterMovement()->GravityScale = 1.31f;
	else
		GetCharacterMovement()->GravityScale = 1;
}

//Initialize keyboard inputs
void AUnbelievableCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	// Sets up test bindings for save and load
	InputComponent->BindAction("Save", IE_Pressed, this, &AUnbelievableCharacter::tempSaveGame);
	InputComponent->BindAction("Load", IE_Pressed, this, &AUnbelievableCharacter::LoadGame);
	//Sets key binds for jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AUnbelievableCharacter::Jump);

	//PlayerInputComponent->BindAction("IncSControl", IE_Pressed, this, &AUnbelievableCharacter::SingleJumpIncrement);
	//PlayerInputComponent->BindAction("IncDControl", IE_Pressed, this, &AUnbelievableCharacter::DoubleJumpIncrement);

	//Sets key binds for movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AUnbelievableCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUnbelievableCharacter::MoveRight);
	PlayerInputComponent->BindAction("WallRun", IE_Pressed, this, &AUnbelievableCharacter::WallRun);
	PlayerInputComponent->BindAction("WallRun", IE_Released, this, &AUnbelievableCharacter::WallRunEnd);

	//Sets key binds for camera movement
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUnbelievableCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AUnbelievableCharacter::LookUpAtRate);

	//Sets key binds for dodge
	PlayerInputComponent->BindAction("DodgeLeft", IE_Pressed, this, &AUnbelievableCharacter::DodgeLeft);
	PlayerInputComponent->BindAction("DodgeRight", IE_Pressed, this, &AUnbelievableCharacter::DodgeRight);
	PlayerInputComponent->BindAction("DodgeLeft", IE_Released, this, &AUnbelievableCharacter::EndDodge);
	PlayerInputComponent->BindAction("DodgeRight", IE_Released, this, &AUnbelievableCharacter::EndDodge);
}

//Core movement functions including camera 
#pragma region CoreMovement
//Movement for forward and backwards
void AUnbelievableCharacter::MoveForward(float Value)
{
	if (CanWallRun && Value != 0.0f && !GetCharacterMovement()->IsMovingOnGround() && DisableSpecialMovement)
	{
		// Do a ring of traces
		FVector TraceStart = GetActorLocation();
		float traceDistance = 100;
		float MinDistance = 9999999;
		FVector HitLocation = FVector::ZeroVector;
		FVector TraceRight = GetActorRightVector();
		FVector RightEnd = TraceStart + TraceRight * traceDistance;
		FVector TraceLeft = -GetActorRightVector();
		FVector LeftEnd = TraceStart + TraceLeft * traceDistance;
		static FName TraceTag = FName(TEXT("WeaponTrace"));
		FCollisionQueryParams TraceParams(TraceTag, true, Instigator);
		TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = true;
		FHitResult Hit(ForceInit);
		for (int i = 0; i < WallJumpTraces; i++)
		{
			//Checks if the player hits a wall
			if (GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, RightEnd, ECC_WorldStatic, TraceParams))
			{
				//Checks if the hit wall was just jumped from and if not it applies the values to variable needed for the jump
				if ((Hit.Location - TraceStart).Size() < MinDistance && !Hit.Actor->GetName().Contains("DeathTracker"))
				{
					HitLocation = Hit.Location;
					MinDistance = (Hit.Location - TraceStart).Size();
					GetCharacterMovement()->Velocity.Z = -75;
					FirstPersonCameraComponent->bUsePawnControlRotation = false;
					bUseControllerRotationRoll = true;
					FirstPersonCameraComponent->SetRelativeRotation(FMath::Lerp(FirstPersonCameraComponent->RelativeRotation, FRotator(0.0f, 0.0f, -22.5f).Clamp(), 0.01f));
					AddMovementInput(GetActorForwardVector(), Value * 1.33f);
				}
				else
				{
					StopSideMovement = true;
					HitLocation = FVector::ZeroVector;
					MinDistance = 9999999;
				}
			}
			else if(GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, LeftEnd, ECC_WorldStatic, TraceParams))
			{
				//Checks if the hit wall was just jumped from and if not it applies the values to variable needed for the jump
				if ((Hit.Location - TraceStart).Size() < MinDistance && !Hit.Actor->GetName().Contains("DeathTracker"))
				{
					HitLocation = Hit.Location;
					MinDistance = (Hit.Location - TraceStart).Size();
					GetCharacterMovement()->Velocity.Z = -75;
					FirstPersonCameraComponent->bUsePawnControlRotation = false;
					bUseControllerRotationRoll = true;
					FirstPersonCameraComponent->SetRelativeRotation(FMath::Lerp(FirstPersonCameraComponent->RelativeRotation, FRotator(0.0f, 0.0f, 22.5f).Clamp(), 0.01f));
					AddMovementInput(GetActorForwardVector(), Value * 1.33f);
				}
				else
				{
					StopSideMovement = true;
					HitLocation = FVector::ZeroVector;
					MinDistance = 9999999;
				}
			}
		}
	}
	else if (Value != 0.0f)
	{
		if (bUseControllerRotationRoll)
		{
			FirstPersonCameraComponent->SetRelativeRotation(FMath::Lerp(FirstPersonCameraComponent->RelativeRotation, FRotator(0.0f, 0.0f, 0.0f).Clamp(), 0.01f));
			FirstPersonCameraComponent->bUsePawnControlRotation = true;
			bUseControllerRotationRoll = false;
		}
		StopSideMovement = false;
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

//Movement for right and left
void AUnbelievableCharacter::MoveRight(float Value)
{
	if (Value != 0.0f && !StopSideMovement)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AUnbelievableCharacter::WallRun()
{
	isheld = true;
}

void AUnbelievableCharacter::WallRunEnd()
{
	isheld = false;
}

//Movement for turning side to side
void AUnbelievableCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

//Movement for looking up and down
void AUnbelievableCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
#pragma endregion CoreMovement

//Functions that control dodge mechanics
#pragma region Dodge
//Launches the player left
void AUnbelievableCharacter::DodgeLeft()
{
	//checks if cooldown is active
	if (CanDodge == true && DisableSpecialMovement)
	{
		CanDodge = false;

		//Lowers friction for smoother movement
		GetCharacterMovement()->GroundFriction = 0;

		//Finds left directiong of the player camera
		const FVector ForwardDir = FirstPersonCameraComponent->GetRightVector();
		const FVector AddForce = -(ForwardDir * 750) + FVector(0, 0, 10) * 10;

		//Launches the player in the direction just found
		ACharacter::LaunchCharacter(AddForce, false, false);

		//Starts the cooldown timer for the dodge
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AUnbelievableCharacter::DodgeCooldown, 1.0f, false, 1.0f);
	}
}

//Launches the player right
void AUnbelievableCharacter::DodgeRight()
{
	//checks if cooldown is active
	if (CanDodge == true && DisableSpecialMovement)
	{
		CanDodge = false;

		//Lowers friction for smoother movement
		GetCharacterMovement()->GroundFriction = 0;

		//Finds right directiong of the player camera
		const FVector ForwardDir = FirstPersonCameraComponent->GetRightVector();
		const FVector AddForce = (ForwardDir * 750) + FVector(0, 0, 10) * 10;

		//Launches the player in the direction just found
		ACharacter::LaunchCharacter(AddForce, false, false);

		//Starts the cooldown timer for the dodge
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AUnbelievableCharacter::DodgeCooldown, 1.0f, false, 1.0f);
	}
}

//Resets the friction after the player dodges
void AUnbelievableCharacter::EndDodge()
{
	GetCharacterMovement()->GroundFriction = 1;
}

//Resets dodge after cooldown timer
void AUnbelievableCharacter::DodgeCooldown()
{
	CanDodge = true;
}
#pragma endregion Dodge

//Functions that control jump, double jump, and wall jump
#pragma region Jump
//Controls if the player should jump or wall jump
void AUnbelievableCharacter::Jump()
{
	if (!GetCharacterMovement()->IsMovingOnGround() && DisableSpecialMovement)
	{
		// Do a ring of traces
		FVector TraceStart = GetActorLocation();
		FVector Front = GetActorRotation().Vector();
		FVector Side = FVector::CrossProduct(Front, FVector::UpVector);
		float MinDistance = 9999999;
		FVector HitLocation = FVector::ZeroVector;
		FVector HitNormal;

		for (int i = 0; i < WallJumpTraces; i++)
		{
			//Basically a ray cast that finds if the player hits a wall then creates a direction for the player to jump
			float TraceAngle = 360 / WallJumpTraces * i;
			FVector TraceDir = Front * FMath::Sin(TraceAngle) + Side * FMath::Cos(TraceAngle);
			FVector TraceEnd = TraceStart + TraceDir * WallJumpTraceDistance;
			static FName TraceTag = FName(TEXT("WeaponTrace"));
			FCollisionQueryParams TraceParams(TraceTag, true, Instigator);
			TraceParams.bTraceAsyncScene = true;
			TraceParams.bReturnPhysicalMaterial = true;
			FHitResult Hit(ForceInit);

			//Checks if the player hits a wall
			if (GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ECC_WorldStatic, TraceParams))
			{
				//Checks if the hit wall was just jumped from and if not it applies the values to variable needed for the jump
				if ((Hit.Location - TraceStart).Size() < MinDistance && Hit.GetActor()->GetUniqueID() != id->
					GetUniqueID())
				{
					HitLocation = Hit.Location;
					HitNormal = Hit.Normal;
					MinDistance = (Hit.Location - TraceStart).Size();
					id = Hit.GetActor();
				}
			}
		}

		//Checks if the player should jump from the wall and launches them
		if (HitLocation != FVector::ZeroVector && canWallJump)
		{
			isheld = false;

			GetCharacterMovement()->AirControl = 1;
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(MyShake, 0.5f);
			canWallJump = false;
			GetWorldTimerManager().SetTimer(MemberTimerHandle2, this, &AUnbelievableCharacter::AllowWallJump, 0.3f, false, 0.3f);
			LaunchCharacter((HitNormal * WalljumpHorizontalStrenght + FVector::UpVector * WalljumpUpwardsStrength) / 2, false, true);
		}
		else
		{
			DoubleJump();
		}
	}
	else
	{
		DoubleJump();
	}
}

//Jump functionality, launches player into air and can be pressed twice and is reset when touching the ground
void AUnbelievableCharacter::DoubleJump()
{
	if (DoubleJumpCounter == 0 && DisableSpecialMovement)
	{
		GetCharacterMovement()->AirControl = SingleJumpControl;
		ACharacter::LaunchCharacter(FVector(0, 0, JumpHeight), false, true);
		DoubleJumpCounter++;
	}
	else if (DoubleJumpCounter == 1 && DisableSpecialMovement && canWallJump)
	{
		GetCharacterMovement()->AirControl = DoubleJumpControl;
		canWallJump = false;
		GetWorldTimerManager().SetTimer(MemberTimerHandle2, this, &AUnbelievableCharacter::AllowWallJump, 0.3f, false, 0.3f);
		ACharacter::LaunchCharacter(FVector(0, 0, JumpHeight), false, true);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(MyShake, 0.5f);
		DoubleJumpCounter++;
	}
}

//Detects when the players touches the ground
void AUnbelievableCharacter::Landed(const FHitResult& Hit)
{
	DoubleJumpCounter = 0;
	id = this;
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(MyShake, 0.75f);
}

void AUnbelievableCharacter::AllowWallJump()
{
	canWallJump = true;
}

#pragma endregion Jump

//Debug functions to be removed after testing
#pragma region Debug
void AUnbelievableCharacter::Debug()
{
	if (MyShake == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("Set MyShake to MyCameraShake in the FirstPersonCharacter"));
	}
}
void AUnbelievableCharacter::tempSaveGame()
{
	UUnbelievable_SaveGame* SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::CreateSaveGameObject(UUnbelievable_SaveGame::StaticClass()));
	SaveGameInstance->PlayerLocation = this->GetActorLocation();
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot"), 0);
	GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("Saved Game"));
}
void AUnbelievableCharacter::SaveGame()
{
	UUnbelievable_SaveGame* SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::CreateSaveGameObject(UUnbelievable_SaveGame::StaticClass()));
	SaveGameInstance->PlayerLocation = locationToSet;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot"), 0);
	GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("Saved Game"));
}
void AUnbelievableCharacter::LoadGame()
{
	UUnbelievable_SaveGame* SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::CreateSaveGameObject(UUnbelievable_SaveGame::StaticClass()));
	SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::LoadGameFromSlot("MySlot",0));
	this->SetActorLocation(SaveGameInstance->PlayerLocation);
	GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("Loaded Game"));
}
//void AUnbelievableCharacter::SingleJumpIncrement()
//{
//	if (SingleJumpControl < 1)
//	{
//		SingleJumpControl += 0.1f;
//		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("Single Jump Increase Control"));
//	}
//	else
//	{
//		SingleJumpControl = 0;
//		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("Single Jump Control Set To Zero"));
//	}
//}
//void AUnbelievableCharacter::DoubleJumpIncrement()
//{
//	if (DoubleJumpControl < 1)
//	{
//		DoubleJumpControl += 0.1f;
//		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("Double Jump Increase Control"));
//	}
//	else
//	{
//		DoubleJumpControl = 0;
//		GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("Double Jump Control Set To Zero"));
//	}
//}
#pragma endregion Debug