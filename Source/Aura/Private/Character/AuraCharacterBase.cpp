// 版权在左，一切自由。
#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"

AAuraCharacterBase::AAuraCharacterBase() {
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const {
	return AbilitySystemComponent;
}

UAttributeSet* AAuraCharacterBase::GetAttributeSet() const {
	return AttributeSet;
}

void AAuraCharacterBase::BeginPlay() {
	Super::BeginPlay();
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float level) const {
	check(IsValid(GameplayEffectClass));

	auto ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);

	const auto& SpecHandle = GetAbilitySystemComponent()->
		MakeOutgoingSpec(GameplayEffectClass, level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
}

void AAuraCharacterBase::InitializeDefaultAttributes() const {
	if (!this->Implements<UCombatInterface>()) {
		checkf(0, TEXT("没有实现ICombatInterface接口"));
		return;
	}

	auto CombatInterface = CastChecked<ICombatInterface>(const_cast<AAuraCharacterBase*>(this));

	auto DefaultPlayerLevel = CombatInterface->GetLevel();

	ApplyEffectToSelf(DefaultPrimaryAttributes, DefaultPlayerLevel);
	ApplyEffectToSelf(DefaultSecondaryAttributes, DefaultPlayerLevel);
	ApplyEffectToSelf(DefaultVitalAttributes, DefaultPlayerLevel);
}
