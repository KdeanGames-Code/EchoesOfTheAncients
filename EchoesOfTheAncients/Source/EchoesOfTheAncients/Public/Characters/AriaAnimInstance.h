// Copyright © 2025 Kdean Games. All Rights Reserved
// Defines the UAriaAnimInstance class, handling animation updates 
// for Aria in Echoes of the Ancients.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/CharacterTypes.h"
#include "AriaAnimInstance.generated.h"

// Forward declarations to minimize header dependencies
class AAriaCharacter;
class UCharacterMovementComponent;

/**
 * UAriaAnimInstance class for managing Aria’s animation state, 
   updating variables like ground speed and falling state.
 */
UCLASS()
class ECHOESOFTHEANCIENTS_API UAriaAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

    /// Initializes animation, sets up character and movement references.
    virtual void NativeInitializeAnimation() override;

    /// Updates animation properties every frame based on the character’s state.
    /// @param DeltaTime Time elapsed since the last frame, used for frame-rate independent updates.
    virtual void NativeUpdateAnimation(float DeltaTime) override;

    /// Reference to the owning Aria character.
    UPROPERTY(BlueprintReadOnly)
    AAriaCharacter* AriaCharacter;

    /// Reference to the character’s movement component.
    UPROPERTY(BlueprintReadOnly, Category = "Aria|Movement")
    UCharacterMovementComponent* AriaCharacterMovement;

    /// Speed in the XY plane for walking animations.
    UPROPERTY(BlueprintReadOnly, Category = "Aria|Movement", meta = (AllowPrivateAccess = "true", ToolTip = "Ground speed of Aria, calculated as the magnitude of her velocity in the XY plane (read-only, updated every frame)."))
    float WalkSpeed;

    /// Indicates if the character is falling (in air).
    UPROPERTY(BlueprintReadOnly, Category = "Aria|Movement", meta = (AllowPrivateAccess = "true", ToolTip = "Indicates whether Aria is currently falling (in the air), updated every frame (read-only)."))
    bool IsFalling;

    /// Aria’s current character state (e.g., unequipped or equipped with a weapon).
    UPROPERTY(BlueprintReadOnly, Category = "Aria|Movement|Character State", meta = (AllowPrivateAccess = "true", ToolTip = "Aria’s current character state (e.g., unequipped or equipped with a weapon), used to drive animation logic (read-only, updated every frame)."))
    ECharacterState CharacterState;
};
