// 版权在左，一切自由。
#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Aura.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "Components/CapsuleComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AAuraCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

FVector AAuraCharacterBase::GetCombatSocketLocation()
{
	return Weapon ? Weapon->GetSocketLocation(WeaponTipSocketName) : FVector::ZeroVector;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float level) const
{
	check(IsValid(GameplayEffectClass));

	auto ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	const auto& SpecHandle = GetAbilitySystemComponent()->
		MakeOutgoingSpec(GameplayEffectClass, level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	if (!this->Implements<UCombatInterface>())
	{
		checkf(0, TEXT("没有实现ICombatInterface接口"));
		return;
	}

	auto CombatInterface = CastChecked<ICombatInterface>(const_cast<AAuraCharacterBase*>(this));

	auto DefaultPlayerLevel = CombatInterface->GetPlayerLevel();

	ApplyEffectToSelf(DefaultPrimaryAttributes, DefaultPlayerLevel);
	ApplyEffectToSelf(DefaultSecondaryAttributes, DefaultPlayerLevel);
	ApplyEffectToSelf(DefaultVitalAttributes, DefaultPlayerLevel);
}


// 技能通常在服务器方授予，然后由ABS复制（replicate）到owning-client。
void AAuraCharacterBase::AddCharacterAbilities()
{
	auto AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);

	if (!HasAuthority())
	{
		return;
	}

	// 授予技能
	AuraASC->AddCharacterAbilities(StartupAbilities);
}
