// 版权在左，一切自由。


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
                                                                   bool bLogNotFound) const
{
	for (const auto& [InputAction, Tag] : AbilityInputActions)
	{
		if (Tag == InputTag && InputAction != nullptr)
		{
			return InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Not Found InputTag = %s"), *InputTag.ToString());
	}
	return nullptr;
}
