// Copyright © 2025 Kdean Games. All Rights Reserved
// Defines the AAriaCharacter class, representing Aria, 
// the protagonist of Echoes of the Ancients.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterTypes.h"
#include "GameFramework/Character.h"
#include "AriaCharacter.generated.h"

// Forward declarations to minimize header dependencies
class AAEOA_Item;
class UAnimHontage;
class UCameraComponent;
class UEnhancedInputComponent;
class UGroomComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class AAEOA_Weapon;

/// AAriaCharacter class definition for Aria, the protagonist of Echoes of the Ancients.
UCLASS()
class ECHOESOFTHEANCIENTS_API AAriaCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	/// Constructor for AAriaCharacter, initializes character properties.
	AAriaCharacter();

	/// Called every frame, updates character state.
	virtual void Tick(float DeltaTime) override;

	/// Called to bind functionality to input.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// Sets the collision enabled state for the equipped weapon’s WeaponBox.
	/// @param CollisionEnabled The collision state to set (e.g., NoCollision, QueryOnly).
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	/// Increments the Dawnspire count when a Dawnspire coin is collected.
	void IncrementDawnspireCount();

protected:
	
	virtual void BeginPlay() override;

	/// Moves the character based on input action value.
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	/// Moves the character left/right based on input action value.
	UFUNCTION()
	void MoveRight(const FInputActionValue& Value);

	/// Rotates the character left/right based on mouse X input.
	UFUNCTION()
	void Turn(const FInputActionValue& Value);

	/// Adjusts the character’s pitch up/down based on mouse Y input.
	UFUNCTION()
	void LookUp(const FInputActionValue& Value);

	/// Callback for the 'E' key press to equip a item.
	void PickupItem();

	/// Callback for the left mouse button press to play an attack montage.
	void Attack_OneHanded();

	/// Resets the action state to unoccupied after an attack montage finishes.
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	/// Checks if Aria can perform a one-handed attack based on her current state.
	bool CanAttack_OneHandedWeapon();

	/// Checks if Aria can arm (equip) her weapon based on her current state.
	bool CanArm();

	/// Checks if Aria can disarm (unequip) her weapon based on her current state.
	bool CanDisarm();

	/// Attaches the equipped weapon to the spine socket (unequip).
	UFUNCTION(BlueprintCallable)
	void Disarm();

	/// Attaches the equipped weapon to the right hand socket (equip).
	UFUNCTION(BlueprintCallable)
	void Arm();

	/// Resets the action state to unoccupied after an equip/unequip montage finishes.
	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	/**
	 * Play montage functions
	 */
	 /// Plays the one-handed attack montage with a random section.
	void PlayAttackMontage_OneHandedWeapon();

	/// Plays the equip/unequip montage with the specified section.
	/// @param SectionName The name of the section to play (e.g., "Equip" or "Unequip").
	void PlayEquipMontage(const FName& SectionName);

	/**
	 * Input Mapping
	 */
	/// Input mapping context for character movement, editable in Blueprints.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aria|Input",
		meta = (ToolTip = "Input mapping context defining key bindings for character movement (e.g., W/S keys)."))
	UInputMappingContext* IMC_AriaMovement;

	/// Input action for character movement, editable in Blueprints.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aria|Input",
		meta = (ToolTip = "Input action for moving the character, bound to W (forward) and S (backward)."))
	UInputAction* IA_MoveAction;

	/// Input action for character left/right movement, editable in Blueprints.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aria|Input",
		meta = (ToolTip = "Input action for moving the character left/right, bound to A/D."))
	UInputAction* IA_MoveRightAction;

	/// Input action for turning the character, editable in Blueprints.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aria|Input",
		meta = (ToolTip = "Input action for turning the character left or right, bound to MouseX."))
	UInputAction* IA_TurnAction;

	/// Input action for looking up/down, editable in Blueprints.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aria|Input",
		meta = (ToolTip = "Input action for adjusting the character’s pitch, bound to MouseY with inverted scale and swizzle modifier."))
	UInputAction* IA_LookUpAction;

	/// Input action for jumping, editable in Blueprints.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aria|Input",
		meta = (ToolTip = "Input action for making the character jump, bound to SpaceBar."))
	UInputAction* IA_JumpAction;

	/// Input action for picking up items, editable in Blueprints.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aria|Input",
		meta = (ToolTip = "Input action for picking up items, bound to the E key."))
	UInputAction* IA_PickupItem;

	/// Input action for attacking with a one-handed weapon, editable in Blueprints.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aria|Input",
		meta = (ToolTip = "Input action for attacking with a one-handed weapon, bound to the Left Mouse Button."))
	UInputAction* IA_Attack_OneHanded;

