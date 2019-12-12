// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "MyComponent/CollisionComponent.h"
#include "MyComponent/ComboSystemComponent.h"
#include "MyComponent/StatusComponent.h"
#include "MyMonster/AIController/MonsterAIController.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyDamageType/MeleeLightDMG.h"
#include "MyDamageType/MeleeHeavyDMG.h"
#include "MyDamageType/MeleeCriticalDMG.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ComboSystem = CreateDefaultSubobject<UComboSystemComponent>(TEXT("ComboSystem"));
	Collision = CreateDefaultSubobject<UCollisionComponent>(TEXT("Collision"));
	Status = CreateDefaultSubobject<UStatusComponent>(TEXT("UStatusComponent"));
}
void AMonster::OnTestTimer()
{
	SetMonsterState(EMonsterState::Chase);

	GetWorldTimerManager().ClearTimer(TimeHandler);
}
// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	Status->OnDeadEvent.BindUObject(this, &AMonster::OnDeadEvent);
	Collision->OnHitEvent.BindUObject(this, &AMonster::OnHitEvent);
	
	//GetWorldTimerManager().SetTimer(TimeHandler, this, &AMonster::OnTestTimer, 10.f, false);
	RotationRateYaw = GetCharacterMovement()->RotationRate.Yaw;
}
float AMonster::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	return 0.f;
}
float AMonster::AngleToRateTime()
{
	return 1 / (RotationRateYaw / 45.f);
}
void AMonster::OnHitEvent(FHitResult HitResult)
{
	SendDamage(HitResult);
}
// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Status->GetDead())
	{
		return;
	}

	//UE_LOG(LogTemp, Error, TEXT("The value of 'GetTargetDistXY' is: %f"), GetTargetDistXY());

	FVector TargetDir = Target->GetActorLocation() - GetActorLocation();
	TargetDir.Normalize();
	TargetDir.Z = 0.f;

	/*DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f,
		FColor::Red, false, -1.f, 1, 3.f);

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + TargetDir * 100.f,
		FColor::Green, false, -1.f, 1, 3.f);*/

	OnPushLocation();
	SmoothLookat();
}
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void AMonster::StartAttack()
{
	if (GetCurrentMontage() == DeadMontage ||
		GetCurrentMontage() == HitMontage)
	{
		return ;
	}
	StopAnimMontage(GetCurrentMontage());
}
bool AMonster::CheckUnitDead_Implementation() const
{
	return Status->GetDead();
}
void AMonster::BlackBoardSetting()
{
	SetMonsterState(EMonsterState::Chase);
}
AActor* AMonster::FindTarget(FName TargetName)
{
	for (TActorIterator<AActor> iter(GetWorld()); iter; ++iter)
	{
		if ((*iter)->ActorHasTag(TargetName))
		{
			return (*iter);
		}
	}
	return nullptr;
}
EMonsterState AMonster::GetMonsterState() const
{
	return (EMonsterState)MonsterAIC->GetCurrentState();
}
bool AMonster::IsMonsterState(EMonsterState State)
{
	return MonsterAIC->GetCurrentState() == (uint8)State ? true : false;
}

float AMonster::GetTargetDistXY() const
{
	return FVector::DistXY(GetActorLocation(), Target->GetActorLocation());
}
void AMonster::SetMonsterState(EMonsterState State)
{
	MonsterAIC->SetCurrentState(State);
}
void AMonster::ResetVariables()
{
	IsLookat = false;
	LookDir = FVector::ZeroVector;
	GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
	//HitDir = FVector::ZeroVector;
	IsHitEnd = false;
	IsAttackEnd = false;
}
bool AMonster::CheckAngle(float MaxAngle)
{
	FVector TargetDir = Target->GetActorLocation() - GetActorLocation();
	TargetDir.Normalize();

	float DeltaYaw = TargetDir.Rotation().Yaw -GetActorRotation().Yaw;
	DeltaYaw = UKismetMathLibrary::NormalizeAxis(DeltaYaw);
	GetCharacterMovement()->RotationRate = FRotator(0.f, abs(DeltaYaw) * (MaxAngle * 0.1f), 0.f);

	if (abs(DeltaYaw) > MaxAngle)
	{
		return true;
	}
	return false;
}

