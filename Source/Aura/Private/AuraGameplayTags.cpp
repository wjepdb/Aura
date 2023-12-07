// 版权在左，一切自由。


#include "AuraGameplayTags.h"

#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	auto&& GameplayTagManager = UGameplayTagsManager::Get();

	// Primary
	GameplayTags.Attribute_Primary_Strength = GameplayTagManager.AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"), FString(TEXT("增加物理伤害")));
	GameplayTags.Attribute_Primary_Intelligence = GameplayTagManager.AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"), FString(TEXT("增加法术伤害")));
	GameplayTags.Attribute_Primary_Resilience = GameplayTagManager.AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"), FString(TEXT("增加护甲及韧性")));
	GameplayTags.Attribute_Primary_Vigor = GameplayTagManager.AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"), FString(TEXT("增加生命")));

	// Secondary
	GameplayTags.Attribute_Secondary_Armor = GameplayTagManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"), FString(TEXT("减少受到的伤害，增加格挡机率")));
	GameplayTags.Attribute_Secondary_ArmorPenetration = GameplayTagManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmoPenetration"), FString(TEXT("无视敌方护甲，增加致命一击的机率")));
	GameplayTags.Attribute_Secondary_BlockChance = GameplayTagManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"), FString(TEXT("伤害减半")));
	GameplayTags.Attribute_Secondary_CriticalHitChance = GameplayTagManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"), FString(TEXT("暴击的机率")));
	GameplayTags.Attribute_Secondary_CriticalHitDamage = GameplayTagManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"), FString(TEXT("暴击时伤害值")));
	GameplayTags.Attribute_Secondary_CriticalHitResistance = GameplayTagManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"), FString(TEXT("受到敌方攻击时，减少暴击的机率")));
	GameplayTags.Attribute_Secondary_HealthRegeneration = GameplayTagManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"), FString(TEXT("每秒增加的生命值")));
	GameplayTags.Attribute_Secondary_ManaRegeneration = GameplayTagManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"), FString(TEXT("每秒增加的魔法值")));
	GameplayTags.Attribute_Secondary_MaxHealth = GameplayTagManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"), FString(TEXT("最大生命值")));
	GameplayTags.Attribute_Secondary_MaxMana = GameplayTagManager.AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"), FString(TEXT("最大魔法值")));


	// 输入相关
	GameplayTags.InputTag_1 = GameplayTagManager.AddNativeGameplayTag(FName("InputTag.1"), FString(TEXT("按键1")));
	GameplayTags.InputTag_2 = GameplayTagManager.AddNativeGameplayTag(FName("InputTag.2"), FString(TEXT("按键2")));
	GameplayTags.InputTag_3 = GameplayTagManager.AddNativeGameplayTag(FName("InputTag.3"), FString(TEXT("按键3")));
	GameplayTags.InputTag_4 = GameplayTagManager.AddNativeGameplayTag(FName("InputTag.4"), FString(TEXT("按键4")));

	GameplayTags.InputTag_LMB = GameplayTagManager.AddNativeGameplayTag(FName("InputTag.LMB"), FString(TEXT("鼠标左键")));
	GameplayTags.InputTag_RMB = GameplayTagManager.AddNativeGameplayTag(FName("InputTag.RMB"), FString(TEXT("鼠标右键")));

	GameplayTags.InputTag_Shift = GameplayTagManager.AddNativeGameplayTag(
		FName("InputTag.Shift"), FString(TEXT("Shift")));

	GameplayTags.Damage = GameplayTagManager.AddNativeGameplayTag(FName("Damage"), FString(TEXT("Damage")));
	GameplayTags.Effects_HitReact = GameplayTagManager.AddNativeGameplayTag(FName("Effects.HitReact"), FString(TEXT("Tag granted when Hit Reacting")));
}
