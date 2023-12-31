// 版权在左，一切自由。

#include "Animation/EnemyAnimInstance.h"

#include "Character/AuraEnemy.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	AuraEnemy = Cast<AAuraEnemy>(GetOwningActor());
	if (AuraEnemy)
	{
		MovementComponent = AuraEnemy->GetCharacterMovement();
	}
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!AuraEnemy)
	{
		AuraEnemy = Cast<AAuraEnemy>(GetOwningActor());
	}
	if (AuraEnemy)
	{
		MovementComponent = AuraEnemy->GetCharacterMovement();
	}
	if (!AuraEnemy || !MovementComponent)
	{
		return;
	}

	const auto Velocity = AuraEnemy->GetVelocity();
	GroundSpeed = Velocity.Size2D();
}
