// 版权在左，一切自由。


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
// widget只存在于local player

// GetOverlayWidgetController 和 GetAttributeMenuWidgetController 代码骨干相同，可以考虑合并代码。
UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (PC)
	{
		auto AuraHud = Cast<AAuraHUD>(PC->GetHUD());
		if (AuraHud)
		{
			const auto PS = PC->GetPlayerState<AAuraPlayerState>();
			const auto ASC = PS->GetAbilitySystemComponent();
			const auto AS = PS->GetAttributeSet();

			const FWidgetControllerParams Params{PC, PS, ASC, AS};

			return AuraHud->GetOverlayWidgetController(Params);
		}
	}

	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (PC)
	{
		auto AuraHud = Cast<AAuraHUD>(PC->GetHUD());
		if (AuraHud)
		{
			const auto PS = PC->GetPlayerState<AAuraPlayerState>();
			const auto ASC = PS->GetAbilitySystemComponent();
			const auto AS = PS->GetAttributeSet();

			const FWidgetControllerParams Params{PC, PS, ASC, AS};

			return AuraHud->GetAttributeMenuWidgetController(Params);
		}
	}

	return nullptr;
}
