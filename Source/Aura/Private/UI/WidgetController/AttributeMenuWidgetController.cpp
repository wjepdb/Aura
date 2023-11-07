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
	
	auto Info = AttributeInfo->FindAttributeInfoForTag(AuraGameplayTag.Attribute_Primary_Strength);
	Info.AttributeValue = AS->GetStrength();
	AttributeInfoDelegate.Broadcast(Info);

	Info = AttributeInfo->FindAttributeInfoForTag(AuraGameplayTag.Attribute_Primary_Intelligence);
	Info.AttributeValue = AS->GetIntelligence();
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BindCallbackToDependencies()
{
	// Super::BindCallbackToDependencies();
}
