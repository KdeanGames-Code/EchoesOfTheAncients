// Copyright © 2025 Kdean Games. All Rights Reserved
// Defines the UAEOA_HealthBar class, a custom user widget 
// for managing the health bar UI in Echoes of the Ancients.
// Binds to the progress bar in the WB_HealthBar widget blueprint for dynamic updates.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AEOA_HealthBar.generated.h"

class UProgressBar;

/**
 * A custom user widget class for the health bar UI, 
 * used to display and update the health bar progress.
 */
UCLASS()
class ECHOESOFTHEANCIENTS_API UAEOA_HealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/// The progress bar widget representing the health bar, bound to the HealthBar variable in the WB_HealthBar blueprint.
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

};
