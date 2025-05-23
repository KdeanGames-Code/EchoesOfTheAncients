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
class USceneComponent;
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

    /// Equips the weapon by attaching it to the specified parent component and socket, setting owner and instigator.
    /// @param InParent The parent component to attach the weapon to (e.g., Aria’s skeletal mesh).
    /// @param InSocketName The name of the socket to attach the weapon to (e.g., "R_hand_weapon").
    /// @param NewOwner The actor that owns the weapon (e.g., AAriaCharacter).
    /// @param NewInstigator The pawn that instigates actions with the weapon (e.g., AAriaCharacter).
    void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

    /// Attaches the weapon’s ItemMesh to the specified parent component at the given socket.
    /// @param InParent The parent component to attach the weapon to (e.g., character’s skeletal mesh).
    /// @param InSocketName The name of the socket to attach the weapon to (e.g., "R_hand_weapon" or "SpineSocket")
    void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

    /// Array of actors to ignore during box traces to prevent multiple hits per swing.
    TArray<AActor*> IgnoreActors;

protected:

    /// Called when the game starts or when spawned, binds overlap events for the WeaponBox.
    virtual void BeginPlay() override;

    /// Overridden function called when the Sphere component begins overlapping with another actor.
    /// Calls the parent (AItem) version to maintain inherited behavior, extensible for weapon-specific actions.
    virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

    /// Overridden function called when the Sphere component stops overlapping with another actor.
    /// Calls the parent (AItem) version to maintain inherited behavior, extensible for weapon-specific actions.
    virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

    /// Callback for when the WeaponBox begins overlapping with another actor, performing a box trace to detect the impact point.
    UFUNCTION()
    void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    /// Blueprint-implementable event to create Field System fields at the specified location.
    UFUNCTION(BlueprintImplementableEvent)
    void CreateFields(const FVector& FieldLocation);

private:

    /// Sound to play when equipping the weapon, editable in Blueprints.
    UPROPERTY(EditAnywhere, Category = "Weapon Properties",
        meta = (ToolTip = "Sound to play when equipping the weapon, set in the default Blueprint (e.g., a chink sound for a sword)."))
    USoundBase* EquipSound;

    /// Box component for detecting overlaps during attacks, editable in Blueprints.
    UPROPERTY(VisibleAnywhere, Category = "Weapon Properties",
        meta = (ToolTip = "Box component for detecting overlaps during attacks, positioned around the weapon’s blade, editable in Blueprints."))
    UBoxComponent* WeaponBox;

    /// Scene component marking the start point of the box trace, positioned at the base of the blade, editable in Blueprints.
    UPROPERTY(VisibleAnywhere, Category = "Weapon Properties",
        meta = (ToolTip = "Scene component marking the start point of the box trace, positioned at the base of the blade. Adjust in Blueprints to align with the blade’s geometry."))
    USceneComponent* BoxTraceStart;

    /// Scene component marking the end point of the box trace, positioned at the tip of the blade, editable in Blueprints.
    UPROPERTY(VisibleAnywhere, Category = "Weapon Properties",
        meta = (ToolTip = "Scene component marking the end point of the box trace, positioned at the tip of the blade. Adjust in Blueprints to align with the blade’s geometry."))
    USceneComponent* BoxTraceEnd;

    /// Amount of damage the weapon deals per hit, editable in Blueprints.
    UPROPERTY(EditAnywhere, Category = "Weapon Properties")
    float Damage = 20.f;

public:
    /// Gets the WeaponBox component for collision detection.
    /// @return UBoxComponent* The WeaponBox component attached to this weapon.
    FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
};
