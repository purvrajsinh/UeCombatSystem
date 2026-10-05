// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CPP_Game2/CharacterTypes.h"
#include "MyAnimInstance.generated.h"

class AMyCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class CPP_GAME2_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	AMyCharacter* MyCharacter;
	
	UPROPERTY(BlueprintReadOnly, Category=Movement)
	UCharacterMovementComponent* MyMovementComponent;
	
	UPROPERTY(BlueprintReadOnly, Category=Movement)
	float PlayerGroundVelocity;
	
	UPROPERTY(BlueprintReadOnly, Category=Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category="Movement | Character State")
	ECharacterState CharacterState;
	
	UPROPERTY(BlueprintReadOnly)
	EActionState ActionState;
	
	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose;
};
