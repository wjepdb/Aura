// 版权在左，一切自由。


#include "AbilitySystem/Ability/AuraProjectileSpell.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 不可以在本地（local）生成，只能在Server端生成。
	// 因此要生成的类，需要设置“可复制”，这样服务端就可以控制生成对象的移动及生命周期。
	// 因为是可复制的，客户端就可以看到生成对象的复制版本。
	
	/*
	if (!ActorInfo) return;

	auto OwnerActor = ActorInfo->OwnerActor;
	if (!OwnerActor.IsValid()) return;
	if (!OwnerActor->HasAuthority()) return;
	*/
}
