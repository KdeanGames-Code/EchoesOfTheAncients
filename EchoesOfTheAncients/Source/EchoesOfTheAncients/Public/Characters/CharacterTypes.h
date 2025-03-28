// Copyright � 2025 Kdean Games. All Rights Reserved
// Defines shared types for character-related functionality in 
// Echoes of the Ancients, such as enums for character states.

#pragma once

// Enum class defining possible states for Aria�s character, used to drive animation and gameplay logic.
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
    ECS_Unequipped UMETA(DisplayName = "Unequipped"),
    ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
    ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

// Enum class defining possible action states for Aria�s character, used to manage gameplay actions like attacking.
UENUM(BlueprintType)
enum class EActionState : uint8
{
    EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
    EAS_Attacking UMETA(DisplayName = "Attacking"),
    EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon")
};