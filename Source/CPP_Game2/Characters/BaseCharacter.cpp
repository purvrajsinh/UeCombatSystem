// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "CPP_Game2/Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_Game2/CharacterTypes.h"
#include "CPP_Game2/Components/AttributeComponent.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage,const FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

void ABaseCharacter::StopMontage(UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(.25f, Montage);
	}
}


void ABaseCharacter::Attack()
{
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))
	{
		CombatTarget = nullptr;
	}
}


// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnable)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnable);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

FVector ABaseCharacter::GetTranslationWarpTarget()
{
	if (CombatTarget == nullptr) return FVector();
	
	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();
	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;
	
	return CombatTargetLocation + TargetToMe; 
	
}

FVector ABaseCharacter::GetRotationWarpTarget()
{
	if (CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	if (HitParticleSystem && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(), 
			HitParticleSystem,
			ImpactPoint);
	}
}

void ABaseCharacter::HandleDeath(FVector ImpactPoint)
{
	
	const FName Direction = GetHitDirection(ImpactPoint);
	if (Direction == FName("FromFront"))
	{
		DeathPose = EDeathPose::EDP_DeathFront;
	}
	else if (Direction == FName("FromLeft"))
	{
		DeathPose = EDeathPose::EDP_DeathLeft;
	}
	else if (Direction == FName("FromBack"))
	{
		DeathPose = EDeathPose::EDP_DeathBack;
	}
	else if (Direction == FName("FromRight"))
	{
		DeathPose = EDeathPose::EDP_DeathRight;
	}
	
	DisableCapsuleCollision();
	SetLifeSpan(DeathLifeSpan);
	SetWeaponCollisionEnable(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECR_Ignore);
}

FName ABaseCharacter::GetHitDirection(const FVector& ImpactPoint)
{
	const FVector ForwardVector = GetActorForwardVector();
	const FVector LowerImpactVector = FVector(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector HitVector = (LowerImpactVector - GetActorLocation()).GetSafeNormal();
	
	// Forward * Hit = |Forward||Hit| * cos(theta)
	// |Forward| and |Hit| = 1, So => Forward * Hit = cos(theta)
	const double CosTheta = FVector::DotProduct(ForwardVector, HitVector);
	double Theta = FMath::Acos(CosTheta);
	//Convert Theta from radian to degrees
	Theta = FMath::RadiansToDegrees(Theta);
	
	//If CrossProduct Points Down then theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(ForwardVector, HitVector);
	if (CrossProduct.Z < 0.f)
	{
		Theta *= -1.f;
	}
	
	if (Theta >=-45.f && Theta < 45.f)
	{
		return FName("FromFront"); // FromFront
		
	}else if (Theta >= -135.f && Theta < -45.f)
	{
		return FName("FromLeft"); // FROMLEFT
		
	}else if (Theta >= 135.f || Theta < -135.f)
	{
		return FName("FromBack");// FROMBACK
		
	}else if (Theta >= 45.f && Theta < 135.f)
	{
		return FName("FromRight");// FROMRIGHT
	}
	return FName("FromBack");
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	PlayMontageSection(HitReactMontage, GetHitDirection(ImpactPoint));
	
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName) const
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

// ATTACK
int32 ABaseCharacter::PlayAttackMontage()
{
   return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (IsAlive())
	{
		DirectionalHitReact(Hitter->GetActorLocation());
	}
	else
	{
		Die(Hitter->GetActorLocation());
	}
	SetWeaponCollisionEnable(ECollisionEnabled::NoCollision);
	SpawnHitParticles(ImpactPoint);
}

float ABaseCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	return DamageAmount;	
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 RandomSectionIndex = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[RandomSectionIndex]);
	return RandomSectionIndex;
}




bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::DisableCapsuleCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// DEATH
void ABaseCharacter::Die_Implementation(const FVector& ImpactPoint)
{
	PlayMontageSection(DeathMontage, GetHitDirection(ImpactPoint));
	Tags.Add(FName("Dead"));
}
