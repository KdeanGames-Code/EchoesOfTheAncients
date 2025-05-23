// Copyright © 2025 Kdean Games. All Rights Reserved
// Implements the UAEOA_HealthBarComponent class, 
// managing the health bar widget display in Echoes of the Ancients.

#include "UI/HUD/AEOA_HealthBarComponent.h"
#include "UI/HUD/AEOA_HealthBar.h"
#include "Components/ProgressBar.h"

/// Sets the health bar’s percentage to the specified value, updating the UI.
/// @param Percent The health percentage (0.0 to 1.0) to set on the progress bar.
void UAEOA_HealthBarComponent::SetHealthPercent(float Percent)
{
    if (HealthBarWidget == nullptr)
    {
        HealthBarWidget = Cast<UAEOA_HealthBar>(GetUserWidgetObject());
    }

    if (HealthBarWidget && HealthBarWidget->HealthBar)
    {
        HealthBarWidget->HealthBar->SetPercent(Percent);
    }
}