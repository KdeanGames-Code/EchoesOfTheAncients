// Copyright © 2025 Kdean Games. All Rights Reserved
// Defines the AAEOA_Weapon class, a child of AAEOA_Item, 
// providing functionality for weapons in Echoes of the Ancients.
// This class handles weapon equipping, attachment to character sockets, 
// and collision detection for attacks.

#pragma once

#include "CoreMinimal.h"
#include "Items/AEOA_Item.h"
#include "AEOA_Weapon.generated.h"

// Forward declarations to minimize header dependencies
class UBoxComponent;
class USoundBase;

/**
 * AAEOA_Weapon class representing a weapon item that can be equipped and used for combat.
 * Inherits from AAEOA_Item to leverage overlap detection and motion effects.
 */
UCLASS()
class ECHOESOFTHEANCIENTS_API AAEOA_Weapon : public AAEOA_Item
{
	GENERATED_BODY()
	
public:

    /// Constructor for AAEOA_Weapon, initializes weapon properties and components.
    AAEOA_Weapon();

    /// Attaches the weapon to the specified parent component at the given socket.
    /// @param InParent The parent component to attach to (e.g., Aria’s skeletal mesh).
    /// @param InSocketName The name of the socket to attach to (e.g., R_hand_weapon).
    void Equip(USceneComponent* InParent, FName InSocketName);

    /// Attaches the weapon’s ItemMesh to the specified parent component at the given socket.
    /// @param InParent The parent component to attach the weapon to (e.g., character’s skeletal mesh).
    /// @param InSocketName The name of the socket to attach the weapon to (e.g., "R_hand_weapon" or "SpineSocket")
    void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

protected:

    /// Overridden function called when the Sphere component begins overlapping with another actor.
    /// Calls the parent (AItem) version to maintain inherited behavior, extensible for weapon-specific actions.
    virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

    /// Overridden function called when the Sphere component stops overlapping with another actor.
    /// Calls the parent (AItem) version to maintain inherited behavior, extensible for weapon-specific actions.
    virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:

    /// Sound to play when equipping the weapon, editable in Blueprints.
    UPROPERTY(EditAnywhere, Category = "Weapon Properties",
        meta = (ToolTip = "Sound to play when equipping the weapon, set in the default Blueprint (e.g., a chink sound for a sword)."))
    USoundBase* EquipSound;

    /// Box component for detecting overlaps during attacks, editable in Blueprints.
    UPROPERTY(VisibleAnywhere, Category = "Weapon Properties",
        meta = (ToolTip = "Box component for detecting overlaps during attacks, positioned around the weapon’s blade, editable in Blueprints."))
    UBoxComponent* WeaponBox;
};
