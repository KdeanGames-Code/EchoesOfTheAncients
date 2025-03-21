// Copyright © 2025 Kdean Games. All Rights Reserved
// Defines the AAEOA_Weapon class, a specialized child class of AItem for weapons in the game,
// inheriting overlap functionality and adding equipping and attack capabilities.

#pragma once

#include "CoreMinimal.h"
#include "Items/AEOA_Item.h"
#include "AEOA_Weapon.generated.h"

/**
 * AAEOA_Weapon class representing a weapon item that can be equipped and used for combat.
 * Inherits from AAEOA_Item to leverage overlap detection and motion effects.
 */
UCLASS()
class ECHOESOFTHEANCIENTS_API AAEOA_Weapon : public AAEOA_Item
{
	GENERATED_BODY()
	
public:
    /// Attaches the weapon to the specified parent component at the given socket.
    /// @param InParent The parent component to attach to (e.g., Aria’s skeletal mesh).
    /// @param InSocketName The name of the socket to attach to (e.g., R_hand_weapon).
    void Equip(USceneComponent* InParent, FName InSocketName);

protected:
    /// Overridden function called when the Sphere component begins overlapping with another actor.
    /// Calls the parent (AItem) version to maintain inherited behavior, extensible for weapon-specific actions.
    virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

    /// Overridden function called when the Sphere component stops overlapping with another actor.
    /// Calls the parent (AItem) version to maintain inherited behavior, extensible for weapon-specific actions.
    virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
