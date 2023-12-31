// 版权在左，一切自由。

#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	check(OverlayWidgetControllerClass);

	if (!OverlayWidgetController)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		if (OverlayWidgetController)
		{
			OverlayWidgetController->SetWidgetControllerParams(WCParams);
			OverlayWidgetController->BindCallbackToDependencies();
		}
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	check(AttributeMenuWidgetControllerClass);

	if (!AttributeMenuWidgetController)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(
			this, AttributeMenuWidgetControllerClass);
		if (AttributeMenuWidgetController)
		{
			AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
			AttributeMenuWidgetController->BindCallbackToDependencies();
		}
	}
	return AttributeMenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ABS, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("必须初始化OverlayWidgetClass"));
	checkf(OverlayWidgetControllerClass, TEXT("必须初始化OverlayWidgetControllerClass"));

	OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);
	const auto WidgetController = GetOverlayWidgetController(FWidgetControllerParams{PC, PS, ABS, AS});
	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	OverlayWidget->AddToViewport();
}
