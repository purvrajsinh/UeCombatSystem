// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CPP_Game2/HUD/HealthBarComponent.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "CPP_Game2/Weapon.h"
#include "CPP_Game2/Components/AttributeComponent.h"
#include "Animation/AnimMontage.h"



AEnemy::AEnemy()
{
	SetUpEnemy();
}

void AEnemy::SetUpEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->MaxWalkSpeed = 100.f;
	
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensingComponent->SightRadius = 400.f;
	PawnSensingComponent->SetPeripheralVisionAngle(45.f);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (PawnSensingComponent) PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::OnSeenPawn);
	InitializeEnemy();
	Tags.Add(FName("Enemy"));

}

void AEnemy::InitializeEnemy()
{
	EnemyAIController = Cast<AAIController>(GetController());
	HideHealthBar();
	MoveToTarget(PatrolTarget);
	SpawnDefaultWeapon();
}

void AEnemy::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDead()) return;
	if (EnemyState > EEnemyState::EES_Patroling)
	{
	CheckCombatTarget();
	}
	else
	{
	CheckPatrolTarget();
	}
}


void AEnemy::HideHealthBar()
{
	if (HealthBarWidget) HealthBarWidget->SetVisibility(false);
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patroling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(PatrolTarget);
}

bool AEnemy::IsOutSideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

void AEnemy::ClearTimer(FTimerHandle TimerToClear)
{
	GetWorldTimerManager().ClearTimer(TimerToClear);
}

bool AEnemy::IsInSideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

void AEnemy::OnSeenPawn(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		!IsDead() &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
		SeenPawn->ActorHasTag(FName("EngeagableTarget"));
	
	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearTimer(PatrolTimer);
		ChaseTarget();		
	}
}

void AEnemy::PatrolTimerFinished() const
{
		MoveToTarget(PatrolTarget);
}

void AEnemy::MoveToTarget(const AActor* TargetActor) const
{

	if (EnemyAIController != nullptr && TargetActor != nullptr)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(TargetActor);
		MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
		EnemyAIController->MoveTo(MoveRequest);
	}
}

AActor* AEnemy::ChoosPatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
	
	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}

void AEnemy::Attack()
{
	Super::Attack();
	if (CombatTarget == nullptr) return;
	
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();
}

bool AEnemy::CanAttack()
{
	bool bCanAttack = 
		IsInSideAttackRadius() && 
		EnemyState != EEnemyState::EES_Attacking && 
		EnemyState != EEnemyState::EES_Engaged &&
		!IsDead();
		
	return bCanAttack;
}

bool AEnemy::InTargetRange(const AActor* Target,const double Radius) const
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}



void AEnemy::AttackEnd()
{
	Super::AttackEnd();

	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

void AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}

void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}

void AEnemy::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

bool AEnemy::IsDead() const
{
	return EnemyState == EEnemyState::EES_Dead;
}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(MinAttackTime,MaxAttackTime);
	GetWorldTimerManager().SetTimer(AttackTimer,this, &AEnemy::Attack, AttackTime);
}

void AEnemy::CheckCombatTarget()
{
	if (IsOutSideCombatRadius())
	{
		ClearTimer(AttackTimer);
		LoseInterest();
		if (EnemyState != EEnemyState::EES_Engaged)
		{
			StartPatrolling();
		}
	}
	else if (!IsInSideAttackRadius() && EnemyState != EEnemyState::EES_Chasing) 
	{			
		ClearTimer(AttackTimer);
		if (EnemyState != EEnemyState::EES_Engaged)
		{
			ChaseTarget();
		}
	}
	else if (CanAttack())
	{
		StartAttackTimer();
	}
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, CombatRadius))
	{
		PatrolTarget = ChoosPatrolTarget();
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, FMath::RandRange(PatrolWaitMin, PatrolWaitMax));
	}
}

void AEnemy::HandleDamage(float DamageAmount)
{
	Super::HandleDamage(DamageAmount);
	if (Attributes && HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}

void AEnemy::ShowHealthBar()
{
	if (HealthBarWidget) HealthBarWidget->SetVisibility(true);
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	if (!IsDead()) ShowHealthBar();
	ClearTimer(PatrolTimer);
	ClearTimer(AttackTimer);
	StopMontage(AttackMontage);
	
	if (IsInSideAttackRadius())
	{
		if (!IsDead())StartAttackTimer();
	}
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	if (IsInSideAttackRadius())
	{
		EnemyState = EEnemyState::EES_Attacking;
	}else if (IsOutSideCombatRadius())
	{
		ChaseTarget();
	}
	return DamageAmount;
}

void AEnemy::Die_Implementation(const FVector& ImpactPoint)
{
	Super::Die_Implementation(ImpactPoint);
	EnemyState = EEnemyState::EES_Dead;
	ClearTimer(AttackTimer);
	HandleDeath(ImpactPoint);
	HideHealthBar();
}

void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}