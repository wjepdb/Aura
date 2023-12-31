// 版权在左，一切自由。


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Interaction/CombatInterface.h"


/* 合并DECLARE_ATTRIBUTE_CAPTUREDEF(P)、DEFINE_ATTRIBUTE_CAPTUREDEF(S, P, T, B)两个宏 */
#define GEN_ATTRIBUTE_CAPTUREDEF(S, P, T, B)\
	FProperty* P##Property = FindFieldChecked<FProperty>(S::StaticClass(), GET_MEMBER_NAME_CHECKED(S, P)); \
	FGameplayEffectAttributeCaptureDefinition P##Def = FGameplayEffectAttributeCaptureDefinition(P##Property, EGameplayEffectAttributeCaptureSource::T, B);

// raw struc, not expose to bp
struct AuraDamageStatics
{
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false)
		// 注意，从source中捕获。因为护甲和BlockChance是从目标处获取（与目标相关），
		// 但护甲穿透是与技能的发起者相关
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false)

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false)

		// auto& GameplayTags = FAuraGameplayTags::Get();
		//
		// // 添加捕获的Tag。
		// // TODO:现在已经碰到全局初始化之间依赖性的关系。本类依赖FAuraGameplayTags初始化，
		// // 但FAuraGameplayTags是在模块AssetManager启动时才进行初始化的，因此存在致命Bug!!!
		// TagsToCaptureDefs.Add(GameplayTags.Attribute_Secondary_Armor, ArmorDef);
		// TagsToCaptureDefs.Add(GameplayTags.Attribute_Secondary_BlockChance, BlockChanceDef);
		// TagsToCaptureDefs.Add(GameplayTags.Attribute_Secondary_ArmorPenetration, ArmorPenetrationDef);
		// TagsToCaptureDefs.Add(GameplayTags.Attribute_Secondary_CriticalHitChance, CriticalHitChanceDef);
		// TagsToCaptureDefs.Add(GameplayTags.Attribute_Secondary_CriticalHitResistance, CriticalHitResistanceDef);
		// TagsToCaptureDefs.Add(GameplayTags.Attribute_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		//
		// TagsToCaptureDefs.Add(GameplayTags.Attribute_Resistance_Arcane, ArcaneResistanceDef);
		// TagsToCaptureDefs.Add(GameplayTags.Attribute_Resistance_Fire, FireResistanceDef);
		// TagsToCaptureDefs.Add(GameplayTags.Attribute_Resistance_Lightning, LightningResistanceDef);
		// TagsToCaptureDefs.Add(GameplayTags.Attribute_Resistance_Physical, PhysicalResistanceDef);
	}

	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance)
	/*	以下方式可行，为了与教程保持一致，故暂不使用
		GEN_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false)
		GEN_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false)
		// 注意，从source中捕获。因为护甲和BlockChance是从目标处获取（与目标相关），
		// 但护甲穿透是与技能的发起者相关
		GEN_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false)
		GEN_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false)
		GEN_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false)
		GEN_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false)
	
		GEN_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false)
		GEN_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false)
		GEN_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false)
		GEN_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false)
	*/
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	void InitializeTagsToCaptureDefsMap()
	{
		auto& GameplayTags = FAuraGameplayTags::Get();

		// 添加捕获的Tag。
		// TODO:现在已经碰到全局初始化之间依赖性的关系。本类依赖FAuraGameplayTags初始化，
		// 但FAuraGameplayTags是在模块AssetManager启动时才进行初始化的，因此存在致命Bug!!!
		TagsToCaptureDefs.Empty();
		TagsToCaptureDefs.Add(GameplayTags.Attribute_Secondary_Armor, ArmorDef);
		TagsToCaptureDefs.Add(GameplayTags.Attribute_Secondary_BlockChance, BlockChanceDef);
		TagsToCaptureDefs.Add(GameplayTags.Attribute_Secondary_ArmorPenetration, ArmorPenetrationDef);
		TagsToCaptureDefs.Add(GameplayTags.Attribute_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCaptureDefs.Add(GameplayTags.Attribute_Secondary_CriticalHitResistance, CriticalHitResistanceDef);
		TagsToCaptureDefs.Add(GameplayTags.Attribute_Secondary_CriticalHitDamage, CriticalHitDamageDef);

		TagsToCaptureDefs.Add(GameplayTags.Attribute_Resistance_Arcane, ArcaneResistanceDef);
		TagsToCaptureDefs.Add(GameplayTags.Attribute_Resistance_Fire, FireResistanceDef);
		TagsToCaptureDefs.Add(GameplayTags.Attribute_Resistance_Lightning, LightningResistanceDef);
		TagsToCaptureDefs.Add(GameplayTags.Attribute_Resistance_Physical, PhysicalResistanceDef);
	}
};

