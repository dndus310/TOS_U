// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeMonster.h"
#include "Monster.h"
#include "MyMonster/AIController/MonsterAIController.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "AnimInst/MeleeAnimInstance.h"
#include "MyComponent/StatusComponent.h"
#include "MyComponent/CollisionComponent.h"
#include "MyDamageType/MeleeLightDMG.h"
#include "MyDamageType/MeleeHeavyDMG.h"
#include "MyDamageType/MeleeCriticalDMG.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AMeleeMonster::AMeleeMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}
FVector AMeleeMonster::GetUnitSwordLocation_Implementation(FName SocketName) const
{
	return GetMesh()->GetSocketLocation(SocketName);
}
float AMeleeMonster::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	if (Status->GetDead() || !DamageCauser->ActorHasTag("Player"))
	{
		return 0.f;
	}

	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (DamageEvent.GetTypeID() == FPointDamageEvent::ClassID) // DamegeType 유형
	{
		FPointDamageEvent* Event = (FPointDamageEvent*)&DamageEvent;

		if (DamageCauser->ActorHasTag("Player"))
		{
			if (Event->DamageTypeClass == UMeleeLightDMG::StaticClass())
			{
				Status->ExcuteDamaged(Damage, EDamagedType::MeleeLight);
			}
			else if (Event->DamageTypeClass == UMeleeHeavyDMG::StaticClass())
			{
				Status->ExcuteDamaged(Damage, EDamagedType::MeleeHeavy);
			}
			else if (Event->DamageTypeClass == UMeleeCriticalDMG::StaticClass())
			{
				Status->ExcuteDamaged(Damage, EDamagedType::MeleeCritical);
			}
			if (Status->GetDead())
			{
				return 0.f;
			}
			GetWorldTimerManager().SetTimer(PushHandler, this, &AMeleeMonster::EndPushLocation, 0.05f, false);
			SetMonsterState(EMonsterState::Hit);
			HitDir = DamageCauser->GetActorForwardVector();
			float HitAngleYaw = (DamageCauser->GetActorLocation() - GetActorLocation()).Rotation().Yaw;
			HitAngleYaw = UKismetMathLibrary::ClampAxis(HitAngleYaw);
			StartHitEvent(HitAngleYaw);

			Collision->SetCollisionCheck(false);
			GetCharacterMovement()->MaxWalkSpeed = 0.f;
			GetCharacterMovement()->Velocity = FVector::ZeroVector;
		}

	}

	return 0.0f;
}
// Called when the game starts or when spawned
void AMeleeMonster::BeginPlay()
{
	Super::BeginPlay();

	CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
	Status->OnDeadEvent.BindUObject(this, &AMeleeMonster::OnDeadEvent);
	
	Status->SetUnitStatus(100.f, 5.f, 50.f, 10.f); // 임시

	MonsterAIC = Cast<AMonsterAIController>(GetController());
	Target = FindTarget(TEXT("Player"));
	BlackBoardSetting();

	TArray<FName> AllSocketName = GetMesh()->GetAllSocketNames();

	for (auto iter : AllSocketName)
	{
		if (iter.ToString().Contains("Socket"))
		{
			SocketNames.Emplace(iter);
		}
	}
	Collision->InitCollisionSocket(SocketNames);
}
void AMeleeMonster::StartAttack()
{
	Super::StartAttack();

	if (GetCurrentMontage() == DeadMontage ||
		GetCurrentMontage() == HitMontage)
	{
		return;
	}
	PlayAnimMontage(BaseAtkMontage, 1.f);
}
void AMeleeMonster::StartHitEvent(float AngleYaw)
{
	if (AngleYaw > 135.f && AngleYaw < 225.f)
	{
		PlayAnimMontage(HitMontage, 1.f, TEXT("Hit_B"));
	}
	else if (AngleYaw > 45.f && AngleYaw < 135.f)
	{
		PlayAnimMontage(HitMontage, 1.f, TEXT("Hit_R"));
	}
	else if (AngleYaw > 225.f && AngleYaw < 315.f)
	{
		PlayAnimMontage(HitMontage, 1.f, TEXT("Hit_L"));
	}
	else
	{
		PlayAnimMontage(HitMontage, 1.f, TEXT("Hit_F"));
	}
}
void AMeleeMonster::BlackBoardSetting()
{
	Super::BlackBoardSetting();

	MonsterAIC->SetTarget(Target);
	MonsterAIC->SetSlefActor(this);
}
void AMeleeMonster::StartSimulate()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}
// Called every frame
void AMeleeMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}
// Called to bind functionality to input
void AMeleeMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMeleeMonster::OnDeadEvent()
{
	Super::OnDeadEvent();
	ResetVariables();
	StopAnimMontage(GetCurrentMontage());
	PlayAnimMontage(DeadMontage, 1.f);
	SetMonsterState(EMonsterState::Dead);
	PrimaryActorTick.bCanEverTick = false;
	Status->OnDeadEvent.Unbind();
}
