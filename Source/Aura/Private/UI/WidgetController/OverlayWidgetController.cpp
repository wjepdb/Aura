// 版权在左，一切自由。

#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BoradcastInitialValues() {
	// Super::BoradcastInitialValues();
	const auto AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(AS->GetHealth());
	OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());

	OnManaChanged.Broadcast(AS->GetMana());
	OnMaxManaChanged.Broadcast(AS->GetMaxMana());
}

void UOverlayWidgetController::BindCallbackToDependencies() {
	// Super::BindCallbackToDependencies();
	const auto AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	auto&& HealthDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute());
	HealthDelegate.AddUObject(this, &ThisClass::HealthChanged);

	auto&& MaxHealthDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute
		());
	MaxHealthDelegate.AddUObject(this, &ThisClass::MaxHealthChanged);

	auto&& ManaDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetManaAttribute());
	ManaDelegate.AddUObject(this, &ThisClass::ManaChanged);

	auto&& MaxManaDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxManaAttribute());
	MaxManaDelegate.AddUObject(this, &ThisClass::MaxManaChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const {
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const {
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) {
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) {
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
