// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyController/BattlePC.h"
#include "Kismet/KismetMathLibrary.h"
#include "kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "MyPlayer/AnimInst/PlayerAnimInstance.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Define.h"

#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT || UE_EDITOR
#include "DrawDebugHelpers.h"
#endif // UE_EDITOR

#include "MyController/BattlePC.h"
#include "AIController.h"
#include "GameFramework/Controller.h"
#include "MyPlayerState/BattlePS.h"
#include "MyGameMode/BattleGM.h"
#include "MyComponent/ComboSystemComponent.h"
#include "MyComponent/CollisionComponent.h"
#include "MyComponent/StatusComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyDamageType/MeleeLightDMG.h"
#include "MyDamageType/MeleeHeavyDMG.h"
#include "MyDamageType/MeleeCriticalDMG.h"
#include "UnrealNetwork.h"

// Sets default values
ASwordPlayer::ASwordPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bCanInit = true;

	InitializeValue();

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraLook = CreateDefaultSubobject<UArrowComponent>(TEXT("CameraLook"));
	ComboSystem = CreateDefaultSubobject<UComboSystemComponent>(TEXT("ComboSystem"));
	Collision = CreateDefaultSubobject<UCollisionComponent>(TEXT("Collision"));
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Status = CreateDefaultSubobject<UStatusComponent>(TEXT("UStatusComponent"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);
	CameraLook->SetupAttachment(Camera);
	Weapon->SetupAttachment(GetMesh(), TEXT("Undraw"));
	
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 75.f));
	SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 400.f;

	CameraLook->SetRelativeRotation(FRotator(60, 0, 0));

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	bReplicates = true;
}
void ASwordPlayer::MovePush()
{
	if (HitDir.Size() != 0.f)
	{
		HitDir.Z = 0.f;

		SetActorLocation(GetActorLocation() + HitDir * 1000.f  * GetWorld()->DeltaTimeSeconds);
	}
}
FVector ASwordPlayer::GetUnitSwordLocation_Implementation(FName SocketName) const
{
	return Weapon->GetSocketLocation(SocketName);
}

