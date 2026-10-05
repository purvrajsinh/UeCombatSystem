// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_Game2/CharacterTypes.h"
#include "CPP_Game2/Characters/BaseCharacter.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UPawnSensingComponent;
UCLASS()
class CPP_GAME2_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	
	/** <AActor> */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;
	/** </AActor> */
	
	/** <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& Impactpoint, AActor* Hitter) override;
	/** </IHitInterface> */
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	
protected:
	/** <ABaseCharacter> */
	virtual void HandleDamage(float DamageAmount) override;
	virtual void Die_Implementation(const FVector& ImpactPoint) override;
	virtual void AttackEnd() override;
	/** </ABaseCharacter> */
	
	virtual void Attack() override;
	virtual bool CanAttack() override;
	
	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patroling;
	 
private:
	void SetUpEnemy();
	void InitializeEnemy();
	void ShowHealthBar();
	void HideHealthBar();
	bool IsDead() const;
	void SpawnDefaultWeapon();
/** AI Behaviour Functions */
	void PatrolTimerFinished() const;
	void LoseInterest();
	void ChaseTarget();
	void StartPatrolling();
	bool IsOutSideCombatRadius();
	void ClearTimer(FTimerHandle TimerToClear);
	bool IsInSideAttackRadius();
	bool InTargetRange(const AActor* Target, double Radius) const;
	void CheckCombatTarget();
	void CheckPatrolTarget();
	void MoveToTarget(const AActor* TargetActor) const;
	AActor* ChoosPatrolTarget();
	void StartAttackTimer();
	
/** Vairables */
	UPROPERTY()
	class AAIController* EnemyAIController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;
	
	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;
	
	/** Patrol */
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensingComponent;
	
	UFUNCTION()
	void OnSeenPawn(APawn* SeenPawn);
	
	UPROPERTY(EditInstanceOnly, Category = AINavigation)
	AActor* PatrolTarget;   // Current Patrol Target
	
	UPROPERTY(EditInstanceOnly, Category = AINavigation)
	TArray<AActor*> PatrolTargets;
	
	FTimerHandle PatrolTimer;
	
	UPROPERTY(EditAnywhere, Category = AINavigation)
	float PatrolWaitMin = 2.f;
	
	UPROPERTY(EditAnywhere, Category = AINavigation)
	float PatrolWaitMax = 4.f;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrollingSpeed = 100.f;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 425.f;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float AcceptanceRadius = 50.f;
	/** Combat */

	
	UPROPERTY(EditAnywhere)
	double CombatRadius = 1000.f;
	
	
	
	/** Attack */
	FTimerHandle AttackTimer;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float MinAttackTime = .5f;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float MaxAttackTime = 1.f;
	
	UPROPERTY(EditAnywhere, Category = AINavigation)
	float AttackRadius = 180.f;
	
	
};


