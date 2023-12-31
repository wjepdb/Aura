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

#include "GameFramework/Character.h"

#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("样条线组件"));
	// 不需要挂接到根组件
	// Spline->SetupAttachment(GetRootComponent());
}

void AAuraPlayerController::AutoRun()
{
	APawn* ControlledPawn = GetPawn();
	if (bAutoRunning && ControlledPawn)
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(
			ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(
			LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

	AutoRun();
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter,
                                                            bool bBlockedHit, bool bCriticalHit)
{
	// DamageTextComponentClass 只是资产，并不会Pending Kill，只测试是否为null_ptr即可；
	// 但TargetCharacter不同，较好的做法是测试非空并非Pending Kill，因此使用::IsValid恰当。
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		auto DamageTextComp = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageTextComp->RegisterComponent();
		// attach 是为了定位，让文本组件初始在角色的RootComponent
		DamageTextComp->AttachToComponent(TargetCharacter->GetRootComponent(),
		                                  FAttachmentTransformRules::KeepRelativeTransform);

		DamageTextComp->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);

		//detach为了让文本组件不再跟随角色移动。
		DamageTextComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
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

	// 使用简化方法
	// AuraInputComponent->BindAction(IA_ShiftAction, ETriggerEvent::Started, this, &ThisClass::ShiftKeyPressed);
	// AuraInputComponent->BindAction(IA_ShiftAction, ETriggerEvent::Completed, this, &ThisClass::ShiftKeyReleased);

	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
	                                       &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_Shift))
	{
		// shift pressed.
		// 对教程的拓展
		bShiftKeyDown = true;
		return;
	}


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
	if (!AuraASC)
	{
		return;
	}

	//GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Blue, FString::Printf(TEXT("FollowTime = %fs"), FollowTime));

	// 与鼠标左键无关/鼠标当前位于敌人的位置上。
	// 不能在此处比较 bTargeting = ThisActor != nullptr，
	// 因为玩家可能在敌人身上点击左键，然后保持下压移动到别的地方，这种情况仍然判定为释放技能
	// 即：是释放技能还是点击移动其实在左键按下时已经决定（是否hit敌人）
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		AuraASC->AbilityInputTagHeld(InputTag);
		return;
	}

	if (bTargeting || bShiftKeyDown)
	{
		AuraASC->AbilityInputTagHeld(InputTag);
	}
	else
	{
		// 增加按下时间，在释放时要根据按下时间决定是click-release还是hold-release
		FollowTime += GetWorld()->GetDeltaSeconds();
		// 参考topdown模板，返回当前帧，保持按下时鼠标位置（逐帧更新）


		//FHitResult HitResult;
		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}
		if (const auto ControlledPawn = GetPawn())
		{
			const auto WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	const auto AuraASC = GetASC();
	if (!AuraASC)
	{
		return;
	}

	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_Shift))
	{
		bShiftKeyDown = false;
		return;
	}

	// 如果不是左键，则无此烦恼
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		AuraASC->AbilityInputTagReleased(InputTag);
		return;
	}

	AuraASC->AbilityInputTagReleased(InputTag);

	// 只有在没有目标也没有按下shift键（强制使用技能），才检测是否短点击
	if (!bTargeting && !bShiftKeyDown)
	{
		// 需要自动移动
		const auto ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn) // 点击
		{
			// 同步查找路径点
			// ReSharper disable once CppTooWideScope
			const UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this, ControlledPawn->GetActorLocation(), CachedDestination);
			if (NavPath)
			{
				// NavPath->PathPoints 是路径点数组，包含起始点和终点
				Spline->ClearSplinePoints();
				for (const auto PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					// FColor RandomColor = FColor::MakeRandomColor();
					// DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, RandomColor, false, 5.f);
				}
				// 短点击会自动移动
				bAutoRunning = true;

				// 新问题：如果点击的位置不在NavMesh上，则出现错误的导航。
				// 最简单的办法：
				/*
				 * 1：对于不位于导航网格上的物体，设置针对ECC_Visibility的碰撞为ignore。
				 *	  这种办法问题在于，如果点击物体的中心，由于忽略了可视通道，其实HitResult获取的是物体中心在导航网格上的投影，
				 *	  这就导致玩家一直向该点运行，却被物体阻挡！（bug）
				 *
				 * 2：较好的解决办法，就是取导航路径生成的导航点的最后一个点（不使用目标点，导航系统生成的点总是可达的）。
				 *	  具体在鼠标释放代码中体现。（修改CacheDestination）。
				 */
				const auto NumNavPoints = NavPath->PathPoints.Num();
				if (NumNavPoints >= 1)
				{
					CachedDestination = NavPath->PathPoints[NumNavPoints - 1];
				}
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