bool ASwordPlayer::CheckUnitDead_Implementation() const
{
	return Status->GetDead();
}
void ASwordPlayer::CtoS_GetUnitSwordLocation_Implementation()
{

}
float ASwordPlayer::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (Status->GetDead())
	{
		return 0.f;
	}
	if (DamageEvent.GetTypeID() == FPointDamageEvent::ClassID) // DamegeType 유형
	{
		FPointDamageEvent* Event = (FPointDamageEvent*)&DamageEvent;

		if (Event->DamageTypeClass == UMeleeLightDMG::StaticClass())
		{
			Status->ExcuteDamaged(Damage, EDamagedType::MeleeLight);

			if (Status->GetDead())
			{
				return 0.f;
			}
			if (DamageCauser->ActorHasTag("Player"))
			{
				HitDir = DamageCauser->GetActorForwardVector();
				float HitAngleYaw = (DamageCauser->GetActorLocation() - GetActorLocation()).Rotation().Yaw;
				HitAngleYaw = UKismetMathLibrary::ClampAxis(HitAngleYaw);
				GetWorldTimerManager().SetTimer(PushHandle, this, &ASwordPlayer::EndPushLocation, 0.05f, false);
				StartHitEvent(HitAngleYaw);
			}
		}
		if (Event->DamageTypeClass == UMeleeHeavyDMG::StaticClass())
		{
			Status->ExcuteDamaged(Damage, EDamagedType::MeleeHeavy);

			if (Status->GetDead())
			{
				return 0.f;
			}
			if (DamageCauser->ActorHasTag("Player"))
			{
				HitDir = DamageCauser->GetActorForwardVector();
				float HitAngleYaw = (DamageCauser->GetActorLocation() - GetActorLocation()).Rotation().Yaw;
				HitAngleYaw = UKismetMathLibrary::ClampAxis(HitAngleYaw);

				GetWorldTimerManager().SetTimer(PushHandle, this, &ASwordPlayer::EndPushLocation, 0.05f, false);
				StartHitEvent(HitAngleYaw);
			}
		}
		if (Event->DamageTypeClass == UMeleeCriticalDMG::StaticClass())
		{
			Status->ExcuteDamaged(Damage, EDamagedType::MeleeCritical);

			if (Status->GetDead())
			{
				return 0.f;
			}
			if (DamageCauser->ActorHasTag("Player"))
			{
				HitDir = DamageCauser->GetActorForwardVector();
				float HitAngleYaw = (DamageCauser->GetActorLocation() - GetActorLocation()).Rotation().Yaw;
				HitAngleYaw = UKismetMathLibrary::ClampAxis(HitAngleYaw);
				GetWorldTimerManager().SetTimer(PushHandle, this, &ASwordPlayer::EndPushLocation, 0.05f, false);
				StartHitEvent(HitAngleYaw);
			}
		}
	}
	return 0.0f;
}
void ASwordPlayer::BeginPlay()
{
	Super::BeginPlay();

	Status->SetUnitStatus(100.f, 70.f, 50.f, 10.f); // 임시

	NormalSpeed = 200.f;

	Status->OnDeadEvent.BindUObject(this, &ASwordPlayer::OnDeadEvent);
	Collision->OnHitEvent.BindUObject(this, &ASwordPlayer::OnHitEvent);

	NextTargetArmLength = SpringArm->TargetArmLength;

	PlayerAnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	ComboSystem->InitComboStateMap(AttackSectionName, ManualKeyName);

	TArray<FName> SocketNames;

	SocketNames = Weapon->GetAllSocketNames();
	SocketNames.RemoveAt(SocketNames.Num() - 1);

	Collision->InitCollisionSocket(SocketNames);
	if (GetNetMode() == ENetMode::NM_ListenServer || 
		GetNetMode() == ENetMode::NM_DedicatedServer ||
		GetNetMode() == ENetMode::NM_Client)
	{
		SetCollisionPresset(TEXT("PlayerPresset"));
	}
	else
	{
		SetCollisionPresset(TEXT("WarpPresset"));
	}
}

// Called every frame
void ASwordPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanInit)
	{
		PlayerPC = GetController<ABattlePC>();
		PlayerPS = GetPlayerState<ABattlePS>();
		
		if (PlayerPS)
		{
			PlayerPS->SetPlayer(this);
			//UE_LOG(LogClass, Error, TEXT("The value of 'Status->GetCurrentHp()' is: %f"), Status->GetCurrentHp());
			PlayerPS->SetCurrentHP(Status->GetCurrentHp());
		}
		else
		{
			return;
		}
		bCanInit = false;
	}

	//UE_LOG(LogTemp, Error, TEXT("The value of 'Status->GetCurrentHp()' is: %f"), Status->GetCurrentHp());

	if (Status->GetDead())
	{
		return;
	}
	
	MovePush();
	SmoothLookat();
	CameraZoom();

//#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT || UE_EDITOR
//	DebugRay();
//#endif // UE_EDITOR



}

// Called to bind functionality to input
void ASwordPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (Status->GetDead())
	{
		return;
	}

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ASwordPlayer::Pitch);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ASwordPlayer::Yaw);
	PlayerInputComponent->BindAxis(TEXT("Wheel"), this, &ASwordPlayer::Wheel);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ASwordPlayer::InputForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ASwordPlayer::InputRight);
	

	//if (PlayerAnimInst != NULL)
	//{
	//	if (PlayerAnimInst->GetIsDrawState())
	//	{
	//		return;
	//	}
	//}
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ASwordPlayer::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ASwordPlayer::UnSprint);

	PlayerInputComponent->BindAction(TEXT("LAttack"), IE_Pressed, this, &ASwordPlayer::LeftAttack);
	PlayerInputComponent->BindAction(TEXT("LAttack"), IE_Released, this, &ASwordPlayer::UnLeftAttack);

	PlayerInputComponent->BindAction(TEXT("RAttack"), IE_Pressed, this, &ASwordPlayer::RightAttack);
	PlayerInputComponent->BindAction(TEXT("RAttack"), IE_Released, this, &ASwordPlayer::UnRightAttack);

	PlayerInputComponent->BindAction(TEXT("Draw"), IE_Pressed, this, &ASwordPlayer::Draw);
	PlayerInputComponent->BindAction(TEXT("Roll"), IE_Pressed, this, &ASwordPlayer::Roll);
	PlayerInputComponent->BindAction(TEXT("BackStep"), IE_Pressed, this, &ASwordPlayer::BackStep);


}

void ASwordPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	

}

void ASwordPlayer::OnPushLocation()
{
	if (HitDir.Size() != 0.f)
	{
		HitDir.Z = 0.f;

		SetActorLocation(GetActorLocation() + HitDir * 1000.f  * GetWorld()->DeltaTimeSeconds);
	}
}

void ASwordPlayer::EndPushLocation()
{
	GetWorldTimerManager().ClearTimer(PushHandle);
	HitDir = FVector::ZeroVector;
}

void ASwordPlayer::InitializeValue()
{
	MinWheel = -15.f;
	MaxWheel = 15.f;
	WheelSpeed = 3.f;
	NormalSpeed = 100.f;
	//SprintMultiple = 3.f;
}

void ASwordPlayer::SmoothLookat()
{
	if (IsInputLook)
	{
		if (LookDir.IsZero())
		{
			LookDir = LookDirection(InputDirection);
		}
		AddMovementInput(LookDir, 1.f);
	}
}
void ASwordPlayer::NextAttack(AttackButtonState State)
{
	FString SectionName = ComboSystem->EnableKey(FString::FromInt((int)State));

	//UE_LOG(LogTemp, Log, TEXT("%s"), *SectionName);


	if (SectionName == "\0")
	{

		ResetVariables();
		
		return;
	}
	if (InputDirection.Size() != 0.f)
	{
		IsInputLook = true;
		PlayerAnimInst->RootMotionMode = ERootMotionMode::IgnoreRootMotion;
		float AngleYaw = GetCharacterMovement()->RotationRate.Yaw;
		GetWorldTimerManager().SetTimer(FLookatHandle, this, &ASwordPlayer::OnLookatTime, 1 / (AngleYaw / 180.f), false);
	}

	StopAnimMontage(GetCurrentMontage());
	PlayAnimMontage(BaseAtkMontage, 1.f, FName(*SectionName));
	PlayerAnimInst->SetCurrentState(AnimState::Attack);

	
	//CtoS_NextAttack(State);

}
void ASwordPlayer::CtoS_NextAttack_Implementation(AttackButtonState State)
{
	StoA_NextAttack(State);
}
void ASwordPlayer::StoA_NextAttack_Implementation(AttackButtonState State)
{
	//FString SectionName = ComboSystem->EnableKey(FString::FromInt((int)State));

	//if (SectionName == "\0")
	//{
	//	ResetVariables();
	//	return;
	//}
	//if (InputDirection.Size() != 0.f)
	//{
	//	IsInputLook = true;
	//	PlayerAnimInst->RootMotionMode = ERootMotionMode::IgnoreRootMotion;
	//	float AngleYaw = GetCharacterMovement()->RotationRate.Yaw;
	//	GetWorldTimerManager().SetTimer(FLookatHandle, this, &ASwordPlayer::OnLookatTime, 1 / (AngleYaw / 180.f), false);
	//}

	//StopAnimMontage(GetCurrentMontage());
	//PlayAnimMontage(BaseAtkMontage, 1.f, FName(*SectionName));
	//PlayerAnimInst->SetCurrentState(AnimState::Attack);
}
void ASwordPlayer::CameraZoom()
{
	if (NextTargetArmLength > SpringArm->TargetArmLength)
	{
		SpringArm->TargetArmLength = UKismetMathLibrary::Lerp(SpringArm->TargetArmLength, NextTargetArmLength, 0.05f);
	}
	else if (NextTargetArmLength < SpringArm->TargetArmLength)
	{
		SpringArm->TargetArmLength = UKismetMathLibrary::Lerp(SpringArm->TargetArmLength, NextTargetArmLength, 0.05f);
	}
	else
	{
		NextTargetArmLength = SpringArm->TargetArmLength;
	}
}

