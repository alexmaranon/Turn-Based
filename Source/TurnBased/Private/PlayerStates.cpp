// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates.h"
#include "CharacterCombatComponent.h"
#include "AttackAbility.h"

TArray<UAttackAbility*> APlayerStates::GetCurrentAbilities()
{
	UCharacterCombatComponent* CombatComponent = GetPawn()->FindComponentByClass<UCharacterCombatComponent>();

	
	if (CombatComponent)
	{
		return CombatComponent->AvailableAbilities;
	}

	return TArray<UAttackAbility*>();

}
