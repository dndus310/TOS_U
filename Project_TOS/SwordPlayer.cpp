// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SwordPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "PlayerAnimInstance.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"
#include "AIController.h"
#include "GameFramework/Controller.h"
#include "ComboSystemComponent.h"
#include "CollisionComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASwordPlayer::ASwordPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitializeValue();

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraLook = CreateDefaultSubobject<UArrowComponent>(TEXT("CameraLook"));
	ComboSystem = CreateDefaultSubobject<UComboSystemComponent>(TEXT("ComboSystem"));
	Collision = CreateDefaultSubobject<UCollisionComponent>(TEXT("Collision"));
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);
	CameraLook->SetupAttachment(Camera);
	//Weapon->SetupAttachment(GetMesh(), TEXT("Status"));
	Weapon->SetupAttachment(GetMesh(), TEXT("Undraw"));
	
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 75.f));
	SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 400.f;

	CameraLook->SetRelativeRotation(FRotator(60, 0, 0));


}

float ASwordPlayer::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	
	UE_LOG(LogTemp, Log, TEXT("DamageHit"));


	return 0.0f;
}

// Called when the game starts or when spawned
void ASwordPlayer::BeginPlay()
{
	Super::BeginPlay();

	NextTargetArmLength = SpringArm->TargetArmLength;

	//GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	PlayerAnimInst = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	DefaultRate = GetCharacterMovement()->RotationRate;

	SaveWeaponTransform = Weapon->GetRelativeTransform();

	ComboSystem->InitComboStateMap(AttackSectionName, ManualKeyName);

	SocketNames = Weapon->GetAllSocketNames();
	SocketNames.RemoveAt(SocketNames.Num() - 1);

	for (int i = 0; i < SocketNames.Num(); ++i)
	{
		SocketPositions.Emplace(SocketNames[i], TArray<FVector>());
	}
}

// Called every frame
void ASwordPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsPause)
	{
		return;
	}

	SmoothLookat();
	CameraZoom();
	DebugRay();
	SprintRate(DeltaTime);
	Movement();
	SwordCollision();
}

// Called to bind functionality to input
void ASwordPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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

