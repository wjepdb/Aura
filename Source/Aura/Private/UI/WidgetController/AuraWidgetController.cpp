// 版权在左，一切自由。

#include "UI/WidgetController/AuraWidgetController.h"

FWidgetControllerParams::FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ABS,
                                                 UAttributeSet* AS) : PlayerController(PC), PlayerState(PS),
                                                                      AbilitySystemComponent(ABS), AttributeSet(AS)
{
}

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}