//#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT || UE_EDITOR
//void ASwordPlayer::DebugRay()
//{
//	FVector Start = GetArrowComponent()->GetComponentLocation();
//	FVector End = FVector(InputDirection.X, InputDirection.Y, 0.f);
//	End = CameraLook->GetComponentQuat() * End;
//	End.Z = 0.f;
//	End.Normalize();
//
//	DrawDebugLine(GetWorld(), Start, Start + (GetArrowComponent()->GetForwardVector() * 50.f),
//		FColor::Red, false, -1.f, 1, 3.f);
//
//	DrawDebugLine(GetWorld(), Start, Start + End * 50.f,
//		FColor::Green, false, -1.f, 1, 3.f);
//
//	DrawDebugLine(GetWorld(), Start, Start + CameraLook->GetForwardVector() * 50.f,
//		FColor::Blue, false, -1.f, 1, 3.f);
//
//}
//#endif // UE_EDITOR

void ASwordPlayer::CtoS_SprintRate_Implementation()
{
	
}

void ASwordPlayer::SprintRate()
{
	//if (IsSprint)
	//{
	//	if (SprintPoint <= SprintRateMax)
	//	{
	//		SprintPoint += SprintMultiple * GetWorld()->DeltaTimeSeconds;
	//		SetSprintPoint(1.f + SprintPoint);
	//	}
	//}
}
void ASwordPlayer::WeaponSetParents(FString SocketName)
{
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(*SocketName));
}
void ASwordPlayer::CtoS_WeaponSetParents_Implementation()
{
	//Weapon->AttachTo(GetMesh(), FName(*TestSocKetName), EAttachLocation::SnapToTargetIncludingScale);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(*TestSocKetName));
}
void ASwordPlayer::ResetVariables()
{
	//PlayerAnimInst->SetCurrentState(AnimState::Default);

	if (IsLocallyControlled())
	{
	//	SetCustomCollisionChannels(ECollisionResponse::ECR_Block);
		CtoS_ResetVariables();
	}
}
void ASwordPlayer::CtoS_ResetVariables_Implementation()
{
	PlayerAnimInst->SetCurrentState(AnimState::Default);
	//PlayerAnimInst->SetCurrentState(AnimState::Default);

	StoA_ResetVariables();
}
void ASwordPlayer::StoA_ResetVariables_Implementation()
{
	IsComboStart = false;
	InputDirection = FVector::ZeroVector;
	IsCollisionState = false;
	IsSaveAttack = false;
	CurrentAttackState = AttackButtonState::Default;
	//PlayerAnimInst->SetCurrentState(AnimState::Default);

	SetCollisionPresset(TEXT("PlayerPresset"));

	ComboSystem->ClearKey();
	//SetComboState(AttackCombo::Default);
}
void ASwordPlayer::Movement()
{
	if (InputDirection.Size() == 0 ||
		PlayerAnimInst->IsCurrentState(AnimState::Roll) ||
		PlayerAnimInst->IsCurrentState(AnimState::Attack) ||
		PlayerAnimInst->IsCurrentState(AnimState::BackStep) ||
		(GetCharacterMovement()->GetMovementName() == "Flying"))
	{
		return;
	}
	FVector vDir = LookDirection(InputDirection);

	float DeltaYaw = vDir.Rotation().Yaw - GetActorRotation().Yaw;
	DeltaYaw = UKismetMathLibrary::NormalizeAxis(DeltaYaw);

	DeltaYaw = GetActorRotation().Yaw + (0.1f * DeltaYaw);
	vDir = UKismetMathLibrary::GetForwardVector(FRotator(0, DeltaYaw, 0));

	AddMovementInput(vDir);
	//GetCharacterMovement()->MaxWalkSpeed = NormalSpeed * ((1.f + SprintPoint));


	CtoS_Movement();

}
void ASwordPlayer::CtoS_Movement_Implementation()
{
	if (InputDirection.Size() == 0 ||
		PlayerAnimInst->IsCurrentState(AnimState::Roll) ||
		PlayerAnimInst->IsCurrentState(AnimState::Attack) ||
		PlayerAnimInst->IsCurrentState(AnimState::BackStep) ||
		(GetCharacterMovement()->GetMovementName() == "Flying"))
	{
		return;
	}
	if (!IsSprint)
	{
		
	}
	else
	{

		//GetCharacterMovement()->MaxWalkSpeed = NormalSpeed * OutSprintPoint;
	}
}
void ASwordPlayer::OnResetAnimRateScale()
{
	GetMesh()->GlobalAnimRateScale = 1.f;
	IsPause = false;
	GetWorldTimerManager().ClearTimer(FPausehandle);
}
void ASwordPlayer::StartPause()
{
	GetMesh()->GlobalAnimRateScale = 0.f;
	GetWorldTimerManager().SetTimer(FPausehandle, this, &ASwordPlayer::OnResetAnimRateScale, 0.05f, false);
	//IsPause = true;

}
void ASwordPlayer::OnHitEvent(FHitResult HitResult)
{
	SendDamage(HitResult);
}
void ASwordPlayer::OnDeadEvent()
{
	Status->OnDeadEvent.Unbind();
	ApplyDead();
}
void ASwordPlayer::StartPoint()
{
	CtoS_StartPoint();
}
void ASwordPlayer::CtoS_StartPoint_Implementation()
{
	ABattleGM* BattleGM = Cast<ABattleGM>(UGameplayStatics::GetGameMode(this));
	StartPosition = BattleGM->GetAutoStartPoint();
}
void ASwordPlayer::StoA_StartPoint_Implementation()
{
	SetActorLocation(StartPosition);
}
void ASwordPlayer::ApplyDead()
{
	CtoS_ApplyDead();
}
void ASwordPlayer::CtoS_ApplyDead_Implementation()
{
	ABattleGM* BattleGM = Cast<ABattleGM>(UGameplayStatics::GetGameMode(this));
	uint32 ControllerID = UGameplayStatics::GetPlayerControllerID(PlayerPC);
	//UE_LOG(LogClass, Error, TEXT("Log message Player Controller ID(%d)"), ControllerID);

	if (GetNetMode() == ENetMode::NM_Standalone)
	{
		//싱글일때 레벨 넘기면된다
	}
	else
	{
		BattleGM->CheckCount();
	}

	StoA_ApplyDead();
}
void ASwordPlayer::StoA_ApplyDead_Implementation()
{
	PlayerAnimInst->StopAllMontages(0.f);
	PlayAnimMontage(DeadMontage, 1.f);
	PlayerAnimInst->SetCurrentState(AnimState::Dead);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PrimaryActorTick.bCanEverTick = false;
	DisableInput(PlayerPC);
	//InputComponent->Activate(false);
}
void ASwordPlayer::SendDamage(FHitResult HitObject)
{
	switch (CurrentDamagedType)
	{
		case EDamagedType::MeleeLight:
		{
			TSubclassOf<UMeleeLightDMG> LightDMG = UMeleeLightDMG::StaticClass();
			UGameplayStatics::ApplyPointDamage(HitObject.GetActor(),
				Status->GetBindDamage(LightDMG.GetDefaultObject()->DamageAmount), 
				HitObject.ImpactPoint, HitObject, GetController(), this, UMeleeLightDMG::StaticClass());
			break;
		}
		case EDamagedType::MeleeHeavy:
		{
			TSubclassOf<UMeleeHeavyDMG> HeavyDMG = UMeleeHeavyDMG::StaticClass();
			UGameplayStatics::ApplyPointDamage(HitObject.GetActor(),
				Status->GetBindDamage(HeavyDMG.GetDefaultObject()->DamageAmount), HitObject.
				ImpactPoint, HitObject, GetController(), this, UMeleeHeavyDMG::StaticClass());
			break;
		}
		case EDamagedType::MeleeCritical:
		{
			TSubclassOf<UMeleeCriticalDMG> CriticalDMG = UMeleeCriticalDMG::StaticClass();
			UGameplayStatics::ApplyPointDamage(HitObject.GetActor(), 
				Status->GetBindDamage(CriticalDMG.GetDefaultObject()->DamageAmount),
				HitObject.ImpactPoint, HitObject, GetController(), this, UMeleeCriticalDMG::StaticClass());
			break;
		}
		default:
			break;
	}
}
void ASwordPlayer::OnLookatTime()
{
	PlayerAnimInst->RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly;
	IsInputLook = false;
	LookDir = FVector::ZeroVector;
	GetWorldTimerManager().ClearTimer(FLookatHandle);
}
FVector ASwordPlayer::LookDirection(FVector vDir)
{
	FVector Start = GetActorLocation();
	FVector End = FVector(vDir.X, vDir.Y, 0.f);
	End = CameraLook->GetComponentQuat() * End;
	End.Normalize();

	FVector ConvertDir = (Start + End * 10.f) - Start;
	ConvertDir.Normalize();

	return ConvertDir;
}

