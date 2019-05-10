#include "UnbelievableCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

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
	WallJumpTraceDistance = 200;

	id = this;
}

void AUnbelievableCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AUnbelievableCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	//Sets key binds for jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AUnbelievableCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//Sets key binds for movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AUnbelievableCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AUnbelievableCharacter::MoveRight);

	//Sets key binds for camera movement
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AUnbelievableCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AUnbelievableCharacter::LookUpAtRate);

	//Sets key binds for dodge
	PlayerInputComponent->BindAction("DodgeLeft", IE_Pressed, this, &AUnbelievableCharacter::DodgeLeft);
	PlayerInputComponent->BindAction("DodgeRight", IE_Pressed, this, &AUnbelievableCharacter::DodgeRight);
	PlayerInputComponent->BindAction("DodgeLeft", IE_Released, this, &AUnbelievableCharacter::EndDodge);
	PlayerInputComponent->BindAction("DodgeRight", IE_Released, this, &AUnbelievableCharacter::EndDodge);
}

//Detects when the players touches the ground
void AUnbelievableCharacter::Landed(const FHitResult& Hit)
{
	DoubleJumpCounter = 0;
	id = this;
}

void AUnbelievableCharacter::StopJumping()
{
	//id = this;
}

//Resets dodge after cooldown timer
void AUnbelievableCharacter::DodgeCooldown()
{
	CanDodge = true;
}

//Jump functionality, launches player into air and can be pressed twice and is reset when touching the ground
void AUnbelievableCharacter::DoubleJump()
{
	if (DoubleJumpCounter <= 1)
	{
		ACharacter::LaunchCharacter(FVector(0, 0, JumpHeight), false, true);
		DoubleJumpCounter++;
	}
}

//Movement for forward and backwards
void AUnbelievableCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

//Movement for right and left
void AUnbelievableCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
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

//Launches the player left
void AUnbelievableCharacter::DodgeLeft()
{
	//checks if cooldown is active
	if (CanDodge == true)
	{
		CanDodge = false;

		//Lowers friction for smoother movement
		GetCharacterMovement()->GroundFriction = 0;

		//Finds left directiong of the player camera
		const FVector ForwardDir = FirstPersonCameraComponent->GetRightVector();
		const FVector AddForce = -(ForwardDir * 1000) + FVector(0, 0, 10) * 10;

		//Launches the player in the direction just found
		ACharacter::LaunchCharacter(AddForce, false, true);

		//Starts the cooldown timer for the dodge
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AUnbelievableCharacter::DodgeCooldown, 1.0f, false,
		                                1.0f);
	}
}

//Launches the player right
void AUnbelievableCharacter::DodgeRight()
{
	//checks if cooldown is active
	if (CanDodge == true)
	{
		CanDodge = false;

		//Lowers friction for smoother movement
		GetCharacterMovement()->GroundFriction = 0;

		//Finds right directiong of the player camera
		const FVector ForwardDir = FirstPersonCameraComponent->GetRightVector();
		const FVector AddForce = (ForwardDir * 1000) + FVector(0, 0, 10) * 10;

		//Launches the player in the direction just found
		ACharacter::LaunchCharacter(AddForce, false, true);

		//Starts the cooldown timer for the dodge
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AUnbelievableCharacter::DodgeCooldown, 1.0f, false,
		                                1.0f);
	}
}

//Resets the friction after the player dodges
void AUnbelievableCharacter::EndDodge()
{
	GetCharacterMovement()->GroundFriction = 1;
}

//Controls if the player should jump or wall jump
void AUnbelievableCharacter::Jump()
{
	if (!GetCharacterMovement()->IsMovingOnGround())
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
			if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_EngineTraceChannel2, TraceParams))
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
		if (HitLocation != FVector::ZeroVector)
		{
			LaunchCharacter((HitNormal * WalljumpHorizontalStrenght + FVector::UpVector * WalljumpUpwardsStrength) / 2,
			                false, true);
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
