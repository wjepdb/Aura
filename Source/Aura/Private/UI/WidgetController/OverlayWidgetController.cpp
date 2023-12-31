// 版权在左，一切自由。

#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	// Super::BoradcastInitialValues();
	const auto AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(AS->GetHealth());
	OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());

	OnManaChanged.Broadcast(AS->GetMana());
	OnMaxManaChanged.Broadcast(AS->GetMaxMana());
}

void UOverlayWidgetController::BindCallbackToDependencies()
{
	// Super::BindCallbackToDependencies();
	const auto AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	auto&& HealthDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute());
	HealthDelegate.AddLambda([this](const auto& Data)
	{
		OnHealthChanged.Broadcast(Data.NewValue);
	});

	auto&& MaxHealthDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute
		());
	MaxHealthDelegate.AddLambda([this](const auto& Data)
	{
		OnMaxHealthChanged.Broadcast(Data.NewValue);
	});

	auto&& ManaDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetManaAttribute());
	ManaDelegate.AddLambda([this](const auto& Data)
	{
		OnManaChanged.Broadcast(Data.NewValue);
	});

	auto&& MaxManaDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxManaAttribute());
	MaxManaDelegate.AddLambda([this](const auto& Data)
	{
		OnMaxManaChanged.Broadcast(Data.NewValue);
	});

	if (const auto AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		AuraASC->EffectAssetTags.AddLambda([this](const auto& Container)
		{
			for (const FGameplayTag& Tag : Container)
			{
				// 防止不正常的Tag
				const auto MessageTage = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (!Tag.MatchesTag(MessageTage))
				{
					continue;
				}
				const auto& Row = GetTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
				MessageWidgetRowDelegate.Broadcast(*Row);
			}
		});
	}
}
