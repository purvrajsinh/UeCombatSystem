// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/PagedArray.h"
#include "CPP_Game2/CharacterTypes.h"
#include "GameFramework/Character.h"
#include "CPP_Game2/Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"


class AWeapon;
class UAttributeComponent;
class UAnimMontage;


UCLASS()
class CPP_GAME2_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	FName GetHitDirection(const FVector& ImpactPoint);
	
	virtual void Attack();
	virtual void HandleDamage(float DamageAmount);
	virtual int32 PlayAttackMontage();
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual bool CanAttack();
	bool IsAlive();
	UFUNCTION(BlueprintNativeEvent)
	void Die(const FVector& ImpactPoint);
	void DisableCapsuleCollision();
	void DirectionalHitReact(const FVector& ImpactPoint);
	void PlayMontageSection(UAnimMontage* Montage,const FName SectionName);
	void StopMontage(UAnimMontage* Montage);
	void PlayHitReactMontage(const FName& SectionName) const;
	void SpawnHitParticles(const FVector& ImpactPoint);
	void HandleDeath(FVector ImpactPoint);
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnable);
	
	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();
	
	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();
	
	UPROPERTY(VisibleAnywhere, Category = Weapons)
	AWeapon* EquippedWeapon;
	
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;
	
	
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;
	
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	AActor* CombatTarget;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	double WarpTargetDistance = 25.f;
	
	UPROPERTY(EditAnywhere)
	float DeathLifeSpan = 6.f;
	
	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose;
	
private:
	int32 PlayRandomMontageSection(UAnimMontage* Montage,const TArray<FName>& SectionNames);
	//ANIMATION MONTAGES
	
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;
	
	
	UPROPERTY(EditAnywhere)
	UParticleSystem* HitParticleSystem;
	
	UPROPERTY(EditAnywhere);
	TArray<FName> AttackMontageSections;
	
public:
	FORCEINLINE EDeathPose GetDeathPose() const { return DeathPose; }
};
