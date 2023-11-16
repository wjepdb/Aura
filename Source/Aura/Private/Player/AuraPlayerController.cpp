// 版权在左，一切自由。

#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "Components/SplineComponent.h"

#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include <NavigationSystem.h>

#include "NavigationPath.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("样条线组件"));
	// 不需要挂接到根组件
	// Spline->SetupAttachment(GetRootComponent());
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 没有输入装置，设定为严重错误，停机。
	check(AuraContext);

	const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const auto AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(IA_MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
	                                       &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		/*
		 * 点击左键时，要区分是要释放技能，还是单纯的想移动（click to move）？
		 * 现在做法就是如果点击在“Enemy”身上，则释放技能，否则就移动。
		 *
		 * 问题：如何判断是否“enemy”？
		 *
		 * 答案：可以参考CursorTrace()，其实现了如果光标所在位置下面的Actor实现了IEnemyInterface接口，就高亮，可以利用这一点。
		 */


		// 解法：添加变量bTargeting，追踪是否鼠标下面有敌人
		bTargeting = ThisActor != nullptr;
		bAutoRunning = false; // 只有释放鼠标时才知道是“点按”还是“点击”。
		CachedDestination = FVector::ZeroVector; // not necessary
		FollowTime = 0.f;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//UE_LOG(LogTemp, Warning, TEXT("保持：Tag = %s"), *InputTag.ToString());
	const auto AuraASC = GetASC();
	if (!AuraASC) return;

	//GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Blue, FString::Printf(TEXT("FollowTime = %fs"), FollowTime));

	// 与鼠标左键无关/鼠标当前位于敌人的位置上。
	// 不能在此处比较 bTargeting = ThisActor != nullptr，
	// 因为玩家可能在敌人身上点击左键，然后保持下压移动到别的地方，这种情况仍然判定为释放技能
	// 即：是释放技能还是点击移动其实在左键按下时已经决定（是否hit敌人）
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB) || bTargeting)
	{
		AuraASC->AbilityInputTagHeld(InputTag);
	}
	else
	{
		// 增加按下时间，在释放时要根据按下时间决定是click-release还是hold-release
		FollowTime += GetWorld()->GetDeltaSeconds();
		// 参考topdown模板，返回当前帧，保持按下时鼠标位置（逐帧更新）
		FHitResult HitResult;
		if (GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
		{
			CachedDestination = HitResult.ImpactPoint;
		}
		if (auto ControlledPawn = GetPawn())
		{
			const auto WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	const auto AuraASC = GetASC();
	if (!AuraASC) return;

	// 如果不是左键，则无此烦恼
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB) || bTargeting)
	{
		AuraASC->AbilityInputTagReleased(InputTag);
	}
	else
	{
		// 需要自动移动
		const auto ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn) // 点击
		{
			// 同步查找路径点
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				// NavPath->PathPoints 是路径点数组，包含起始点和终点
				Spline->ClearSplinePoints();
				for (const auto PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
				}
				// 短点击会自动移动
				bAutoRunning = true;
			}
		}
		else // hold press
		{
		}
		FollowTime = 0.f; // not necessary, have done in pressed.
		bTargeting = false; // not necessary, have adjust in pressed.
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (!AuraAbilitySystemComponent)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return AuraAbilitySystemComponent;
}

void AAuraPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D Movement = Value.Get<FVector2D>();
	if (Movement.IsZero())
	{
		return;
	}
	FRotator YawRotation{0.f, ControlRotation.Yaw, 0.f};
	FVector MoveDirection{Movement.Y, Movement.X, 0.f};
	MoveDirection = YawRotation.RotateVector(MoveDirection);
	if (const auto ControlledPawn = GetPawn())
	{
		ControlledPawn->AddMovementInput(MoveDirection, Movement.Size());
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		return;
	}

	/*
	 * 基本思路就是使用两个指针，一个用于当前命中Actor，另一个用于上帧命中Actor
	 *
	 */
	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	if (ThisActor != LastActor)
	{
		if (LastActor)
		{
			LastActor->UnHighlightActor();
		}
		if (ThisActor)
		{
			ThisActor->HighlightActor();
		}
	}
}
