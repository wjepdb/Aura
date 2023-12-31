// 版权在左，一切自由。

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UDamageTextComponent;
class USplineComponent;
class UAuraAbilitySystemComponent;
class UAuraInputConfig;
class IEnemyInterface;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	void AutoRun();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "输入")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditDefaultsOnly, Category = "输入")
	TObjectPtr<UInputAction> IA_MoveAction;

	/* 教程中使用ShiftAction实现检测是否shift键按下，尝试利用了InputConfig，侦听shift是否按下 */
	// UPROPERTY(EditDefaultsOnly, Category = "输入")
	// TObjectPtr<UInputAction> IA_ShiftAction;
	// void ShiftKeyPressed() { bShiftKeyDown = true; }
	// void ShiftKeyReleased() { bShiftKeyDown = false; }

	bool bShiftKeyDown = false;


	UPROPERTY(EditDefaultsOnly, Category = "输入")
	TObjectPtr<UAuraInputConfig> InputConfig;

	void Move(const FInputActionValue& Value);

	void CursorTrace();
	FHitResult CursorHit;

	IEnemyInterface* LastActor = nullptr;
	IEnemyInterface* ThisActor = nullptr;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	const float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly, Category = "寻路")
	float AutoRunAcceptRadius = 50.f;

	UPROPERTY(VisibleAnywhere, Category = "寻路")
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};
