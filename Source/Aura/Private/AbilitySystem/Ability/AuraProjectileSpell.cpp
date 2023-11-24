// 版权在左，一切自由。


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"

#include "Interaction/CombatInterface.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnProjectile()
{
	// !!!不可以!!!在本地（local）生成，只能在Server端生成。
	// 因此要生成的类，需要设置“可复制”，这样服务端就可以控制生成对象的移动及生命周期。
	// 因为是可复制的，客户端就可以看到生成对象的复制版本。

	/*
	if (!ActorInfo) return;

	auto OwnerActor = ActorInfo->OwnerActor;
	if (!OwnerActor.IsValid()) return;
	if (!OwnerActor->HasAuthority()) return;
	*/
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;


	// 只在服务端生成Actor

	// 需要在技能的发起者某个骨骼位置上生成火球
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (!CombatInterface) return;

	const auto SocketLocation = CombatInterface->GetCombatSocketLocation();
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	// TODO: 想找到方向（鼠标点击的物体）并不容易，因为客户端也可以触发技能并远程调用在服务端执行
	//		 需要一种特别的方法，稍后实现。
	APawn* Pawn = Cast<APawn>(GetOwningActorFromActorInfo());
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass, SpawnTransform, Pawn, Pawn, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (Projectile)
	{
		// TODO: give the projectile a gameplay effect spec for causing damage.
		Projectile->FinishSpawning(SpawnTransform);
	}
}
