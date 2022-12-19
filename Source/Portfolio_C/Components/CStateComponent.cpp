#include "CStateComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/CharacterInterface.h"
#include "Components/CStatusComponent.h"

UCStateComponent::UCStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ACharacter* owner = Cast<ACharacter>(GetOwner());
	CheckNull(owner);

	if (owner->GetCharacterMovement()->IsFalling() && IsStateIdle())
		SetStateJump();

	if (owner->GetCharacterMovement()->IsFalling() == false && IsStateJump())
		SetStateIdle();

	ICharacterInterface* character = Cast<ICharacterInterface>(GetOwner());
	CheckNull(character);
	UCStatusComponent* status = character->GetStatus();
	CheckNull(status);

	if (status->GetHealth() <= 0.f)
		SetStateDead();
}

