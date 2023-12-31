// 版权在左，一切自由。

#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"

#include "AbilitySystem/Ability/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	/**
	 *	OnGameplayEffectAppliedDelegateToSelf:
	 *
	 *	Called on server whenever a GE is applied to self. This includes instant and duration based GEs.
	 *
	 *	因此客户端不会触发相应（UI）事件
	 */
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::EffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const auto& AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec{AbilityClass, 1};
		if (const auto AuraAbilityInstanceCDO = CastChecked<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbilityInstanceCDO->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("初始技能需要从AuraGameplayAbility派生，并添加GameplayTag"));
		}
		// GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	// 我们不希望每帧都尝试激活GameplayAbility。Hold会逐帧调用。
	// 我们希望如果对应的技能没有激活，则尝试激活，否则什么也不做。
	if (!InputTag.IsValid())
	{
		return;
	}

	// 检查ASC所有激活的组件，是否与InputTag匹配


	// GetActivatableAbilities() 返回“可激活”的技能列表，可激活并不表示已经激活；
	const auto ActivatableAbilitySpecs = GetActivatableAbilities();
	for (auto AbilitySpec : ActivatableAbilitySpecs)
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) // 完全匹配
		{
			// 如果技能需要“Pressed”的概念，需要调用如下函数。
			// 我们此处也调用这个函数，因为只要保持Hold状态，
			//我们想让技能知道这个“输入”已经Pressed。
			AbilitySpecInputPressed(AbilitySpec);

			if (!AbilitySpec.IsActive())
			{
				// 可能有什么东西阻止我们激活技能，因此使用“Try...”激活技能
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())
	{
		return;
	}
	// 有些技能并不需要主动结束或取消，因此只要告诉技能按下已经“Released”即可。
	const auto ActivatableAbilitySpecs = GetActivatableAbilities();
	for (auto AbilitySpec : ActivatableAbilitySpecs)
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) // 完全匹配
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::EffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                               const FGameplayEffectSpec& EffectSpec,
                                                               FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}
