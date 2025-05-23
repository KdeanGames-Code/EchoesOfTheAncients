// Copyright © 2025 Kdean Games. All Rights Reserved
// Implements the UAriaAnimInstance class, 
// updating animation properties for Aria in Echoes of the Ancients.

#include "Characters/AriaAnimInstance.h"
#include "Characters/AriaCharacter.h"
#include "Characters/CharacterTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// --- Native Initialize Animation ---
void UAriaAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    AriaCharacter = Cast<AAriaCharacter>(TryGetPawnOwner());
    if (AriaCharacter)
    {
        AriaCharacterMovement = AriaCharacter->GetCharacterMovement();
    }
}

// --- Native Update Animation ---
void UAriaAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);
    
    if (AriaCharacterMovement)
    {
        WalkSpeed = UKismetMathLibrary::VSizeXY(AriaCharacterMovement->Velocity);
        IsFalling = AriaCharacterMovement->IsFalling();  // Update falling state
        CharacterState = AriaCharacter->GetCharacterState();
    }
}