bool AMonster::CheckRadius(float MaxRadius)
{
	float CurrentRadius = FVector::DistXY(Target->GetActorLocation(), GetActorLocation());

	return (CurrentRadius < MaxRadius) ? true : false;
}
bool AMonster::CheckTargetDead()
{
	bool Result = false;
	if (Target->GetClass()->ImplementsInterface(UUnitInterface::StaticClass()))
	{
		Result = IUnitInterface::Execute_CheckUnitDead(Target);
	}
	return Result;
}
void AMonster::StartLookat()
{
	if (!IsLookat)
	{
		IsLookat = true;
		GetWorldTimerManager().SetTimer(LookatHandler, this, &AMonster::OnLookatTime, AngleToRateTime(), false);
	}
}

void AMonster::OnLookatTime()
{
	LookDir = FVector::ZeroVector;
	GetWorldTimerManager().ClearTimer(LookatHandler);
	IsLookat = false;
	StartAttack();
}

void AMonster::SmoothLookat()
{
	if (IsLookat)
	{
		FRotator NextRotation = UKismetMathLibrary::RLerp(GetActorRotation(), LookDirection().Rotation(), AngleToRateTime(), true);
		SetActorRotation(NextRotation);
	}
}
void AMonster::OnDeadEvent()
{
	

}
void AMonster::SendDamage(FHitResult HitResult)
{
	switch (CurrentDamagedType)
	{
	case EDamagedType::MeleeLight:
	{
		TSubclassOf<UMeleeLightDMG> LightDMG = UMeleeLightDMG::StaticClass();
		UGameplayStatics::ApplyPointDamage(HitResult.GetActor(),
			Status->GetBindDamage(LightDMG.GetDefaultObject()->DamageAmount),
			HitResult.ImpactPoint, HitResult, GetController(), this, UMeleeLightDMG::StaticClass());
		break;
	}
	case EDamagedType::MeleeHeavy:
	{
		TSubclassOf<UMeleeHeavyDMG> HeavyDMG = UMeleeHeavyDMG::StaticClass();
		UGameplayStatics::ApplyPointDamage(HitResult.GetActor(),
			Status->GetBindDamage(HeavyDMG.GetDefaultObject()->DamageAmount), 
			HitResult.ImpactPoint, HitResult, GetController(), this, UMeleeHeavyDMG::StaticClass());
		break;
	}
	case EDamagedType::MeleeCritical:
	{
		TSubclassOf<UMeleeCriticalDMG> CriticalDMG = UMeleeCriticalDMG::StaticClass();
		UGameplayStatics::ApplyPointDamage(HitResult.GetActor(),
			Status->GetBindDamage(CriticalDMG.GetDefaultObject()->DamageAmount),
			HitResult.ImpactPoint, HitResult, GetController(), this, UMeleeCriticalDMG::StaticClass());
		break;
	}
	default:
		break;
	}
}
FVector AMonster::LookDirection()
{
	FVector vDir = Target->GetActorLocation() - GetActorLocation();
	vDir.Z = 0.f;
	vDir.Normalize();

	return vDir;
}
void AMonster::OnPushLocation()
{
	if (HitDir.Size() != 0.f)
	{
		HitDir.Z = 0.f;

		SetActorLocation(GetActorLocation() + HitDir * 1500.f  * GetWorld()->DeltaTimeSeconds);
	}
}

void AMonster::EndPushLocation()
{
	GetWorldTimerManager().ClearTimer(PushHandler);
	HitDir = FVector::ZeroVector;
}