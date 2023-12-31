// 版权在左，一切自由。

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


USTRUCT(BlueprintType)
struct FEffectProperties
{
	GENERATED_BODY()

public:
	FEffectProperties() = default;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AController* SourceController = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AController* TargetController = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ACharacter* TargetCharacter = nullptr;

	FGameplayEffectContextHandle EffectContextHandle;
};

template <typename T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;


	//using FunctionPointer = FGameplayAttribute(*)();
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;


	/*
	 * Primary Attributes：不依赖其它属性
	 */
	// Strength
	UPROPERTY(ReplicatedUsing = OnRep_Strength, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength)

	// Intelligence：增加魔法伤害
	UPROPERTY(ReplicatedUsing = OnRep_Intelligence, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence)

	// Resilience 韧性：增加护甲及护甲穿透
	UPROPERTY(ReplicatedUsing = OnRep_Resilience, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience)

	// Vigor 活力：增加生命值及多数重要（vital）属性
	UPROPERTY(ReplicatedUsing = OnRep_Vigor, BlueprintReadOnly, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor)

	/*
	 * Secondary Attributes
	 */
	// Armor：依赖韧性，随着韧性增加而增加；可以减少伤害，并增加“block chance”
	UPROPERTY(ReplicatedUsing = OnRep_Armor, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor)

	// ArmorPenetration：护甲穿透，依赖韧性，增加忽略敌人护甲的“bolck chance”机率，增加“致命攻击”（cirtical hit chance)
	UPROPERTY(ReplicatedUsing = OnRep_ArmorPenetration, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration)

	// BlockChance：依赖Armo。格挡机率，如果成功，则减少一半伤害。
	UPROPERTY(ReplicatedUsing = OnRep_BlockChance, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance)

	// CriticalHitChance：注意护甲穿透会增加暴击。随着护甲穿透力的增加，暴击率也会增加。
	// 这将有机会使伤害加倍，并加上暴击加成。
	UPROPERTY(ReplicatedUsing = OnRep_CriticalHitChance, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance)

	// CriticalHitDamage
	UPROPERTY(ReplicatedUsing = OnRep_CriticalHitDamage, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage)

	// CriticalHitResistance
	UPROPERTY(ReplicatedUsing = OnRep_CriticalHitResistance, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance)

	// HealthRegeneration
	UPROPERTY(ReplicatedUsing = OnRep_HealthRegeneration, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration)

	// ManaRegeneration
	UPROPERTY(ReplicatedUsing = OnRep_ManaRegeneration, BlueprintReadOnly, Category = "Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration)

	/*
	 *	Resistance Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_FireResistance, Category="Resistance Attributes")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(ThisClass, FireResistance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArcaneResistance, Category="Resistance Attributes")
	FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS(ThisClass, ArcaneResistance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LightningResistance, Category="Resistance Attributes")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(ThisClass, LightningResistance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalResistance, Category="Resistance Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(ThisClass, PhysicalResistance)
	/*
	 * Vital Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category="Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(ThisClass, Health)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category="Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHealth)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category="Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(ThisClass, Mana)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category="Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(ThisClass, MaxMana)

	/*
	 * Meta Attributes
	 */
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(ThisClass, IncomingDamage)

protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const;
	UFUNCTION()
	void OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const;
	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const;
	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const;
private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props);
	void ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit);
};
