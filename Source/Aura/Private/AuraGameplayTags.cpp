// 版权在左，一切自由。


#include "AuraGameplayTags.h"

#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	auto&& GameplayTagManager = UGameplayTagsManager::Get();
	GameplayTagManager.AddNativeGameplayTag(FName("Attributes.Secondary.Armor"),
	                                        FString("Reduces damage taken, improves Block Chance"));
}
