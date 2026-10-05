// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "CPP_Game2/Pawn/MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	MyCharacter = Cast<AMyCharacter>(TryGetPawnOwner());
	if (MyCharacter)
	{
		MyMovementComponent = MyCharacter->GetCharacterMovement();
	}
}
 
void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (MyCharacter)
	{
		PlayerGroundVelocity = UKismetMathLibrary::VSizeXY(MyMovementComponent->Velocity);
		IsFalling = MyMovementComponent->IsFalling();
		CharacterState = MyCharacter->GetCharacterState();
		ActionState = MyCharacter->GetActionState();
		DeathPose = MyCharacter->GetDeathPose();
	}
}
