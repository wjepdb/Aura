// 版权在左，一切自由。


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	FString DebugMsg = FString::Printf(TEXT("AIOwner = %s, Owner = %s"), *AIOwner->GetName(), *ActorOwner->GetName());
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, DebugMsg);
}
