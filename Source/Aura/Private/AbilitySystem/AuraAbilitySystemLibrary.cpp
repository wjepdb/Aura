// 版权在左，一切自由。


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Game/AuraGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
// widget只存在于local player

// GetOverlayWidgetController 和 GetAttributeMenuWidgetController 代码骨干相同，可以考虑合并代码。
// 例如，创建静态函数：CreateWidgetControllerParameters(UObject* WorldContext) 函数即可。
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

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContext, ECharacterClass CharacterClass,
                                                            float Level, UAbilitySystemComponent* ASC)
{
	const auto AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
	if (!AuraGameModeBase) return;

	const auto AvatorActor = ASC->GetAvatarActor();

	const auto CharacterClassInfo = AuraGameModeBase->CharacterClassInfo;
	const auto ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	auto ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(AvatorActor);

	const auto PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level,
	                                                               ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data);

	const auto SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level,
	                                                                 ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data);

	const auto VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level,
	                                                             ContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data);
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContext, UAbilitySystemComponent* ASC)
{
	const auto AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
	if (!AuraGameModeBase) return;

	const auto AvatorActor = ASC->GetAvatarActor();
	auto ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(AvatorActor);

	const auto CharacterClassInfo = AuraGameModeBase->CharacterClassInfo;
	for (const auto AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec{AbilityClass, 1};
		ASC->GiveAbility(AbilitySpec);
	}
}
