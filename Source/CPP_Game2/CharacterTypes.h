#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_HitReacting UMETA(DisplayName = "HitReacting"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "EquippingWeapon"),
	EAS_Dead UMETA(DisplayName = "Dead")

};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDP_DeathFront UMETA(DisplayName = "DeathFront"),
	EDP_DeathLeft UMETA(DisplayName = "DeathLeft"),
	EDP_DeathBack UMETA(DisplayName = "DeathBack"),
	EDP_DeathRight UMETA(DisplayName = "DeathRight")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_NoState UMETA(DisplayName = "NoState"),
	EES_Dead UMETA(DisplayName = "Dead"),
		EES_Patroling UMETA(DisplayName = "Patroling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged")
	

	
};