void ASwordPlayer::Wheel(float Value)
{
	if (Value < 0)
	{
		CurrentWheel -= WheelSpeed;
		CurrentWheel = UKismetMathLibrary::ClampAngle(CurrentWheel, MinWheel - 5, MaxWheel - 5);
		NextTargetArmLength -= MinWheel < CurrentWheel ? WheelSpeed * 10.f : 0.f;
	}
	if(Value > 0)
	{
		CurrentWheel += WheelSpeed;
		CurrentWheel = UKismetMathLibrary::ClampAngle(CurrentWheel, MinWheel + 5, MaxWheel + 5);
		NextTargetArmLength += MaxWheel > CurrentWheel ? WheelSpeed * 10.f : 0.f;
	}
}

void ASwordPlayer::InputForward(float Value)
{
	InputDirection.X = Value;
	CtoS_SetInputForward(Value);

	Movement();
}
void ASwordPlayer::CtoS_SetInputForward_Implementation(float Value)
{
	InputDirection.X = Value;
}
void ASwordPlayer::InputRight(float Value)
{
	InputDirection.Y = Value;
	CtoS_SetInputRight(Value);

	Movement();
}
void ASwordPlayer::CtoS_SetInputRight_Implementation(float Value)
{
	InputDirection.Y = Value;
}
void ASwordPlayer::Yaw(float Value)
{
	if (Value != 0)
	{
		AddControllerYawInput(Value);
	}
}
void ASwordPlayer::Pitch(float Value)
{
	if (Value != 0)
	{
		AddControllerPitchInput(Value);
	}
}
void ASwordPlayer::Sprint()
{
	if (GetController()->IsLocalController())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	CtoS_Sprint();
}
void ASwordPlayer::UnSprint()
{
	if (GetController()->IsLocalController())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
	CtoS_UnSprint();
}
void ASwordPlayer::CtoS_Sprint_Implementation()
{
	SetIsSprint(true);
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}
void ASwordPlayer::CtoS_UnSprint_Implementation()
{
	SetIsSprint(false);
	SprintPoint = 0.f;
	NormalSpeed = 200.f;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}
