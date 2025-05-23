// Copyright © 2025 Kdean Games. All Rights Reserved


#include "SFX/AnimNotifyState_PlayMetaSound.h"
#include "Characters/AriaCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// --- Notify Begin ---
// Called when the notify state begins, plays the SFX_Woosh MetaSound with appropriate parameters.
void UAnimNotifyState_PlayMetaSound::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

    // Get the owning actor (Aria) and cast to AAriaCharacter.
    AAriaCharacter* AriaCharacter = Cast<AAriaCharacter>(MeshComp->GetOwner());
    if (AriaCharacter && MetaSound)
    {
        // Get the location of the skeletal mesh to play the sound at Aria's position.
        FVector Location = MeshComp->GetComponentLocation();

        // Spawn an Audio Component to play the MetaSound.
        UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAtLocation(
            AriaCharacter,
            MetaSound,
            Location,
            FRotator::ZeroRotator,
            1.0f, // Volume multiplier.
            1.0f, // Pitch multiplier (handled by SetFloatParameter).
            0.0f, // Start time.
            nullptr, // Attenuation settings (default).
            nullptr, // Concurrency settings (default).
            true // Auto-destroy after playback (one-shot sound).
        );

        if (AudioComponent)
        {
            // Set parameters for duration, panning direction, and pitch shift.
            if (SectionName == FName("Attack1"))
            {
                // Right-to-left swing: Pan from left to right, 2.4 seconds.
                AudioComponent->SetFloatParameter(FName("StartPan"), -1.0f); // Start on the left.
                AudioComponent->SetFloatParameter(FName("EndPan"), 1.0f);   // End on the right.
                AudioComponent->SetFloatParameter(FName("Duration"), 2.4f);
                AudioComponent->SetFloatParameter(FName("Pitch Shift"), 0.525824f); // 1.261979 / 2.4.
            }
            else if (SectionName == FName("Attack2"))
            {
                // Left-to-right swing: Pan from right to left, 3.166667 seconds.
                AudioComponent->SetFloatParameter(FName("StartPan"), 1.0f);  // Start on the right.
                AudioComponent->SetFloatParameter(FName("EndPan"), -1.0f);  // End on the left.
                AudioComponent->SetFloatParameter(FName("Duration"), 3.166667f);
                AudioComponent->SetFloatParameter(FName("Pitch Shift"), 0.398523f); // 1.261979 / 3.166667.
            }
        }
    }
}

// --- Notify End ---
// Called when the notify state ends, currently does nothing as the Audio Component auto-destroys.
void UAnimNotifyState_PlayMetaSound::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::NotifyEnd(MeshComp, Animation, EventReference);
    // Sound playback is handled by SpawnSoundAtLocation, which auto-destroys, so no cleanup needed.
}