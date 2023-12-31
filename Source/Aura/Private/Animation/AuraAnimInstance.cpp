// 版权在左，一切自由。

#include "Animation/AuraAnimInstance.h"

#include "Character/AuraCharacter.h"

void UAuraAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	AuraCharacter = Cast<AAuraCharacter>(GetOwningActor());
	if (AuraCharacter)
	{
		MovementComponent = AuraCharacter->GetCharacterMovement();
	}
}

void UAuraAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!AuraCharacter)
	{
		AuraCharacter = Cast<AAuraCharacter>(GetOwningActor());
	}
	if (AuraCharacter)
	{
		MovementComponent = AuraCharacter->GetCharacterMovement();
	}
	if (!AuraCharacter || !MovementComponent)
	{
		return;
	}

	const auto Velocity = AuraCharacter->GetVelocity();
	Speed = Velocity.Size2D();

	bShouldMove = Speed > 3.f;
}
