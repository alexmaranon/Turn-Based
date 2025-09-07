// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterCombatComponent.h"
#include "AttackAbility.h"
#include "GameFramework/Character.h"
#include <TurnBased/TurnBasedCharacter.h>

// Sets default values for this component's properties
UCharacterCombatComponent::UCharacterCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	

	// ...
}


// Called when the game starts
void UCharacterCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentEnergy = MaxEnergy;
	CurrentHP = MaxHP;
	initializeAttacks();
	
}


// Called every frame
void UCharacterCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCharacterCombatComponent::Attack(UAttackAbility* Ability, UCharacterCombatComponent* Target)
{

	if(Ability->AttackAttributes.Cost<=CurrentEnergy)
	{

		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
		if (OwnerCharacter && Ability->AttackAttributes.Anim)
		{
			float Duration = OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_Play(Ability->AttackAttributes.Anim);

			if (Duration > 0.f)
			{

				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(
					TimerHandle,
					[this,Ability,Target]() {
						//Manage effectiveness rates
						CurrentEnergy -= Ability->AttackAttributes.Cost;
						Target->ReceiveDamage(Ability->AttackAttributes.Damage);


						FTimerHandle FinishTimer;
						GetWorld()->GetTimerManager().SetTimer(
							FinishTimer,
							[this]() {
								if (OnAttackFinished.IsBound())
								{
									OnAttackFinished.Execute();
								}
							}, 1.0f, false);


					}, Duration, false);

				

			}
			else
			{
				if (OnAttackFinished.IsBound())
				{
					OnAttackFinished.Execute();
				}
			}
		}
		else
		{
			if (OnAttackFinished.IsBound())
			{
				OnAttackFinished.Execute();
			}
		}
	}
}

void UCharacterCombatComponent::ReceiveDamage(float InDamage)
{

	ATurnBasedCharacter* Player = Cast<ATurnBasedCharacter>(GetOwner());

	if(Player)
	{
		if(Player->IsDodging||Player->IsParrying)
		{
			
		}else
		{
			CurrentHP -= InDamage;
			ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
			if (OwnerCharacter && TakeDamageAnim)
			{
				OwnerCharacter->PlayAnimMontage(TakeDamageAnim);
			}

			if (CurrentHP <= 0)
			{
				OnCharacterDied.Execute(GetOwner());
				GetOwner()->Destroy();
				//Character dead
			}
		}
	}
	else
	{
		CurrentHP -= InDamage;

		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
		if (OwnerCharacter && TakeDamageAnim)
		{
			OwnerCharacter->PlayAnimMontage(TakeDamageAnim);
		}

		if (CurrentHP <= 0)
		{
			OnCharacterDied.Execute(GetOwner());
			GetOwner()->Destroy();
			//Character dead
		}
	}

	
}

void UCharacterCombatComponent::initializeAttacks()
{

	for(const auto AttackName : EquippedAttacks)
	{
		FAttackAttributes* AttackData = AttacksDT->FindRow<FAttackAttributes>(AttackName.AttackRowName, TEXT(""));

		if(AttackData)
		{
			//Create empty attack
			UAttackAbility* NewAttack = NewObject<UAttackAbility>(this, UAttackAbility::StaticClass());
			//Initialize the attack with the given attack data
			NewAttack->InitializeAttack(*AttackData);
			AvailableAbilities.Add(NewAttack);
		}

	}

}

