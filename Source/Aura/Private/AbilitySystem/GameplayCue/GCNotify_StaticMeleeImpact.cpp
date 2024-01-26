// 版权在左，一切自由。


#include "AbilitySystem/GameplayCue/GCNotify_StaticMeleeImpact.h"

#include "NiagaraFunctionLibrary.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"


bool UGCNotify_StaticMeleeImpact::OnExecute_Implementation(AActor* MyTarget,
                                                           const FGameplayCueParameters& Parameters) const
{
	// 对AggregatedSourceTags枚举
	const FVector Location = Parameters.Location;
	const auto EffectCauser = Parameters.EffectCauser.Get();
	auto SourceObject = Parameters.SourceObject.Get();

	for (const auto& Tag : Parameters.AggregatedSourceTags)
	{
		const auto TaggedMontage = ICombatInterface::Execute_GetTaggedMontageByTag(EffectCauser, Tag);
		UGameplayStatics::PlaySoundAtLocation(EffectCauser, TaggedMontage.ImpactSound, Location);

		const auto BloodEffect = ICombatInterface::Execute_GetBloodEffect(EffectCauser);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(EffectCauser, BloodEffect, Location);
	}
	return Super::OnExecute_Implementation(MyTarget, Parameters);
}