void ASwordPlayer::LeftAttack()
{
	CtoS_LeftAttack();
}
void ASwordPlayer::CtoS_LeftAttack_Implementation()
{
	StoA_LeftAttack();
}
void ASwordPlayer::StoA_LeftAttack_Implementation()
{
	if (!IsDraw ||
		PlayerAnimInst->GetIsDrawState() ||
		PlayerAnimInst->IsCurrentState(AnimState::Roll) ||
		PlayerAnimInst->IsCurrentState(AnimState::BackStep) ||
		(GetCharacterMovement()->GetMovementName() == "Flying"))
	{
		return;
	}
	CurrentAttackState = AttackButtonState::LATK;

	if (IsComboStart)
	{
		NextAttack(CurrentAttackState);
	}
	else if (!ComboSystem->AnyKey())
	{
		NextAttack(CurrentAttackState);
	}
	else if (ComboSystem->AnyKey() && !IsComboStart)
	{
		if (!IsSaveAttack)
		{
			IsSaveAttack = true;
		}
	}

}
void ASwordPlayer::UnLeftAttack()
{

}
void ASwordPlayer::RightAttack()
{

	CtoS_RightAttack();
	
}
void ASwordPlayer::CtoS_RightAttack_Implementation()
{
	StoA_RightAttack();
}
void ASwordPlayer::StoA_RightAttack_Implementation()
{
	if (!IsDraw ||
		PlayerAnimInst->GetIsDrawState() ||
		PlayerAnimInst->IsCurrentState(AnimState::Roll) ||
		PlayerAnimInst->IsCurrentState(AnimState::BackStep))
	{
		//UE_LOG(LogTemp, Log, TEXT("aaaa"));
		return;
	}
	CurrentAttackState = AttackButtonState::RATK;

	if (IsComboStart)
	{
		NextAttack(CurrentAttackState);
	}
	else if (!ComboSystem->AnyKey())
	{
		NextAttack(CurrentAttackState);
	}
	else if (ComboSystem->AnyKey() && !IsComboStart)
	{
		if (!IsSaveAttack)
		{
			IsSaveAttack = true;
		}
	}
}
void ASwordPlayer::UnRightAttack()
{

}
void ASwordPlayer::Draw()
{

	ResetVariables();
	
	
	CtoS_Draw();
}

