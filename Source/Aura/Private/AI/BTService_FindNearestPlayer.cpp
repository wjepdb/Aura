// 版权在左，一切自由。


#include "AI/BTService_FindNearestPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// FString DebugMsg = FString::Printf(TEXT("AIOwner = %s, Owner = %s"), *AIOwner->GetName(), *ActorOwner->GetName());
	// GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, DebugMsg);
	//
	auto OwningPawn = AIOwner->GetPawn();
	// 找出所有玩家“control”pawn
	// 比较简单的方法是在需要标记的Actor上添加``Tag''
	const auto TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsWithTag(this, TargetTag, Players);

	const auto SelfPosition = OwningPawn->GetActorLocation();
	float BestDistance = 1000000000000.f;
	AActor* NearestPlayer = nullptr;
	for (const auto& actor : Players)
	{
		float distance = (actor->GetActorLocation() - SelfPosition).SquaredLength();
		if (distance < BestDistance)
		{
			BestDistance = distance;
			NearestPlayer = actor;
		}
	}
	if (NearestPlayer)
	{
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red,
		                                 FString::Printf(TEXT("最近%f,名字:%s"), BestDistance, *NearestPlayer->GetName()));
	}
}
