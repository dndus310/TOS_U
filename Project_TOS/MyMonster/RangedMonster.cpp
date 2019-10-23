// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedMonster.h"
#include "MyMonster/AIController/MonsterAIController.h"
#include "MyMonster/RangeBullet.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "AnimInst/RangedAnimInstance.h"
#include "MyComponent/StatusComponent.h"
#include "MyComponent/CollisionComponent.h"
#include "MyDamageType/MeleeLightDMG.h"
#include "MyDamageType/MeleeHeavyDMG.h"
#include "MyDamageType/MeleeCriticalDMG.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"



// Sets default values
ARangedMonster::ARangedMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}
FVector ARangedMonster::GetUnitSwordLocation_Implementation(FName SocketName) const
{
	return FVector();
}
// Called when the game starts or when spawned
void ARangedMonster::BeginPlay()
{
	Super::BeginPlay();

	CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
	Status->OnDeadEvent.BindUObject(this, &ARangedMonster::OnDeadEvent);

	Status->SetUnitStatus(100.f, 5.f, 50.f, 10.f); // юс╫ц

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
void ARangedMonster::StartAttack()
{
	Super::StartAttack();

	if (GetTargetDistXY() < 150.f)
	{
		PlayAnimMontage(BaseAtkMontage, 1.f, "Atk1");
	}
	else if (GetTargetDistXY() < 250.f)
	{
		PlayAnimMontage(BaseAtkMontage, 1.f, "Atk2");
	}
}
void ARangedMonster::StartHitEvent(float AngleYaw)
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
void ARangedMonster::BlackBoardSetting()
{
	Super::BlackBoardSetting();

	MonsterAIC->SetTarget(Target);
	MonsterAIC->SetSlefActor(this);
}
void ARangedMonster::OnDeadEvent()
{
	Super::OnDeadEvent();
	ResetVariables();
	StopAnimMontage(GetCurrentMontage());
	PlayAnimMontage(DeadMontage, 1.f);
	SetMonsterState(EMonsterState::Dead);
	PrimaryActorTick.bCanEverTick = false;
	Status->OnDeadEvent.Unbind();
}
void ARangedMonster::SpwanFire()
{
	GetWorld()->SpawnActor<ARangeBullet>(ARangeBullet::StaticClass(), GetMesh()->GetSocketTransform(TEXT("Fire")));
}
// Called every frame
void ARangedMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
// Called to bind functionality to input
void ARangedMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

