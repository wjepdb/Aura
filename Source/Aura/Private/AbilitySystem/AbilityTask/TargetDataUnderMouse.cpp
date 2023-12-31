// 版权在左，一切自由。


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

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
	// 我的练习代码
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

	// 练习代码（答案）
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();

	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		// TargetSetDelegate 使用 AbilityTargetDataSetDelegate 查找，一次性
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		auto&& TargetDataSetDelegate = AbilitySystemComponent->AbilityTargetDataSetDelegate(
			SpecHandle, ActivationPredictionKey);
		//绑定回调函数
		TargetDataSetDelegate.AddUObject(this, &ThisClass::OnTargetDataReplicatedCallback);

		// 检查TargetData先于Task到达，即在执行到此处时已经到达。
		// 顾名思义，如果此时TargetData已经到达，则会自动（重新）调用前面设置的委托（AbilityTargetDataSetDelegate）
		const bool bCalledDelegate = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(
			SpecHandle, ActivationPredictionKey);
		// 如果还没到达，则等待
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}


	// DrawDebugSphere(GetWorld(), CursorHit.Location, 8, 8, FColor::Blue, false, 5.f, 0, 2.f);
}

// 创建TargetData并发送至Server
void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPredictionWindow{AbilitySystemComponent.Get()};

	auto PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit;
	Data->HitResult = CursorHit;

	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
	                                                      GetActivationPredictionKey(),
	                                                      DataHandle,
	                                                      FGameplayTag{},
	                                                      AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
                                                           FGameplayTag ActivationTag)
{
	// 告诉ASC，该TargetData已经使用
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
