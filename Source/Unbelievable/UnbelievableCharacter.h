#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraShake.h"
#include "UnbelievableCharacter.generated.h"

class UInputComponent;

UCLASS(config = Game)

class AUnbelievableCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* R_MotionController;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* L_MotionController;

	UPROPERTY()
	AActor* id;

public:

	AUnbelievableCharacter();

	int WallJumpTraces;
	float WallJumpTraceDistance;
	float SingleJumpControl = 0.5f;
	float DoubleJumpControl = 0.3f;

	//void SingleJumpIncrement();
	//void DoubleJumpIncrement();

	void Jump();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WalljumpHorizontalStrenght;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WalljumpUpwardsStrength;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UCameraShake> MyShake;

protected:

	void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
		int healthPoints;
	UFUNCTION()
		void takeDamage(int damageAmount);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UFUNCTION()
	void DoubleJump();
	UPROPERTY()
	int DoubleJumpCounter;
	UFUNCTION()
	void WallRun();
	UFUNCTION()
	void WallRunEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpHeight;

	bool CanDodge = true;

	bool CanWallRun = false;
	bool StopSideMovement = false;

protected:

	void MoveForward(float Val);
	void MoveRight(float Val);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void DodgeLeft();
	void DodgeRight();
	void EndDodge();

protected:

	void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	void Landed(const FHitResult& Hit) override;
	void Debug();
private:

	void SaveGame();
	void LoadGame();
public:

	bool isheld = false;

	void DodgeCooldown();
	struct FTimerHandle MemberTimerHandle;

	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UPROPERTY(EditAnywhere)
		bool DisableSpecialMovement = true;

};