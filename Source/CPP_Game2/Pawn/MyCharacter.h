// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_Game2/Characters/BaseCharacter.h"
#include "InputActionValue.h"
#include "CPP_Game2/CharacterTypes.h"
#include "MyCharacter.generated.h"

class UGameOverlay;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AItem;
class UAnimMontage;


UCLASS()
class CPP_GAME2_API AMyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	void EquipWeapon(AWeapon* Weapon);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Die_Implementation(const FVector& ImpactPoint) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	
	//Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Input")
	UInputAction* EquipAction;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Input")
	UInputAction* AttackAction;
	
	//Input Functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	void EKeyPressed();
	virtual void Attack() override;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipUnEquipMontage;
	
	UPROPERTY(BlueprintReadWrite)
	EActionState ActionState = EActionState::EAS_Unoccupied;
	//Play Montage Function
	virtual void AttackEnd() override;
	UFUNCTION(BlueprintCallable)
	void AttachSwordToBack();
	UFUNCTION(BlueprintCallable)
	void DrawSword();
	UFUNCTION(BlueprintCallable)
	void Endequip();
	UFUNCTION(BlueprintCallable)
	void HitReactEnd();	
	
	virtual bool CanAttack() override;
	bool CanDisArm();
	bool CanArm();
	void DisArm();
	void Arm();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
	
private:
	void InitializeGameOverlay();
	void SetHUDHealth();
	
	UPROPERTY()
	UGameOverlay* GameOverlay;
	//Character State
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	

	
	//Components
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* MySpringArm;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* MyCamera;
	
	UPROPERTY()
	AItem* OverlappingItem;
	
public:
	FORCEINLINE void SetOverLappingItem(AItem* Item){OverlappingItem = Item;}
	FORCEINLINE ECharacterState GetCharacterState() const {return CharacterState;} 
	FORCEINLINE EActionState GetActionState() const {return ActionState;}
};