void ASwordPlayer::CtoS_Draw_Implementation()
{
	StoA_Draw();
}

void ASwordPlayer::StoA_Draw_Implementation()
{
	if (!IsDraw)
	{
		IsDraw = true;
		PlayerAnimInst->SetIsDraw(true);
		PlayerAnimInst->ChangeLayer(LayerState::CombatLayer);
		PlayAnimMontage(DrawMontage, 1.f, TEXT("Draw"));
		GetCharacterMovement()->RotationRate = FRotator(0, 900.f, 0);
	}
	else
	{
		IsDraw = false;
		PlayerAnimInst->SetIsDraw(false);
		PlayerAnimInst->ChangeLayer(LayerState::BaseLayer);
		PlayAnimMontage(DrawMontage, 1.f, TEXT("UnDraw"));
		GetCharacterMovement()->RotationRate = FRotator(0, 540.f, 0);
	}

}

void ASwordPlayer::Roll()
{
	//GetCapsuleComponent()->SetCollisionProfileName(TEXT("Ghost"));
	SetCollisionPresset(TEXT("Ghost"));

	CtoS_Roll();
}
void ASwordPlayer::CtoS_Roll_Implementation()
{
	StoA_Roll();
}
void ASwordPlayer::StoA_Roll_Implementation()
{
	if (!PlayerAnimInst->GetIsDrawState() &&
		!PlayerAnimInst->IsCurrentState(AnimState::Roll) &&
		!PlayerAnimInst->IsCurrentState(AnimState::BackStep) &&
		!(GetCharacterMovement()->GetMovementName() == "Flying"))
	{
		IsCollisionState = false;

		PlayerAnimInst->SetCurrentState(AnimState::Roll);
		SetActorRotation(FRotator(GetActorRotation().Pitch, LookDirection(InputDirection).Rotation().Yaw, GetActorRotation().Roll));
		StopAnimMontage(GetCurrentMontage());
		PlayAnimMontage(RollMontage, 1.2f, "Roll");
	}
}
void ASwordPlayer::BackStep()
{
	CtoS_BackStep();
}
void ASwordPlayer::CtoS_BackStep_Implementation()
{
	StoA_BackStep();
}
void ASwordPlayer::StoA_BackStep_Implementation()
{
	if (!PlayerAnimInst->IsCurrentState(AnimState::Roll) &&
		!PlayerAnimInst->IsCurrentState(AnimState::BackStep) &&
		!(GetCharacterMovement()->GetMovementName() == "Flying"))
	{
		int PCIndex = UGameplayStatics::GetPlayerControllerID(PlayerPC);

		IsCollisionState = false;

		PlayerAnimInst->SetCurrentState(AnimState::BackStep);
		StopAnimMontage(GetCurrentMontage());
		PlayAnimMontage(BackStepMontage, 1.f, "BackStep");
	}
}

