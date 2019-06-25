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
	UFUNCTION(BlueprintCallable, Category = "On Land shake method")
	void Landed(const FHitResult& Hit) override;
	void Debug();
private:
	void tempSaveGame();
	UFUNCTION(BlueprintCallable,Category="Save System Functionality")
	void SaveGame();
	UFUNCTION(BlueprintCallable, Category = "Save System Functionality")
	void LoadGame();
public:

	bool isheld = false;
	void DodgeCooldown();
	struct FTimerHandle MemberTimerHandle;
	struct FTimerHandle MemberTimerHandle2;
	struct FTimerHandle MemberTimerHandle3;

	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UPROPERTY(BlueprintReadWrite)
		bool DisableSpecialMovement = true;

	UPROPERTY(BlueprintReadWrite)
		bool shouldShake = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Location of Saving")
		FVector locationToSet;

	bool WallClimb = true;
	bool WallClimb2 = true;

	UFUNCTION()
		void RunFall();

	bool WallRunCooldown = true;

	void cool_down();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int indexOfSaveFile;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float float_TimeSpentInGame;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int ref_NumberofDeaths;
	
	UFUNCTION(BlueprintCallable, Category = "Save System Functionality")
		void setModeForSave(int indexOfGameMode);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ref_GameMode;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ref_LevelBeingPlayed;
	
	UFUNCTION(BlueprintCallable, Category = "Save System Functionality")
		void setLevelBeingPlayed(int indexOfLevel);

	struct FTimerHandle MemberTimerHandle4;
	void reset_camera_rotation();

};