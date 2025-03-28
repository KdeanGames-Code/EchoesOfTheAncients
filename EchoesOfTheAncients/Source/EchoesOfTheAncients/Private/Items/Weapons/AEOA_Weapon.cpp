// Copyright © 2025 Kdean Games. All Rights Reserved
// Implements the AAEOA_Weapon class, providing functionality 
// for weapons in Echoes of the Ancients.
// This class handles weapon equipping, attachment to character sockets, 
// collision detection setup, and overlap event handling.

#include "Items/Weapons/AEOA_Weapon.h"
#include "Characters/AriaCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

// --- Constructor ---
// Initializes the weapon with a box component for collision detection.
AAEOA_Weapon::AAEOA_Weapon()
{
    WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
    WeaponBox->SetupAttachment(GetRootComponent());
}

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
    AttachMeshToSocket(InParent, InSocketName);
    // Play the equip sound at the weapon’s location, if set.
    if (EquipSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            EquipSound,
            GetActorLocation()
        );
    }
    // Disable the sphere’s collision to prevent re-overlapping after equipping.
    if (Sphere)
    {
        Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

// --- AttachMeshToSocket ---
// Attaches the weapon’s ItemMesh to the specified parent component at the given socket.
void AAEOA_Weapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
    // Define attachment rules to snap the weapon to the target socket, including scale.
    FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
    // Attach the ItemMesh to the specified parent component (e.g., Aria’s skeletal mesh) at the given socket.
    ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
    // Set the item state to equipped to stop hovering behavior.
    ItemState = EItemState::EIS_Equipped;
}