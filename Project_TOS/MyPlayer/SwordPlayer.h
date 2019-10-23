// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyComponent/StatusComponent.h"
#include "MyInterface/UnitInterface.h"
#include "SwordPlayer.generated.h"

UENUM()
enum class AttackButtonState : uint8
{
	Default = 0,
	LATK = 1,
	RATK = 2,
};

UCLASS()
class PROJECT_TOS_API ASwordPlayer : public ACharacter, public IUnitInterface
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

	void MovePush();

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetUnitSwordLocation(FName SocketName = "") const;
	virtual FVector GetUnitSwordLocation_Implementation(FName SocketName = "") const override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CheckUnitDead() const;
	virtual bool CheckUnitDead_Implementation() const override;


	UFUNCTION(Server, Reliable)
	void CtoS_GetUnitSwordLocation();
	void CtoS_GetUnitSwordLocation_Implementation();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	void OnHitEvent(FHitResult HitResult);
	void OnDeadEvent();

	void StartPoint();

	UFUNCTION(Server, Reliable)
	void CtoS_StartPoint();
	void CtoS_StartPoint_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void StoA_StartPoint();
	void StoA_StartPoint_Implementation();


	
	void ApplyDead();
	
	UFUNCTION(Server, Reliable)
	void CtoS_ApplyDead();
	void CtoS_ApplyDead_Implementation();
	
	UFUNCTION(NetMulticast, Reliable)
	void StoA_ApplyDead();
	void StoA_ApplyDead_Implementation();

	void SendDamage(FHitResult HitObject);
	void OnLookatTime();
	void OnResetAnimRateScale();
	void StartPause();
	bool IsPause;
	FTimerHandle FPausehandle;
private:
	void NextAttack(AttackButtonState State);

	UFUNCTION(Client, Reliable)
	void CtoS_NextAttack(AttackButtonState State);
	void CtoS_NextAttack_Implementation(AttackButtonState State);

	UFUNCTION(NetMulticast, Reliable)
	void StoA_NextAttack(AttackButtonState State);
	void StoA_NextAttack_Implementation(AttackButtonState State);

	void CameraZoom();
	void DebugRay();

	UFUNCTION(Server, Unreliable)
	void CtoS_SprintRate();
	void CtoS_SprintRate_Implementation();

	void SprintRate();
	void SmoothLookat();
	void InitializeValue();
public:
	void WeaponSetParents(FString SocketName);

	UFUNCTION(Server, Reliable)
	void CtoS_WeaponSetParents();
	void CtoS_WeaponSetParents_Implementation();

	FString TestSocKetName;

	void ResetVariables();

	UFUNCTION(Server, Reliable)
	void CtoS_ResetVariables();
	void CtoS_ResetVariables_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void StoA_ResetVariables();
	void StoA_ResetVariables_Implementation();

	void Movement();
	
	UFUNCTION(Server, Reliable)
	void CtoS_Movement();
	void CtoS_Movement_Implementation();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FTimerHandle PushHandle;
	void OnPushLocation();
	void EndPushLocation();

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

	UPROPERTY(EditAnywhere, meta = (Displayname = "StatusComponent"))
		class UStatusComponent* Status;

	class UPlayerAnimInstance* PlayerAnimInst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (ClampMin = "-15.0", ClampMax = "-30.0", UIMin = "-15.0", UIMax = "-30.0"))
		float MinWheel;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (ClampMin = "15.0", ClampMax = "50.0", UIMin = "15.0", UIMax = "50.0"))
		float MaxWheel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (ClampMin = "0.1", ClampMax = "5.0", UIMin = "0.1", UIMax = "5.0"))
		float WheelSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementValue", meta = (ClampMin = "100.0", ClampMax = "500.0", UIMin = "100.0", UIMax = "500.0"))
	float NormalSpeed = 200.f;

	float SprintSpeed = 400.0f;
/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementValue", meta = (ClampMin = "1.0", ClampMax = "3.0", UIMin = "1.0", UIMax = "3.0"))
		float SprintMultiple = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementValue", meta = (ClampMin = "1.0", ClampMax = "5.0", UIMin = "1.0", UIMax = "5.0"))
		float SprintRateMax = 1.f;
*/
	float SmoothRotationValue = 0.1f;
	FVector StartPosition;

	class ABattlePC* PlayerPC = NULL;
	class ABattlePS* PlayerPS = NULL;