void ASwordPlayer::StartHitEvent(float AngleYaw)
{


	StoA_StartHitEvent(AngleYaw);
	
	//CtoS_StartHitEvent(AngleYaw);

}
void ASwordPlayer::CtoS_StartHitEvent_Implementation(float AngleYaw)
{
	//if (AngleYaw > 0.f && AngleYaw < 135.f)
	//{
	//	PlayAnimMontage(HitMontage, 1.f, TEXT("Hit_L"));
	//	//PlayAnimMontage(DeadMontage, 1.f);
	//}
	//else if (AngleYaw > 225.f && AngleYaw < 360.f)
	//{
	//	PlayAnimMontage(HitMontage, 1.f, TEXT("Hit_R"));
	//	//PlayAnimMontage(DeadMontage, 1.f);
	//}
	//else
	//{
	//	PlayAnimMontage(HitMontage, 1.f, TEXT("Hit_B"));
	//	//PlayAnimMontage(DeadMontage, 1.f);
	//}
	StoA_StartHitEvent(AngleYaw);
}
void ASwordPlayer::StoA_StartHitEvent_Implementation(float AngleYaw)
{
	if (AngleYaw > 0.f && AngleYaw < 135.f)
	{
		PlayAnimMontage(HitMontage, 1.f, TEXT("Hit_L"));
		//PlayAnimMontage(DeadMontage, 1.f);
	}
	else if (AngleYaw > 225.f && AngleYaw < 360.f)
	{
		PlayAnimMontage(HitMontage, 1.f, TEXT("Hit_R"));
		//PlayAnimMontage(DeadMontage, 1.f);
	}
	else
	{
		PlayAnimMontage(HitMontage, 1.f, TEXT("Hit_B"));
		//PlayAnimMontage(DeadMontage, 1.f);
	}
}

void ASwordPlayer::SetCharacterMovementMode(EMovementMode MovementMode)
{
	CtoS_SetCharacterMovementMode(MovementMode);
}

void ASwordPlayer::CtoS_SetCharacterMovementMode_Implementation(EMovementMode MovementMode)
{																
	StoA_SetCharacterMovementMode(MovementMode);
}																
void ASwordPlayer::StoA_SetCharacterMovementMode_Implementation(EMovementMode MovementMode)
{
	GetCharacterMovement()->SetMovementMode(MovementMode);
}

void ASwordPlayer::SetCollisionPresset(FName PressetName)
{
	if(HasAuthority())
	{
		CtoS_SetCollisionPresset(PressetName);
	}
}

void ASwordPlayer::CtoS_SetCollisionPresset_Implementation(FName PressetName)
{
	StoA_SetCollisionPresset(PressetName);
}

void ASwordPlayer::StoA_SetCollisionPresset_Implementation(FName PressetName)
{
	GetCapsuleComponent()->SetCollisionProfileName(PressetName);

}


void ASwordPlayer::SetIsSprint(bool Value)
{
	StoA_SetIsSprint(Value);
}
void ASwordPlayer::StoA_SetIsSprint_Implementation(bool Value)
{
	IsSprint = Value;
}

void ASwordPlayer::SetIsSaveAttack(bool Value)
{
	IsSaveAttack = Value;
	//CtoS_SetIsSaveAttack(Value);
}

void ASwordPlayer::CtoS_SetIsSaveAttack_Implementation(bool Value)
{
	StoA_SetIsSaveAttack(Value);
}

void ASwordPlayer::StoA_SetIsSaveAttack_Implementation(bool Value)
{
	IsSaveAttack = Value;
}