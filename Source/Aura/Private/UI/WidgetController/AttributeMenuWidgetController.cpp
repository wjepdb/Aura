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

	for (const auto& [key,value] : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(key, value());
	}
}

void UAttributeMenuWidgetController::BindCallbackToDependencies()
{
	// Super::BindCallbackToDependencies();
	const auto AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	for (const auto& [key,value] : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(value()).AddLambda(
			[this,key,value](const auto& Data)
			{
				BroadcastAttributeInfo(key, value());
			});
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                            const FGameplayAttribute& Attribute) const
{
	auto Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag, true);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