private:
	bool bCanInit = false;
	AttackButtonState CurrentAttackState;
	FString CurrentManualKey;
	FVector InputDirection = FVector::ZeroVector;
	FVector HitDir;

	EDamagedType CurrentDamagedType;
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
	void Wheel(float Value);
	void InputForward(float Value);

	UFUNCTION(Server, Reliable)
	void CtoS_SetInputForward(float Value);
	void CtoS_SetInputForward_Implementation(float Value);

	void InputRight(float Value);

	UFUNCTION(Server, Reliable)
	void CtoS_SetInputRight(float Value);
	void CtoS_SetInputRight_Implementation(float Value);

	
	void Yaw(float Value);
	void Pitch(float Value);
	
	void Sprint();
	
	UFUNCTION(Server, Reliable)
	void CtoS_Sprint();
	void CtoS_Sprint_Implementation();

	void UnSprint();

	UFUNCTION(Server, Reliable)
	void CtoS_UnSprint();
	void CtoS_UnSprint_Implementation();
	
	void LeftAttack();

	UFUNCTION(Server, Reliable)
	void CtoS_LeftAttack();
	void CtoS_LeftAttack_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void StoA_LeftAttack();
	void StoA_LeftAttack_Implementation();

	void UnLeftAttack();
	
	void RightAttack();

	UFUNCTION(Server, Reliable)
	void CtoS_RightAttack();
	void CtoS_RightAttack_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void StoA_RightAttack();
	void StoA_RightAttack_Implementation();


	void UnRightAttack();
	
	void Draw();

	UFUNCTION(Server, Reliable)
	void CtoS_Draw();
	void CtoS_Draw_Implementation();
	
	UFUNCTION(NetMulticast, Reliable)
	void StoA_Draw();
	void StoA_Draw_Implementation();


	void Roll();

	UFUNCTION(Server, Reliable)
	void CtoS_Roll();
	void CtoS_Roll_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void StoA_Roll();
	void StoA_Roll_Implementation();

	void BackStep();

	UFUNCTION(Server, Reliable)
	void CtoS_BackStep();
	void CtoS_BackStep_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void StoA_BackStep();
	void StoA_BackStep_Implementation();

	void StartHitEvent(float AngleYaw);

	UFUNCTION(Server, Reliable)
	void CtoS_StartHitEvent(float AngleYaw);
	void CtoS_StartHitEvent_Implementation(float AngleYaw);

	UFUNCTION(NetMulticast, Reliable)
	void StoA_StartHitEvent(float AngleYaw);
	void StoA_StartHitEvent_Implementation(float AngleYaw);

public:
	void NextComboAttack() { NextAttack(CurrentAttackState); }
	bool GetIsSprint() const { return IsSprint; }
	bool GetIsDraw() const { return IsDraw; }
	bool GetIsComboStart() const { return IsComboStart; }
	bool GetIsRolling() const { return IsRolling; }
	bool GetIsSaveAttack() const { return IsSaveAttack; }
	UAnimMontage* GetLAttackMontage() const { return BaseAtkMontage; }
	EDamagedType GetDamagedType() const { return CurrentDamagedType; }
public:
	void SetIsCollisionState(bool Value) { IsCollisionState = Value; }
	void SetIsComboStart(bool Value) { IsComboStart = Value; }
	void SetIsRolling(bool Value) { IsRolling = Value; }
	
	void SetIsSaveAttack(bool Value);
	
	UFUNCTION(Server, Reliable)
	void CtoS_SetIsSaveAttack(bool Value);
	void CtoS_SetIsSaveAttack_Implementation(bool Value);

	UFUNCTION(NetMulticast, Reliable)
	void StoA_SetIsSaveAttack(bool Value);
	void StoA_SetIsSaveAttack_Implementation(bool Value);

	void SetCharacterMovementMode(EMovementMode MovementMode);

	UFUNCTION(Server, Reliable)
	void CtoS_SetCharacterMovementMode(EMovementMode MovementMode);
	void CtoS_SetCharacterMovementMode_Implementation(EMovementMode MovementMode);

	UFUNCTION(NetMulticast, Reliable)
	void StoA_SetCharacterMovementMode(EMovementMode MovementMode);
	void StoA_SetCharacterMovementMode_Implementation(EMovementMode MovementMode);

	void SetCollisionPresset(FName PressetName);

	UFUNCTION(Server, Reliable)
	void CtoS_SetCollisionPresset(FName PressetName);
	void CtoS_SetCollisionPresset_Implementation(FName PressetName);

	UFUNCTION(NetMulticast, Reliable)
	void StoA_SetCollisionPresset(FName PressetName);
	void StoA_SetCollisionPresset_Implementation(FName PressetName);



	void SetDamagedType(EDamagedType Type) { CurrentDamagedType = Type; }

	void SetIsSprint(bool Value);

	UFUNCTION(NetMulticast, Reliable)
	void StoA_SetIsSprint(bool Value);
	void StoA_SetIsSprint_Implementation(bool Value);


};
