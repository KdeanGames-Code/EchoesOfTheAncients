// Copyright © 2025 Kdean Games. All Rights Reserved
// Defines a custom animation notify state to play the 
// SFX_Woosh MetaSound with parameters during attack animations.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "MetasoundSource.h" // Include for UMetaSoundSource
#include "AnimNotifyState_PlayMetaSound.generated.h"

/**
 * 
 */
UCLASS()
class ECHOESOFTHEANCIENTS_API UAnimNotifyState_PlayMetaSound : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
    /// Called when the notify state begins, triggers the sound playback with parameters.
    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

    /// Called when the notify state ends, stops the sound playback.
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

    /// Name of the attack section (e.g., Attack1 or Attack2) to determine panning and pitch shift.
    UPROPERTY(EditAnywhere, Category = "Aria|SFX",
        meta = (ToolTip = "Name of the attack section (e.g., Attack1 or Attack2) to determine panning and pitch shift for the SFX_Woosh sound."))
    FName SectionName;

    /// Reference to the MetaSound asset to play.
    UPROPERTY(EditAnywhere, Category = "Aria|SFX",
        meta = (ToolTip = "Reference to the MetaSound asset to play during the animation (e.g., SFX_Woosh)."))
    class UMetaSoundSource* MetaSound;
};
