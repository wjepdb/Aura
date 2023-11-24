// 版权在左，一切自由。


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"

#include "GameFramework/Character.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility, FName(TEXT("获取目标数据任务")));
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	/** Called to trigger the actual task once the delegates have been set up
	 *	Note that the default implementation does nothing and you don't have to call it */
	// Super::Activate();

	// 目前任务：
	/**
	 *
	 * 获取鼠标位置
	 * 广播ValidData
	 * 绘制调试球
	 */
	// 练习代码
	/*
	const auto& AvatorCharacter = Cast<ACharacter>(GetAvatarActor());
	if (!AvatorCharacter || !AvatorCharacter->HasAuthority()) return;
	const auto& PC = Cast<APlayerController>(AvatorCharacter->GetController());
	if (!PC) return;

	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	if (!HitResult.bBlockingHit)return;

	ValidData.Broadcast(HitResult.ImpactPoint);

	DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 8, 8, FColor::Blue, false, 5.f, 0, 2.f);
	*/

	// 练习代码
}
