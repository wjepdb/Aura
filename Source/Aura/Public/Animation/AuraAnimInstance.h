// 版权在左，一切自由。

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AuraAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="运动")
	float Speed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="运动")
	bool bShouldMove = false;

private:
	TObjectPtr<class AAuraCharacter> AuraCharacter;
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;
};
