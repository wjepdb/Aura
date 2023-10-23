// 版权在左，一切自由。

#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet() {
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec,
                                                FActiveGameplayEffectHandle ActiveEffectHandle) {
	UE_LOG(LogTemp, Warning, TEXT("GE：[%s] <-> %s"), *EffectSpec.Def.GetName(), *ActiveEffectHandle.ToString());
}
