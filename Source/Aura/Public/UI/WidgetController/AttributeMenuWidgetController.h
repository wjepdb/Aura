// 版权在左，一切自由。

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UAttributeInfo;
/**
 * 有可能多个Widget绑定到同一控制器，因此要使用multi dynamic delegate
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, AuraAttributeInfo);

UCLASS(Blueprintable, BlueprintType)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbackToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS | Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