void ASwordPlayer::InitializeValue()
{
	MinWheel = -15.f;
	MaxWheel = 15.f;
	WheelSpeed = 3.f;
	NormalSpeed = 100.f;
	SprintMultiple = 3.f;
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
void ASwordPlayer::DebugRay()
{
	FVector Start = GetArrowComponent()->GetComponentLocation();
	FVector End = FVector(InputDirection.X, InputDirection.Y, 0.f);
	End = CameraLook->GetComponentQuat() * End;
	End.Z = 0.f;
	End.Normalize();

	DrawDebugLine(GetWorld(), Start, Start + (GetArrowComponent()->GetForwardVector() * 50.f),
		FColor::Red, false, -1.f, 1, 3.f);

	DrawDebugLine(GetWorld(), Start, Start + End * 50.f,
		FColor::Green, false, -1.f, 1, 3.f);

	DrawDebugLine(GetWorld(), Start, Start + CameraLook->GetForwardVector() * 50.f,
		FColor::Blue, false, -1.f, 1, 3.f);


	//UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), FVector::ZeroVector, FVector::ZeroVector,,);

}
void ASwordPlayer::SprintRate(float DeltaTime)
{
	if (IsSprint)
	{
		if (SprintPoint <= SprintRateMax)
		{
			SprintPoint += SprintMultiple * DeltaTime;
		}
	}
}
void ASwordPlayer::WeaponSetParents(FString SoketName)
{
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(*SoketName));
}
void ASwordPlayer::ResetVariables()
{
	UE_LOG(LogTemp, Log, TEXT("Reset"));

	InputDirection = FVector::ZeroVector;
	IsCollisionState = false;
	IsSaveAttack = false;
	CurrentAttackState = AttackButtonState::Default;
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

	FVector Start = GetArrowComponent()->GetComponentLocation();
	FVector End = FVector(InputDirection.X, InputDirection.Y, 0.f);
	End = CameraLook->GetComponentQuat() * End;
	End.Normalize();

	FVector vDir = (Start + End * 10.f) - GetArrowComponent()->GetComponentLocation();
	vDir.Normalize();

	float DeltaYaw = vDir.Rotation().Yaw - GetActorRotation().Yaw;
	DeltaYaw = UKismetMathLibrary::NormalizeAxis(DeltaYaw);
	
	DeltaYaw = GetActorRotation().Yaw + (0.05f * DeltaYaw);
	vDir = UKismetMathLibrary::GetForwardVector(FRotator(0, DeltaYaw, 0));

	AddMovementInput(vDir);
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed * ((1.f + SprintPoint));
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
void ASwordPlayer::OnLookatTime()
{
	PlayerAnimInst->RootMotionMode = ERootMotionMode::RootMotionFromMontagesOnly;
	IsInputLook = false;
	LookDir = FVector::ZeroVector;
	GetWorldTimerManager().ClearTimer(FLookatHandle);
}
FVector ASwordPlayer::LookDirection(FVector vDir)
{
	FVector Start = GetArrowComponent()->GetComponentLocation();
	FVector End = FVector(vDir.X, vDir.Y, 0.f);
	End = CameraLook->GetComponentQuat() * End;
	End.Normalize();

	FVector ConvertDir = (Start + End * 10.f) - GetArrowComponent()->GetComponentLocation();
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
}
void ASwordPlayer::InputRight(float Value)
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
	IsSprint = true;
}
void ASwordPlayer::UnSprint()
{
	IsSprint = false;
	SprintPoint = 0.f;
}
void ASwordPlayer::LeftAttack()
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
	if (!IsDraw ||
		PlayerAnimInst->GetIsDrawState() ||
		PlayerAnimInst->IsCurrentState(AnimState::Roll) ||
		PlayerAnimInst->IsCurrentState(AnimState::BackStep))
	{
		UE_LOG(LogTemp, Log, TEXT("aaaa"));
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
	if (PlayerAnimInst->IsCurrentState(AnimState::Attack) ||
		PlayerAnimInst->IsCurrentState(AnimState::Roll) ||
		PlayerAnimInst->IsCurrentState(AnimState::BackStep) ||
		(GetCharacterMovement()->GetMovementName() == "Flying"))
	{
		return;
	}

	ResetVariables();

	if (!IsDraw)
	{
		IsDraw = true;
		PlayerAnimInst->IsDrawState = true;
		PlayAnimMontage(DrawMontage, 1.f, TEXT("Draw"));
		GetCharacterMovement()->RotationRate = FRotator(0, 900.f, 0);
	}
	else
	{
		IsDraw = false;
		PlayerAnimInst->IsDrawState = true;
		PlayAnimMontage(DrawMontage, 1.f, TEXT("UnDraw"));
		GetCharacterMovement()->RotationRate = FRotator(0, 540.f, 0);
	}
}

void ASwordPlayer::Roll()
{
	if(!PlayerAnimInst->GetIsDrawState() &&
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
	if (!PlayerAnimInst->IsCurrentState(AnimState::Roll) &&
		!PlayerAnimInst->IsCurrentState(AnimState::BackStep) &&
		!(GetCharacterMovement()->GetMovementName() == "Flying"))
	{
		IsCollisionState = false;

		PlayerAnimInst->SetCurrentState(AnimState::Roll);
		StopAnimMontage(GetCurrentMontage());
		PlayAnimMontage(BackStepMontage, 1.f, "BackStep");
	}
}
void ASwordPlayer::SwordCollision()
{
	bool Result = false;
	if (IsCollisionState) //특정 애니메이션 프레임일때
	{
		TArray<AActor*> IgnoreObjects = {};
		TArray<FHitResult> OutHits = {};
		TMap<FName, FVector>::TRangedForIterator iter = SocketPosArray.begin();

		for (; iter != SocketPosArray.end(); ++iter)
		{
			FVector Start = (*iter).Value;
			FVector End = Weapon->GetSocketLocation((*iter).Key);
			Result = Collision->LineTraceObjects(Start, End, IgnoreObjects, OutHits);

			if (Result)
			{
				for (int i = 0; i < OutHits.Num(); ++i) // 충돌된 오브젝트를 한번씩만 받아온다
				{
					if (!HitObjects.Contains(OutHits[i].GetActor()))
					{
						HitObjects.Emplace(OutHits[i].GetActor());
						UGameplayStatics::ApplyPointDamage(OutHits[i].GetActor(),
							10.f, OutHits[i].ImpactPoint, OutHits[i], GetController(), this, nullptr);
					}
				}
			}
		}
	
	}
	else
	{
		HitObjects.Empty();
	}
	for (int i = 0; i < SocketNames.Num(); ++i) // 소켓 포지션을 갱신시킨다.
	{
		SocketPosArray.Emplace(SocketNames[i], Weapon->GetSocketLocation(SocketNames[i]));
	}
	

}
