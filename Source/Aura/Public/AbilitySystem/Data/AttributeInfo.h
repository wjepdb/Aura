// 版权在左，一切自由。

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText{};

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};

/**
 * 以蓝图方式方便的设置资产信息
 */
UCLASS()
class AURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	FAuraAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraAttributeInfo> AttributeInformation;
};
