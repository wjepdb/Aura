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

	/*
	 * 不太好的做法：逐个列出所有要播报的属性值
	 * 以下采用for_each做法
	
	auto Info = AttributeInfo->FindAttributeInfoForTag(AuraGameplayTag.Attribute_Primary_Strength);
	Info.AttributeValue = AS->GetStrength();
	AttributeInfoDelegate.Broadcast(Info);
	*/
	for (const auto& [key,value] : AS->TagsToAttributes)
	{
		auto Info = AttributeInfo->FindAttributeInfoForTag(key, true);
		auto Attr = value.Execute();
		Info.AttributeValue = Attr.GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
	}
}

void UAttributeMenuWidgetController::BindCallbackToDependencies()
{
	// Super::BindCallbackToDependencies();
}
