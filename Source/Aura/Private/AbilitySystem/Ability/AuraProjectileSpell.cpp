// 版权在左，一切自由。


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"

#include "Actor/AuraProjectile.h"

#include "Interaction/CombatInterface.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
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

	auto Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar || !GetAvatarActorFromActorInfo()->HasAuthority())
	{
		return;
	}


	// 只在服务端生成Actor

	// 需要在技能的发起者某个骨骼位置上生成火球
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Avatar);
	if (!CombatInterface)
	{
		return;
	}

	const auto SocketLocation = CombatInterface->GetCombatSocketLocation();
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	// TODO:要根据情况，不使用Picth = 0.f;
	//Rotation.Pitch = 0.f; // 如果清0，会导致目标高度不一致时，不能正确的发射火球

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	APawn* Pawn = Cast<APawn>(GetOwningActorFromActorInfo());
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass, SpawnTransform, Pawn, Pawn, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	if (Projectile)
	{
		// TODO: give the projectile a gameplay effect spec for causing damage.
		const auto SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Avatar);
		auto ContextHandle = SourceASC->MakeEffectContext();
		ContextHandle.AddSourceObject(this);
		const auto SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ContextHandle);

		const auto PlayerLevel = GetAbilityLevel();
		for (auto& [Tag,Value] : DamageTypes)
		{
			const auto ScaledDamage = Value.GetValueAtLevel(PlayerLevel);
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Tag, ScaledDamage);
		}

		Projectile->DamageEffectSpecHandle = SpecHandle;

		Projectile->FinishSpawning(SpawnTransform);
	}
	else
	{
		checkf(false, TEXT("找出错误原因"));
	}
}
