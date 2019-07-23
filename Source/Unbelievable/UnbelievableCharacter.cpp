#include "UnbelievableCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Editor/UnrealEd/Public/Editor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Unbelievable_SaveGame.h"
#include "GameFramework/PlayerController.h"
#include "DeathTracker.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetStringLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);
AUnbelievableCharacter::AUnbelievableCharacter()
{
	//Sets up Collision
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	//Sets up the camera functionality
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(30.0, 1.75f, 64.f); // Position the camera
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
	//healthPoints = 10;
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
	debugTimer += DeltaTime;
	if (isheld)
	{
		CanWallRun = true;
		if (debugTimer > delayToPrint) // Method fragment to check for how Jon is evaluating the Wall Run, specifically checkking to see if shift is held
		{
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("CanWallRun value is: %s"), CanWallRun ? TEXT("true") : TEXT("false")));
			debugTimer = 0.0f;

			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("WallClimb value is: %s"), WallClimb ? TEXT("true") : TEXT("false")));	

			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("WallClimb2 value is: %s"), WallClimb2 ? TEXT("true") : TEXT("false")));
		}
	}
	else
	{
		CanWallRun = false;
		if (debugTimer > delayToPrint)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("CanWallRun value is: %s"), CanWallRun ? TEXT("true") : TEXT("false")));
			debugTimer = 0.0f;

			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("WallClimb value is: %s"), WallClimb ? TEXT("true") : TEXT("false")));

			//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("WallClimb2 value is: %s"), WallClimb2 ? TEXT("true") : TEXT("false")));
		}
	}
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
	{
		GetCharacterMovement()->GravityScale = 1.31f;
		timeSpentInAir += DeltaTime;
	}
	else
	{
		GetCharacterMovement()->GravityScale = 1;
		timeSpentInAir = 0.0f;
	}
	float_TimeSpentInGame += DeltaTime;

	/*if (healthTimer > rateOfRegeneration)
	{
		healthTimer = 0;
		RegainHealth();
	}*/
	if (damageHasBeenTaken)
	{
		healthTimer += DeltaTime;
	}
	if (healthTimer > rateOfRegeneration)
	{
		shouldBeHealing = true;
		damageHasBeenTaken = false;
	}

	if (shouldBeHealing)
	{
		RegainHealth();
		shouldBeHealing = false;
		/*if (healthPoints == healthPointsMaximum)
		{
			shouldBeHealing = false;
		}*/
	}
	//healthTimer += DeltaTime;
	/*FHitResult OutHit;

	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector End = (Start + (ForwardVector*1000.0f));

	FCollisionQueryParams CollisionParams;
	DrawDebugLine(GetWorld(),Start,End,FColor::Green,false,1,0,1);

	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

	if (isHit)
	{
		if (OutHit.bBlockingHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName()));
		}
	}*/
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
	/*PlayerInputComponent->BindAxis("TurnRate", this, &AUnbelievableCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);*/
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
	if (CanWallRun && Value != 0.0f && !GetCharacterMovement()->IsMovingOnGround() && DisableSpecialMovement) // Disable specual movement is to disable movement for entering the vignette
	{
		// Do a ring of traces
		/*FVector TraceStart = GetActorLocation();
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
		FHitResult Hit(ForceInit);*/
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("The value of Value For Move Forward is: %f"), Value));
		//for (int i = 0; i < WallJumpTraces; i++)
		//{
			//Checks if the player hits a wall
			if (GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, RightEnd, ECC_WorldStatic, TraceParams))
			{
				debugFloat += 1;
				UE_LOG(LogTemp, Warning, TEXT("The value of debug float is: %f"), debugFloat); 
				//Checks if the hit wall was just jumped from and if not it applies the values to variable needed for the jump
				if ((Hit.Location - TraceStart).Size() < MinDistance && !Hit.Actor->GetName().Contains("DeathTracker"))
				{
					HitLocation = Hit.Location;
					MinDistance = (Hit.Location - TraceStart).Size();
					if (WallClimb)
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("The value of Value For Move Forward is: %f"), Value));
						GetCharacterMovement()->Velocity.Z = arcOfWallRun;
						if (WallClimb2) 
						{
							//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("The value of Value For Move Forward is: %f"), Value));
							GetWorldTimerManager().SetTimer(MemberTimerHandle2, this, &AUnbelievableCharacter::RunFall, 1.0f, false, 1.0f);
							WallClimb2 = false;
						}
					}
					else
						GetCharacterMovement()->Velocity.Z = -75;

					FirstPersonCameraComponent->bUsePawnControlRotation = false;
					bUseControllerRotationRoll = true;
					FirstPersonCameraComponent->SetRelativeRotation(FMath::Lerp(FirstPersonCameraComponent->RelativeRotation, FRotator(0.0f, 0.0f, -22.5f).Clamp(), 0.01f));
					AddMovementInput(GetActorForwardVector(), Value * 2);
				}
				else
				{
					StopSideMovement = true;
					HitLocation = FVector::ZeroVector;
					MinDistance = 9999999;
					//FirstPersonCameraComponent->SetRelativeRotation(FMath::Lerp(FirstPersonCameraComponent->RelativeRotation, FRotator(0, 0, 0).Clamp(), 0.1f));
				}
			}
			else if(GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, LeftEnd, ECC_WorldStatic, TraceParams))
			{
				//Checks if the hit wall was just jumped from and if not it applies the values to variable needed for the jump
				if ((Hit.Location - TraceStart).Size() < MinDistance && !Hit.Actor->GetName().Contains("DeathTracker"))
				{
					HitLocation = Hit.Location;
					MinDistance = (Hit.Location - TraceStart).Size();
					if (WallClimb)
					{
						GetCharacterMovement()->Velocity.Z = arcOfWallRun;
						if (WallClimb2)
						{
							GetWorldTimerManager().SetTimer(MemberTimerHandle2, this, &AUnbelievableCharacter::RunFall, 1.0f, false, 1.0f);
							WallClimb2 = false;
						}
					}
					else
						GetCharacterMovement()->Velocity.Z = -75;
					FirstPersonCameraComponent->bUsePawnControlRotation = false;
					bUseControllerRotationRoll = true;
					FirstPersonCameraComponent->SetRelativeRotation(FMath::Lerp(FirstPersonCameraComponent->RelativeRotation, FRotator(0.0f, 0.0f, 22.5f).Clamp(), 0.01f));
					AddMovementInput(GetActorForwardVector(), Value * 2);
				}
				else
				{
					StopSideMovement = true;
					HitLocation = FVector::ZeroVector;
					MinDistance = 9999999;
				}
			}
		//}
	}
	else if (!WallClimb && !GetCharacterMovement()->IsMovingOnGround() && Value==0.0) //if you land after/while  holding shift
	{
		FirstPersonCameraComponent->SetRelativeRotation(FMath::Lerp(FirstPersonCameraComponent->RelativeRotation, FRotator(0, 0, 0).Clamp(), 0.1f));
		GetWorldTimerManager().SetTimer(MemberTimerHandle4, this, &AUnbelievableCharacter::reset_camera_rotation, 1.0f, false, 0.1f);
	}
	else if(Value != 0.0f)//else if (Value != 0.0f) // 
	{
		//if (bUseControllerRotationRoll)
		//{
		//	FirstPersonCameraComponent->SetRelativeRotation(FMath::Lerp(FirstPersonCameraComponent->RelativeRotation, FRotator(0.0f, 0.0f, 0.0f).Clamp(), 0.1f));
		//	if (FirstPersonCameraComponent->RelativeRotation == FRotator(0.0f, 0.0f, 0.0f))
		//	{
		//		FirstPersonCameraComponent->bUsePawnControlRotation = true;
		//		bUseControllerRotationRoll = false;
		//	}

		//}
		FirstPersonCameraComponent->SetRelativeRotation(FMath::Lerp(FirstPersonCameraComponent->RelativeRotation, FRotator(0, 0, 0).Clamp(), 0.1f));
		GetWorldTimerManager().SetTimer(MemberTimerHandle4, this, &AUnbelievableCharacter::reset_camera_rotation, 1.0f, false, 0.1f);

		StopSideMovement = false;
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AUnbelievableCharacter::reset_camera_rotation()
{
	FirstPersonCameraComponent->SetRelativeRotation(FRotator(0, FirstPersonCameraComponent->RelativeRotation.Yaw, FirstPersonCameraComponent->RelativeRotation.Pitch));
	//Controller->SetControlRotation(FirstPersonCameraComponent->RelativeRotation);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	bUseControllerRotationRoll = false;
}


void AUnbelievableCharacter::RunFall()
{
	WallClimb = false;
	WallClimb2 = true;
}

void AUnbelievableCharacter::cool_down()
{
	WallRunCooldown = true;
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
	if (WallRunCooldown == true)
	{
		WallRunCooldown = false;
		isheld = true;
		GetWorldTimerManager().SetTimer(MemberTimerHandle3, this, &AUnbelievableCharacter::cool_down, 1.0f, false, 1.0f); // Equivalent of Coroutine which is a program sequence timer
	}
}

void AUnbelievableCharacter::WallRunEnd()
{
	isheld = false;
}

//Movement for turning side to side
void AUnbelievableCharacter::TurnAtRate(float Rate)
{
	//AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

//Movement for looking up and down
void AUnbelievableCharacter::LookUpAtRate(float Rate)
{
	if (bUseControllerRotationRoll == false)
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
		TutJustDodged = true;

		//Tutorial Dodge Timer
		GetWorldTimerManager().SetTimer(MemberTimerHandleDodge, this, &AUnbelievableCharacter::TutDodgeCheck, 0.2f, false, 0.2f);

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
		TutJustDodged = true;

		//TutorialDodgeTimer
		GetWorldTimerManager().SetTimer(MemberTimerHandleDodge, this, &AUnbelievableCharacter::TutDodgeCheck, 0.2f, false, 0.2f);

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
void AUnbelievableCharacter::TutDodgeCheck()
{
	TutJustDodged = false;
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
					GetUniqueID() && Hit.Actor->ActorHasTag(TEXT("Wall")))
				{
					HitLocation = Hit.Location;
					HitNormal = Hit.Normal;
					MinDistance = (Hit.Location - TraceStart).Size();
					id = Hit.GetActor();
				}
			}
		}

		//Checks if the player should jump from the wall and launches them
		if (HitLocation != FVector::ZeroVector)
		{
			isheld = false;
			WallClimb = true;
			GetCharacterMovement()->AirControl = 1;
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(MyShake, 0.5f);
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
		WallClimb = true;
		GetCharacterMovement()->AirControl = SingleJumpControl;
		ACharacter::LaunchCharacter(FVector(0, 0, JumpHeight), false, true);
		DoubleJumpCounter++;
	}
	else if (DoubleJumpCounter == 1 && DisableSpecialMovement)
	{
		WallClimb = true;
		GetCharacterMovement()->AirControl = DoubleJumpControl;
		ACharacter::LaunchCharacter(FVector(0, 0, JumpHeight), false, true);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(MyShake, 0.5f);
		DoubleJumpCounter++;
	}
}

