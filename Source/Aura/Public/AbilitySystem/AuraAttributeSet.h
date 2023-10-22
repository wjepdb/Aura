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

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

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

protected:
	UFUNCTION()
	void OnRep_Health(FGameplayAttributeData const& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(FGameplayAttributeData const& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(FGameplayAttributeData const& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(FGameplayAttributeData const& OldMaxMana) const;
private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props);
};
