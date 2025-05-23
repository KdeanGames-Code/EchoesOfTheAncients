// Copyright © 2025 Kdean Games. All Rights Reserved
// Implements the AAEOA_Weapon class, providing functionality 
// for weapons in Echoes of the Ancients.
// This class handles weapon equipping, attachment to character sockets, 
// collision detection setup, and overlap event handling.

#include "Items/Weapons/AEOA_Weapon.h"
#include "Characters/AriaCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"


// --- Constructor ---
// Initializes the weapon with a box component for collision detection.
AAEOA_Weapon::AAEOA_Weapon()
{
    WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
    WeaponBox->SetupAttachment(GetRootComponent());

    // Set collision presets to query only, overlap all channels except Pawn to avoid self-overlaps.
    WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

    BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
    BoxTraceStart->SetupAttachment(GetRootComponent());

    BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
    BoxTraceEnd->SetupAttachment(GetRootComponent());
}

// --- BeginPlay ---
// Called when the game starts or when spawned, binds overlap events for the WeaponBox.
void AAEOA_Weapon::BeginPlay()
{
    Super::BeginPlay();
    // Bind the OnBoxOverlap function to the WeaponBox’s overlap event to detect hits during attacks.
    WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AAEOA_Weapon::OnBoxOverlap);
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

/// Equips the weapon by attaching it to the specified parent component and socket, setting owner and instigator, plays an equip sound, and deactivates the SpectralxAI effect.
/// @param InParent The parent component to attach the weapon to (e.g., Aria’s skeletal mesh).
/// @param InSocketName The name of the socket to attach the weapon to (e.g., "R_hand_weapon").
/// @param NewOwner The actor that owns the weapon (e.g., AAriaCharacter).
/// @param NewInstigator The pawn that instigates actions with the weapon (e.g., AAriaCharacter).
void AAEOA_Weapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
    SetOwner(NewOwner);
    SetInstigator(NewInstigator);

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
    // Deactivate the SpectralxAI effect to stop the visual cue once the weapon is equipped.
    if (SpectralxAIEffect)
    {
        SpectralxAIEffect->Deactivate();
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

// --- Box Overlap Callback ---
// Triggered when the WeaponBox begins overlapping with another actor, performs a box trace to detect the impact point.
void AAEOA_Weapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Get the start and end positions for the box trace from the scene components.
    const FVector Start = BoxTraceStart->GetComponentLocation();
    const FVector End = BoxTraceEnd->GetComponentLocation();

    // Create an array of actors to ignore during the trace (ignore the weapon itself to prevent self-hits).
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);

    for (AActor* Actor : IgnoreActors)
    {
        ActorsToIgnore.AddUnique(Actor);
    }

    // Perform the box trace using the Visibility channel to detect the first blocking hit.
    FHitResult BoxHit;
    UKismetSystemLibrary::BoxTraceSingle(
        this,  // World context object (this weapon, an actor in the world).
        Start,  // Start position of the trace (base of the blade).
        End,  // End position of the trace (tip of the blade).
        FVector(5.f, 5.f, 5.f),  // Box half-size (5 units in each dimension for a small box).
        BoxTraceStart->GetComponentRotation(),  // Orientation of the box (matches the start component’s rotation).
        ETraceTypeQuery::TraceTypeQuery1,  // Visibility channel (TraceTypeQuery1 corresponds to Visibility).
        false,  // Do not trace complex geometry (faster, uses simple collision shapes).
        ActorsToIgnore,  // Actors to ignore during the trace.
        EDrawDebugTrace::None,  // Draw debug for 5 seconds (red until hit, green on hit).
        BoxHit,  // Hit result (output, populated with impact data).
        true  // Ignore self (redundant since we added 'this' to ActorsToIgnore, but required).
    );

    // If the trace hits an actor, check if it implements IHitInterface and call GetHit.
    if (BoxHit.GetActor())
    {
        IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
        if (HitInterface)
        {
            // Apply damage to the hit actor using the weapon’s damage value.
            UGameplayStatics::ApplyDamage(
                BoxHit.GetActor(),  // The actor to damage (e.g., an enemy).
                Damage,  // The amount of damage to apply.
                GetInstigator()->GetController(),  // The controller responsible for the damage (e.g., Aria’s controller).
                this,  // The damage causer (this weapon).
                UDamageType::StaticClass()  // The damage type class (default UDamageType).
            );

            HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);  // Execute GetHit on the hit actor, passing the impact point.
        
            IgnoreActors.AddUnique(BoxHit.GetActor()); // Add the hit actor to IgnoreActors to prevent multiple hits.

            CreateFields(BoxHit.ImpactPoint);  // Create Field System fields at the impact point to break destructible objects.
            
        }
    }
}