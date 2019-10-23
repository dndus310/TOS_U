// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SwordPlayer.generated.h"

enum class AttackButtonState : uint8
{
	Default = 0,
	LATK = 1,
	RATK = 2,
};

UCLASS()
class PROJECT_TOS_API ASwordPlayer : public ACharacter
{
	GENERATED_BODY()
public:
	/*FName AttackAnimationName[(int)AttackCombo::End] =
	{
		"Padding",
		"LATK1",
		"LATK2",
		"LATK3",
		"LATK4",
	};*/
	TArray<FString> AttackSectionName =
	{
		"LATK1",
		"LATK2",
		"LATK3",
		"LATK4",
		"RATK1",
		"RATK2",
		"RATK3",
		"RATK4",
		"RATK5",
		"LRATK",
		"LRRATK",
		"LLRATK",
		"LLRRATK",
		"LLRRRATK",
	};
	TArray<FString> ManualKeyName =
	{
		"1",
		"11",
		"111",
		"1111",
		"2",
		"22",
		"222",
		"2222",
		"22221",
		"12",
		"122",
		"112",
		"1122",
		"11222",
	};
public:
	//DECLARE_MULTICAST_DELEGATE
	// Sets default values for this character's properties
	ASwordPlayer();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	void OnLookatTime();
	void OnResetAnimRateScale();
	void StartPause();
	bool IsPause;
	FTimerHandle FPausehandle;
private:
	void NextAttack(AttackButtonState State);
	void CameraZoom();
	void DebugRay();
	void SprintRate(float Deltatime);
	void SmoothLookat();
	void InitializeValue();
	void SwordCollision();
public:
	void WeaponSetParents(FString SoketName);
	void ResetVariables();
	void Movement();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FTimerHandle FLookatHandle;
	FRotator MaxRotator;

	FVector LookDirection(FVector vDir);
	FVector LookDir = FVector::ZeroVector;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* BaseAtkMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMeshComponent* Weapon;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* DrawMontage;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* RollMontage;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* BackStepMontage;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere)
		class UAnimMontage* DeadMontage;

	UPROPERTY(EditAnywhere)
		class UArrowComponent* CameraLook;

	UPROPERTY(EditAnywhere, meta = (Displayname = "ComboSystemComponent"))
		class UComboSystemComponent* ComboSystem;

	UPROPERTY(EditAnywhere, meta = (Displayname = "CollisionComponent"))
		class UCollisionComponent* Collision;

	class UPlayerAnimInstance* PlayerAnimInst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (ClampMin = "-15.0", ClampMax = "-30.0", UIMin = "-15.0", UIMax = "-30.0"))
		float MinWheel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (ClampMin = "15.0", ClampMax = "30.0", UIMin = "15.0", UIMax = "30.0"))
		float MaxWheel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (ClampMin = "0.1", ClampMax = "5.0", UIMin = "0.1", UIMax = "5.0"))
		float WheelSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementValue", meta = (ClampMin = "100.0", ClampMax = "500.0", UIMin = "100.0", UIMax = "500.0"))
		float NormalSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementValue", meta = (ClampMin = "1.0", ClampMax = "3.0", UIMin = "1.0", UIMax = "3.0"))
		float SprintMultiple = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementValue", meta = (ClampMin = "1.0", ClampMax = "5.0", UIMin = "1.0", UIMax = "5.0"))
		float SprintRateMax = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementValue", meta = (ClampMin = "250.0", ClampMax = "500.0", UIMin = "250.0", UIMax = "500.0"))
		float RunSpeed;

private:
	AttackButtonState CurrentAttackState;
	FString CurrentManualKey;
	FRotator DefaultRate = FRotator::ZeroRotator;
	FVector InputDirection = FVector::ZeroVector;
	FTransform SaveWeaponTransform = FTransform::Identity;
	TMap<FName, TArray<FVector>> SocketPositions;
	TArray<FName> SocketNames;
	TArray<AActor*> HitObjects;
	TMap<FName, FVector> SocketPosArray;

	float CurrentWheel = 0.f;
	float NextTargetArmLength = 0.f;
	float SprintPoint = 0.f;
	bool IsSprint = false;
	bool IsDraw = false;
	bool IsComboStart = false;
	bool IsRolling = false;
	bool IsInputLook = false;
	bool IsSaveAttack = false;
public:
	bool IsCollisionState = false;
private:
	void Wheel(float val);
	void InputForward(float val);
	void InputRight(float val);
	void Yaw(float val);
	void Pitch(float val);
	void Sprint();
	void UnSprint();
	void LeftAttack();
	void UnLeftAttack();
	void RightAttack();
	void UnRightAttack();
	void Draw();
	void Roll();
	void BackStep();
public:
	void NextComboAttack() { NextAttack(CurrentAttackState); }
	bool GetIsSprint() const { return IsSprint; }
	bool GetIsDraw() const { return IsDraw; }
	bool GetIsComboStart() const { return IsComboStart; }
	bool GetIsRolling() const { return IsRolling; }
	bool GetIsSaveAttack() const { return IsSaveAttack; }
public:
	void SetIsCollisionState(bool Value) { IsCollisionState = Value; }
	void SetIsComboStart(bool Value) { IsComboStart = Value; }
	void SetIsRolling(bool Value) { IsRolling = Value; }
	void SetIsSaveAttack(bool Value) { IsSaveAttack = Value; }
	class UAnimMontage* GetLAttackMontage() const { return BaseAtkMontage; }
};