//Detects when the players touches the ground
void AUnbelievableCharacter::Landed(const FHitResult& Hit)
{
	WallClimb = true;
	DoubleJumpCounter = 0;
	id = this;
	amountForCameraToShakeOnLand = rateOfShakePerSecondOfDescent * timeSpentInAir;
	if (shouldShake)
	{
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->PlayCameraShake(MyShake, amountForCameraToShakeOnLand);
	}
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

void AUnbelievableCharacter::setAllVariablesNecessaryFromBlueprint(int numberOfDeathsToAdd, int indexOfGameMode, int indexOfLevel)
{
	setDeathCountFromBlueprint(numberOfDeathsToAdd);
	setModeForSave(indexOfGameMode);
	setLevelBeingPlayed(indexOfLevel);
}

void AUnbelievableCharacter::setNumberOfDeaths(int numberOfDeathsToAdd)
{
	ref_NumberofDeaths += numberOfDeathsToAdd;
}

void AUnbelievableCharacter::setDeathCountFromBlueprint(int numberOfDeaths)
{
	ref_NumberofDeaths = numberOfDeaths;
}

void AUnbelievableCharacter::setModeForSave(int indexOfGameMode)
{
	switch (indexOfGameMode)
	{
		case 1:
			ref_GameMode = "Exploration";
			break;
		case 2:
			ref_GameMode = "Core";
			break;
		case 3:
			ref_GameMode = "Hard";
			break;
		case 4:
			ref_GameMode = "Shadow";
			break;
		
		default:
			ref_GameMode = "No bueno";
			break;
	}
}

void AUnbelievableCharacter::setLevelBeingPlayed(int levelOfIndex)
{
	ref_LevelBeingPlayed = levelOfIndex;
}

void AUnbelievableCharacter::tempSaveGame()
{
	UUnbelievable_SaveGame* SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::CreateSaveGameObject(UUnbelievable_SaveGame::StaticClass()));
	SaveGameInstance->PlayerLocation = this->GetActorLocation();
	FDateTime dateTime=dateTime.Now();
	int month = dateTime.GetMonth();
	int day = dateTime.GetDay();
	FString dateOfPlaySession = FString::FromInt(month);
	dateOfPlaySession.Append("/");
	dateOfPlaySession.Append(FString::FromInt(day));
	dateOfPlaySession.Append("/");
	dateOfPlaySession.Append(FString::FromInt(dateTime.GetYear()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("The day found is: %s"), *dateOfPlaySession));
	SaveGameInstance->dateOfLastPlay = dateOfPlaySession;
	SaveGameInstance->numberOfDeathes = ref_NumberofDeaths;
	SaveGameInstance->gameTimeTotal = float_TimeSpentInGame;
	SaveGameInstance->modeOfPlay = ref_GameMode;
	SaveGameInstance->levelPlayed = ref_LevelBeingPlayed;
	switch (indexOfSaveFile)
	{
	case 0:
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot0"), 0);
		break;
	case 1:
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot1"), 0);
		break;
	case 2:
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot2"), 0);
		break;
	case 3:
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot3"), 0);
		break;
	case 4:
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot4"), 0);
		break;
	}
	//UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot"), 0);
	//FString command = FString(TEXT("HighResShot 1"));
	//if (GEditor)
	//{
	//	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("We made it to G Editor"));
	//	UWorld* world = GEditor->GetEditorWorldContext().World();

	//	if (world)
	//	{
	//		GEditor->Exec(world, *command);
	//	}
	//	else
	//	{
	//		//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("Photoshoot"));
	//	}
	//}
	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("Saved Game"));
}
void AUnbelievableCharacter::SaveGame()
{
	UUnbelievable_SaveGame* SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::CreateSaveGameObject(UUnbelievable_SaveGame::StaticClass()));
	SaveGameInstance->PlayerLocation = locationToSet;
	FDateTime dateTime = dateTime.Now();
	int month = dateTime.GetMonth();
	int day = dateTime.GetDay();
	FString dateOfPlaySession = FString::FromInt(month);
	dateOfPlaySession.Append("/");
	dateOfPlaySession.Append(FString::FromInt(day));
	dateOfPlaySession.Append("/");
	dateOfPlaySession.Append(FString::FromInt(dateTime.GetYear()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("The day found is: %s"), *dateOfPlaySession));
	SaveGameInstance->dateOfLastPlay = dateOfPlaySession;
	SaveGameInstance->numberOfDeathes = ref_NumberofDeaths;
	SaveGameInstance->gameTimeTotal = float_TimeSpentInGame;
	SaveGameInstance->modeOfPlay = ref_GameMode;
	SaveGameInstance->levelPlayed = ref_LevelBeingPlayed;
	switch (indexOfSaveFile)
	{
	case 0:
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot0"), 0);
		break;
	case 1:
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot1"), 0);
		break;
	case 2:
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot2"), 0);
		break;
	case 3:
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot3"), 0);
		break;
	case 4:
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot4"), 0);
		break;
	}
	//UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySlot"+indexOfSaveFile), 0);
	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("Saved Game"));
}
void AUnbelievableCharacter::LoadGame()
{
	UUnbelievable_SaveGame* SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::CreateSaveGameObject(UUnbelievable_SaveGame::StaticClass()));
	switch (indexOfSaveFile)
	{
	case 0:
		SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::LoadGameFromSlot("MySlot0", 0));
		break;
	case 1:
		SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::LoadGameFromSlot("MySlot1", 0));
		break;
	case 2:
		SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::LoadGameFromSlot("MySlot2", 0));
		break;
	case 3:
		SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::LoadGameFromSlot("MySlot3", 0));
		break;
	case 4:
		SaveGameInstance = Cast<UUnbelievable_SaveGame>(UGameplayStatics::LoadGameFromSlot("MySlot4", 0));
		break;
	}
	this->SetActorLocation(SaveGameInstance->PlayerLocation);
	ref_NumberofDeaths = SaveGameInstance->numberOfDeathes;
	float_TimeSpentInGame = SaveGameInstance->gameTimeTotal;
	ref_GameMode = SaveGameInstance->modeOfPlay;
	ref_LevelBeingPlayed = SaveGameInstance->levelPlayed;
	//GEngine->AddOnScreenDebugMessage(-1, 15, FColor::Red, TEXT("Loaded Game"));
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