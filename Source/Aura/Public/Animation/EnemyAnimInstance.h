// 版权在左，一切自由。

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="运动")
	float GroundSpeed = 0.f;

private:
	TObjectPtr<class AAuraEnemy> AuraEnemy;
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;
};