private:

	/// Spring arm component to offset and manage the camera, visible in Blueprints.
	UPROPERTY(VisibleAnywhere, Category = "Aria|Camera",
		meta = (ToolTip = "Spring arm component to offset the camera, enabling dynamic distance adjustment."))
	USpringArmComponent* CameraBoom;

	/// Camera component for the player’s viewpoint, visible in Blueprints.
	UPROPERTY(VisibleAnywhere, Category = "Aria|Camera",
		meta = (ToolTip = "Camera component providing the player’s viewpoint, attached to the spring arm."))
	UCameraComponent* ViewCamera;

	/// Groom component for Aria’s hair, visible in Blueprints.
	UPROPERTY(VisibleAnywhere, Category = "Aria|Hair",
		meta = (ToolTip = "Groom component for Aria’s hair, attached to the head socket of the Echo mesh."))
	UGroomComponent* Hair;

	/// Groom component for Aria’s eyebrows, visible in Blueprints.
	UPROPERTY(VisibleAnywhere, Category = "Aria|Hair",
		meta = (ToolTip = "Groom component for Aria’s eyebrows, attached to the head socket of the Echo mesh."))
	UGroomComponent* Eyebrows;

	/// Pointer to the item Aria is currently overlapping with, if any.
	UPROPERTY(VisibleInstanceOnly)
	AAEOA_Item* OverlappingItem;

	/// Tracks Aria’s current state (e.g., unequipped or equipped with a weapon).
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	/// Tracks Aria’s current action state (e.g., unoccupied or attacking).
	EActionState ActionState = EActionState::EAS_Unoccupied;

	/// Pointer to the currently equipped weapon, if any.
	UPROPERTY(VisibleAnywhere, Category = "Aria|Weapon",
		meta = (ToolTip = "Pointer to the currently equipped weapon, set during gameplay when equipping a weapon."))
	AAEOA_Weapon* EquippedWeapon;

	/**
	 * Animation montages
	 */
	 /// Animation montage for one-handed attacks, editable in the default Blueprint.
	UPROPERTY(EditDefaultsOnly, Category = "Aria|Montages",
		meta = (ToolTip = "Animation montage containing one-handed attack animations, set in the default Blueprint."))
	UAnimMontage* Attack_OneHandedWeapon;

	/// Animation montage for equipping and unequipping the weapon, editable in the default Blueprint.
	UPROPERTY(EditDefaultsOnly, Category = "Aria|Montages",
		meta = (ToolTip = "Animation montage containing equip and unequip animations for the weapon, set in the default Blueprint."))
	UAnimMontage* EquipMontage;

	/// Total number of Dawnspires collected by Aria.
	UPROPERTY(VisibleAnywhere, Category = "Aria|Treasure")
	int32 TotalDawnspiresCollected;

public:

	/// Sets the overlapping item when Aria overlaps with an item.
	/// @param Item The item to set as overlapping.
	FORCEINLINE void SetOverlappingItem(AAEOA_Item* Item) { OverlappingItem = Item; }
	/// Gets the current character state (e.g., unequipped or equipped).
	/// @return ECharacterState The current state of Aria.
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	/// Gets the total number of Dawnspires collected.
	/// @return The total number of Dawnspires collected by Aria.
	FORCEINLINE int32 GetTotalDawnspiresCollected() const { return TotalDawnspiresCollected; }
};
