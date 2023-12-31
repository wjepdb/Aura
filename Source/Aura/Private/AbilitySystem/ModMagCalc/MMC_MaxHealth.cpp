// 版权在左，一切自由。

#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"

#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target.
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 为了捕获某个属性及得到该属性的值，必须创建 FAggregatorEvaluateParameters
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float VigorValue = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluateParameters, VigorValue);

	VigorValue = FMath::Max(VigorValue, 0.f);

	// max health 依赖vigor
	const auto CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	check(CombatInterface);
	const auto PlayerLevel = CombatInterface->GetPlayerLevel();

	const float MaxHealth = 80.f + 2.5 * VigorValue + PlayerLevel * 10.f;
	return MaxHealth;
}
