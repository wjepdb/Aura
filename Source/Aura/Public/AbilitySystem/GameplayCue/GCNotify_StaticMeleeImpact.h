// 版权在左，一切自由。

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GCNotify_StaticMeleeImpact.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UGCNotify_StaticMeleeImpact : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
};
