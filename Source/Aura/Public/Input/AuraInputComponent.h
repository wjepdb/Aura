// 版权在左，一切自由。

#pragma once

#include "CoreMinimal.h"
#include "AuraInputConfig.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template <typename UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc,
	                        ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <typename UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object,
                                             PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc,
                                             HeldFuncType HeldFunc)
{
	check(InputConfig);
	for (const auto& [Action,Tag] : InputConfig->AbilityInputActions)
	{
		if (Action && Tag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action, ETriggerEvent::Started, Object, PressedFunc, Tag);
			}

			if (ReleasedFunc)
			{
				BindAction(Action, ETriggerEvent::Completed, Object, ReleasedFunc, Tag);
			}

			if (HeldFunc)
			{
				BindAction(Action, ETriggerEvent::Triggered, Object, HeldFunc, Tag);
			}
		}
	}
}
