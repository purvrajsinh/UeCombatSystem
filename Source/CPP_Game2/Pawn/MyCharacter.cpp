// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CPP_Game2/Components/AttributeComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CPP_Game2/Item.h"
#include "CPP_Game2/Weapon.h"
#include "CPP_Game2/HUD/GameOverlay.h"
#include "CPP_Game2/HUD/GameHUD.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MySpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	MySpringArm->TargetArmLength = 300.0f;
	MySpringArm->SetupAttachment(RootComponent);

	MyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	MyCamera->SetupAttachment(MySpringArm);

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 950.f, 0.0f);
	
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void AMyCharacter::InitializeGameOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		AGameHUD* GameHUD = Cast<AGameHUD>(PlayerController->GetHUD());	
		if (GameHUD)
		{
			GameOverlay = GameHUD->GetGameOverlay();
			if (GameOverlay && Attributes)
			{
				GameOverlay->SetHealthBarPercentage(Attributes->GetHealthPercent()); 
			}
			
		}
	}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("EngeagableTarget"));
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	
	InitializeGameOverlay();
}

void AMyCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	if (Attributes && Attributes->GetHealthPercent() > .0f)
	{
	ActionState = EActionState::EAS_HitReacting;
	}
}
 

void AMyCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AMyCharacter::AttachSwordToBack()
{
	if (EquippedWeapon)
	{
	EquippedWeapon->AttachMeshToSoket(GetMesh(), FName("Soket_BackSwordHolder"));
	}
}

void AMyCharacter::DrawSword()
{
	if (EquippedWeapon)
	{
	EquippedWeapon->AttachMeshToSoket(GetMesh(), FName("Soket_Sword"));
	}
}

void AMyCharacter::Endequip()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void AMyCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	const FVector2d CurrentValue = Value.Get<FVector2D>();
	const FRotator ControllerRotation = GetControlRotation();
	const FRotator YawRotation = FRotator(0.f, ControllerRotation.Yaw, 0.f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);


	AddMovementInput(Direction, CurrentValue.X, false);
	AddMovementInput(RightDirection, CurrentValue.Y, false);
}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	const FVector2d CurrentValue = Value.Get<FVector2D>();
	AddControllerYawInput(CurrentValue.X);
	AddControllerPitchInput(-CurrentValue.Y);
}

void AMyCharacter::DisArm()
{
	PlayMontageSection(EquipUnEquipMontage, FName("UnEquip"));
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AMyCharacter::Arm()
{
	PlayMontageSection(EquipUnEquipMontage, FName("Equip"));
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AMyCharacter::EKeyPressed()
{
	
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		EquipWeapon(OverlappingWeapon);
	}else
	{
		if (CanDisArm())
		{
			DisArm();
			
		}else if (CanArm())
		{
			Arm();
		}
	}
}

void AMyCharacter::EquipWeapon(AWeapon* Weapon)
{
	Weapon->Equip(GetMesh(), FName("Soket_Sword"), this , this);
	CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

void AMyCharacter::SetHUDHealth()
{
	if (GameOverlay && Attributes)
	{
		GameOverlay->SetHealthBarPercentage(Attributes->GetHealthPercent());
	}
}

float AMyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                               class AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void AMyCharacter::Die_Implementation(const FVector& ImpactPoint)
{
	Super::Die_Implementation(ImpactPoint);
	ActionState = EActionState::EAS_Dead;
	HandleDeath(ImpactPoint);
}

void AMyCharacter::Attack()
{
	Super::Attack();
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

bool AMyCharacter::CanAttack()
{
	return (ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped);
}

bool AMyCharacter::CanDisArm()
{
	return (ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unequipped);
}

bool AMyCharacter::CanArm()
{
	return (ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unequipped && EquippedWeapon);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &AMyCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AMyCharacter::Attack);
	}
}
