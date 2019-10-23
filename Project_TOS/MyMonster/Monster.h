// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyComponent/StatusComponent.h"
#include "MyInterface/UnitInterface.h"
#include "Monster.generated.h"

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Default = 0,
	Attack = 1,
	Chase = 2,
	Strafe = 3,
	BackStep = 4,
	Stun = 5,
	Hit = 6,
	Dead = 7,
	End = 8,
};
UCLASS()
class PROJECT_TOS_API AMonster : public ACharacter, public IUnitInterface
{
	GENERATED_BODY()
	// Sets default values for this actor's properties
		
public:
	AMonster();
protected:
	TArray<FName> SocketNames;
	EDamagedType CurrentDamagedType = EDamagedType::MeleeLight;
	FRotator CurrentRotator;
	FTimerHandle LookatHandler;
	FTimerHandle TimeHandler;
	FTimerHandle PushHandler;
	FVector HitDir = FVector::ZeroVector;
	FVector LookDir = FVector::ZeroVector;
	bool IsLookat = false;
	AActor* Target;
	class AMonsterAIController* MonsterAIC;
	float RotationRateYaw = 0.f;

	bool IsAttackEnd = false;
	bool IsHitEnd = false;
protected:
	void OnTestTimer();
	float AngleToRateTime();
	void OnHitEvent(FHitResult HitResult);
public:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void StartAttack();
	virtual void StartHitEvent(float AngleYaw) {};
	virtual void BlackBoardSetting();
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CheckUnitDead() const;
	virtual bool CheckUnitDead_Implementation() const override;

public:
	AActor* FindTarget(FName TargetName);
	bool CheckAngle(float MaxAngle);
	bool CheckRadius(float MaxRadius);
	bool CheckTargetDead();
	void StartLookat();
	void OnLookatTime();
	void SmoothLookat();
protected:
	virtual void OnDeadEvent();
	void SendDamage(FHitResult HitResult);
protected:
	FVector LookDirection();
	void OnPushLocation();
	void EndPushLocation();
public:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (Displayname = "ComboSystemComponent"))
	class UComboSystemComponent* ComboSystem;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (Displayname = "CollisionComponent"))
	class UCollisionComponent* Collision;

	UPROPERTY(EditAnywhere, meta = (Displayname = "StatusComponent"))
	class UStatusComponent* Status;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentSpeed = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBehaviorTree* MonsterAI;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* DeadMontage;
public:
	AActor* GetTarget() const { return Target; }
	UBehaviorTree* GetAI() const { return MonsterAI; }
	AMonsterAIController* GetAIC() const { return MonsterAIC; }
	EMonsterState GetMonsterState() const;
	bool IsMonsterState(EMonsterState State);
	bool GetAttackEnd() const { return IsAttackEnd; }
	bool GetHitEnd() const { return IsHitEnd; }
	bool GetIsLookat() const { return IsLookat; }
	float GetTargetDistXY() const;
public:
	void SetAttackEnd(bool Value) { IsAttackEnd = Value; }
	void SetHitEnd(bool Value) { IsHitEnd = Value; }
	void SetMonsterState(EMonsterState State);
	void ResetVariables();
};
