// 版权在左，一切自由。

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

// Aura gmaeplay tags : singleton containing native gameplay tags.
struct FAuraGameplayTags
{
	static const FAuraGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

private:
	static FAuraGameplayTags GameplayTags;
};
