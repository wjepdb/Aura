// 版权在左，一切自由。

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

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
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "输入")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditDefaultsOnly, Category = "输入")
	TObjectPtr<UInputAction> IA_MoveAction;

	void Move(const FInputActionValue& Value);

	void CursorTrace();

	IEnemyInterface* LastActor = nullptr;
	IEnemyInterface* ThisActor = nullptr;
};
