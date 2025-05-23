// Copyright © 2025 Kdean Games. All Rights Reserved
// Defines the AAEOA_Item class, a base class for items in 
// Echoes of the Ancients (AEOA) that can be interacted with,
// including overlap events and dynamic motion effects (bobbing and rotation).

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "AEOA_Item.generated.h"

// Forward declaration to avoid including the full header, improving compile-time efficiency.
class UNiagaraComponent;
class USphereComponent;

// Enum class defining possible states for an item, 
// used to manage its behavior (e.g., hovering or equipped).
enum class EItemState : uint8
{
    EIS_Hovering,
    EIS_Equipped
};

UCLASS()
class ECHOESOFTHEANCIENTS_API AAEOA_Item : public AActor
{
	GENERATED_BODY()
	
public:
    /// Constructor for AEOA_Item, initializes actor properties.
    AAEOA_Item();

    /// Called every frame, updates actor state.
    virtual void Tick(float DeltaTime) override;

protected:

    /// Called when the game starts or when spawned.
    virtual void BeginPlay() override;

    /// Returns the transformed sine value based on 
    /// RunningTime and TimeConstant for vertical bobbing motion.
    /// @return float The transformed sine value (Amplitude * Sin(RunningTime * TimeConstant)).
    UFUNCTION(BlueprintPure, meta = (ToolTip = "Returns transformed sine value: Amplitude * Sin(RunningTime * TimeConstant) for motion calculations"))
    float TransformedSin();

    /// Returns the transformed cosine value based on 
    /// RunningTime and TimeConstant for potential horizontal motion.
    /// @return float The transformed cosine value (Amplitude * Cos(RunningTime * TimeConstant)).
    UFUNCTION(BlueprintPure, meta = (ToolTip = "Returns transformed cosine value: Amplitude * Cos(RunningTime * TimeConstant) for motion calculations"))
    float TransformedCos();

    /// Template function to compute the average of two values of type T.
    /// @param First The first value to average.
    /// @param Second The second value to average.
    /// @return T The arithmetic mean of First and Second.
    template<typename T>
    T Avg(T First, T Second);

    // Callback function for the OnComponentBeginOverlap delegate on the Sphere component.
    // Triggered when another actor begins overlapping with the Sphere, 
    // printing the overlapping actor's name.
    // Parameters match the delegate signature defined in UPrimitiveComponent.
    UFUNCTION()
    virtual void OnSphereOverlap(
        UPrimitiveComponent* OverlappedComponent,   // Pointer to the Sphere component on this actor that was overlapped
        AActor* OtherActor,                         // Pointer to the actor that overlapped with this component (e.g., player)
        UPrimitiveComponent* OtherComp,             // Pointer to the component on the other actor (e.g., player's capsule)
        int32 OtherBodyIndex,                       // Index of the physics body on the other actor (if applicable)
        bool bFromSweep,                            // True if the overlap resulted from a sweep (movement check)
        const FHitResult& SweepResult               // Reference to hit result data if bFromSweep is true
    );

    // Callback function for the OnComponentEndOverlap delegate on the Sphere component.
    // Triggered when another actor stops overlapping with the Sphere, printing a message with the actor's name.
    // Parameters match the delegate signature defined in UPrimitiveComponent.
    UFUNCTION()
    virtual void OnSphereEndOverlap(
        UPrimitiveComponent* OverlappedComponent,   // Pointer to the Sphere component on this actor that stopped overlapping
        AActor* OtherActor,                         // Pointer to the actor that stopped overlapping (e.g., player)
        UPrimitiveComponent* OtherComp,             // Pointer to the component on the other actor (e.g., player's capsule)
        int32 OtherBodyIndex                        // Index of the physics body on the other actor (if applicable)
    );

    /// Pointer to the static mesh component representing the item’s visual representation.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* ItemMesh;  // Exposed for Blueprint visibility, mesh set in BP

    // Sphere component used for detecting overlaps with other actors (e.g., the player).
    UPROPERTY(VisibleAnywhere)
    USphereComponent* Sphere;

    /// Tracks the item’s current state (e.g., hovering or equipped), 
    /// accessible to derived classes.
    EItemState ItemState = EItemState::EIS_Hovering;

    /// Amplitude of item bobbing, adjustable at runtime (Unreal units: 1 uu = 1 cm).
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion|Bobbing",
        meta = (ToolTip = "Height of item bobbing in Unreal units (1 uu = 1 cm), adjustable at runtime"))
    float Amplitude = 0.25f;

    /// Time constant for bobbing speed, adjustable at runtime (period = 2π/TimeConstant seconds).
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion|Bobbing",
        meta = (ToolTip = "Speed factor for bobbing; period = 2π/TimeConstant seconds, adjustable at runtime"))
    float TimeConstant = 3.14159f;

    /// Rotation speed in degrees per second, adjustable at runtime.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion|Rotation",
        meta = (ToolTip = "Rotation speed in degrees per second around the Y-axis, adjustable at runtime"))
    float RotationRate = 45.0f;

    /// Niagara component for the SpectralxAI effect, displays glowing embers around the item to indicate it can be picked up.
    UPROPERTY(EditAnywhere)
    UNiagaraComponent* SpectralxAIEffect;



private:

    /// Elapsed time since spawn, managed by Tick for sine-based bobbing (read-only).
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion|Bobbing",
        meta = (AllowPrivateAccess = "true", ToolTip = "Elapsed time in seconds since spawn; increments by DeltaTime each frame to drive sine-based bobbing (read-only, managed by Tick for smooth motion)"))
    float RunningTime;

};

/// Inline definition of the Avg template function.
template<typename T>
inline T AAEOA_Item::Avg(T First, T Second)
{
    return (First + Second) / 2;  // Computes the average of two values
}