// 版权在左，一切自由。


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	// sure AttributeInfo is set.
	check(AttributeInfo);

	const auto AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	const auto AuraGameplayTag = FAuraGameplayTags::Get();
	auto Strength = AttributeInfo->FindAttributeInfoForTag(AuraGameplayTag.Attribute_Primary_Strength);
	Strength.AttributeValue = AS->GetStrength();
	AttributeInfoDelegate.Broadcast(Strength);
}

void UAttributeMenuWidgetController::BindCallbackToDependencies()
{
	// Super::BindCallbackToDependencies();
}
