// Copyright © 2025 Kdean Games. All Rights Reserved
// Implements the AAEOA_Weapon class, providing overridden 
// overlap functions to call the parent AAEOA_Item behavior.

#include "Items/Weapons/AEOA_Weapon.h"
#include "Characters/AriaCharacter.h"

// --- Overlap Begin Callback ---
// Triggered when the Sphere component begins overlapping with another actor, setting the overlapping item on Aria.
void AAEOA_Weapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Call the parent class (AItem) version of OnSphereOverlap to 
    // maintain inherited overlap functionality.
    Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    
}

// --- Overlap End Callback ---
// Triggered when the Sphere component stops overlapping with another actor, clearing the overlapping item on Aria.
void AAEOA_Weapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // Call the parent class (AItem) version of OnSphereEndOverlap 
    // to maintain inherited overlap functionality.
    Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

// --- Equip ---
// Attaches the weapon’s ItemMesh to the specified parent component at the given socket.
void AAEOA_Weapon::Equip(USceneComponent* InParent, FName InSocketName)
{
    // Define attachment rules to snap the weapon to the target socket, including scale.
    FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
    // Attach the ItemMesh to the specified parent component (e.g., Aria’s skeletal mesh) at the given socket.
    ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}