// 版权在左，一切自由。

#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "Camera/CameraComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"

#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 600.0f; // The camera follows at this distance behind the character
	SpringArm->SetRelativeRotation(FRotator{-45.f, 0.f, 0.f});
	SpringArm->bUsePawnControlRotation = false; // Rotate the arm based on the controller
	SpringArm->bEnableCameraLag = true;
	SpringArm->SocketOffset = FVector{0.f, 75.f, 75.f};
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator{0.0f, 720.0f, 0.0f}; // ...at this rotation rate
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// 客户端调用
	InitAbilityActorInfo();
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// 服务端调用
	InitAbilityActorInfo();
	// 添加技能，注意 PossessedBy 函数只在服务端调用，客户端对应的是 OnRep_PlayerState
	AddCharacterAbilities();
}

int32 AAuraCharacter::GetPlayerLevel()
{
	const auto AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	if (const auto AuraABS = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		AuraABS->AbilityActorInfoSet();
	}

	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);

	// 此处保证PlayerController、PlayerState、AbilitySystemComponent及AttributeSet已经完成初始化。
	const auto PC = Cast<AAuraPlayerController>(GetController());

	// 对于模拟客户端，无PlayerController
	// Hud也仅对local control有效，因此要进一步检查
	// 服务端存在多个PlayerController，但最多只有一个是Local Control（Listen Server情况）。
	if (PC && IsLocallyControlled())
	{
		const auto Hud = Cast<AAuraHUD>(PC->GetHUD());

		check(Hud);

		Hud->InitOverlay(PC, AuraPlayerState, AbilitySystemComponent, AttributeSet);
	}

	InitializeDefaultAttributes();
}
