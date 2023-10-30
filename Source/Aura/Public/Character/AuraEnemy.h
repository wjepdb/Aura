// 版权在左，一切自由。

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"

#include "Interaction/EnemyInterface.h"

#include "AuraEnemy.generated.h"

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase
                            , public IEnemyInterface
                            , public ICombatInterface {
	GENERATED_BODY()

public:
	AAuraEnemy();
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	// combat interface
	virtual int32 GetLevel() override;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults",
		meta=(AllowPrivateAccess = "true"))
	int32 Level = 1;
};
