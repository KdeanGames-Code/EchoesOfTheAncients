// Copyright © 2025 Kdean Games. All Rights Reserved
// Defines the UAEOA_HealthBarComponent class, a custom widget component 
// for displaying a health bar above actors in Echoes of the Ancients.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "AEOA_HealthBarComponent.generated.h"

class UAEOA_HealthBar;

/**
 * A custom widget component for displaying a health bar above actors, 
   such as enemies, in screen space.
 */
UCLASS()
class ECHOESOFTHEANCIENTS_API UAEOA_HealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:

	/// Sets the health bar’s percentage to the specified value, updating the UI.
	/// @param Percent The health percentage (0.0 to 1.0) to set on the progress bar.
	void SetHealthPercent(float Percent);

private:
	/// Cached reference to the UAEOA_HealthBar widget object displayed by this component.
	UPROPERTY()
	UAEOA_HealthBar* HealthBarWidget;
	
};