/*如果改成全局变量（去除AuraDamageStatics结构），则要考虑编译时和运行时问题*/
static AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
	//	return *g_AuraDamageStatics;
}


UExecCalc_Damage::UExecCalc_Damage()
{
	/*
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	*/
	const auto RelevantAttributes = {
		DamageStatics().ArmorDef,
		DamageStatics().ArmorPenetrationDef,
		DamageStatics().BlockChanceDef,
		DamageStatics().CriticalHitChanceDef,
		DamageStatics().CriticalHitResistanceDef,
		DamageStatics().CriticalHitDamageDef,
		DamageStatics().FireResistanceDef,
		DamageStatics().LightningResistanceDef,
		DamageStatics().ArcaneResistanceDef,
		DamageStatics().PhysicalResistanceDef
	};
	RelevantAttributesToCapture.Append(RelevantAttributes);
}

/**
 *	通过这个函数学习：如何通过该执行计算影响到任意别的属性。 
 */
void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const auto SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const auto TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	auto SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	auto TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	auto SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	auto TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();


	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// get damage set by caller magnitude
	float Damage = 0.f;
	for (const auto& [Key,Value] : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		checkf(DamageStatics().TagsToCaptureDefs.Contains(Value), TEXT("TagsToCaptureDefs没有包含标签：[%s]"),
		       *Value.ToString());
		//float DamageTypeValue = Spec.GetSetByCallerMagnitude(Key);
		// 禁用警告
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Key, false);
		
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = DamageStatics().TagsToCaptureDefs[Value];
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= (100.f - Resistance) / 100.f;
		Damage += DamageTypeValue;
	}
	// Capture BlockChance on Target, and determine if there was a successful block

	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters,
	                                                           TargetBlockChance);
	TargetBlockChance = FMath::Max(0.f, TargetBlockChance);
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;

	auto EffectContextHandle = Spec.GetContext();
	/*auto AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get());
	AuraEffectContext->SetIsBlockedHit(bBlocked);
	*/
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);

	// if Block, halve the damage.
	if (bBlocked)
	{
		Damage *= 0.5f;
	}

	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters,
	                                                           TargetArmor);
	TargetArmor = FMath::Max(0.f, TargetArmor);


	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef,
	                                                           EvaluationParameters,
	                                                           SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max(0.f, SourceArmorPenetration);

	const auto CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	if (!CharacterClassInfo)
	{
		checkf(false, TEXT("客户端理论上不会进行Exec Calc，检查代码的逻辑"));
		return;
	}
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName(TEXT("ArmorPenetration")), FString{});
	const auto ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());

	// ArmorPenetration ignores a percentage of the Target's Armor.
	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;

	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName(TEXT("EffectiveArmor")), FString{});
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());

	// Armor ignores a percentage of incoming damage
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;


	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef,
	                                                           EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max(0.f, SourceCriticalHitChance);


	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef,
	                                                           EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max(0.f, TargetCriticalHitResistance);


	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef,
	                                                           EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max(0.f, SourceCriticalHitDamage);

	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName(TEXT("CriticalHitResistance")), FString{});
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(
		TargetCombatInterface->GetPlayerLevel());

	// critical hit resistance reduces critical hit chance by a certain percentage
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance *
		CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;
	Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;

	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);

	const FGameplayModifierEvaluatedData EvaluatedData{
		UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage
	};
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

void InitializeStaticTagsToCaptureDefsMap()
{
	DamageStatics().InitializeTagsToCaptureDefsMap();
}
