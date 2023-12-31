// 版权在左，一切自由。


#include "AuraAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "AuraGameplayTags.h"

#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);
	auto AssetManager = CastChecked<UAuraAssetManager>(GEngine->AssetManager);
	return *AssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::Get().InitializeNativeGameplayTags();
	//初始化全局捕获表
	InitializeStaticTagsToCaptureDefsMap();
	// This is required to use Target Data!
	UAbilitySystemGlobals::Get().InitGlobalData();
}
