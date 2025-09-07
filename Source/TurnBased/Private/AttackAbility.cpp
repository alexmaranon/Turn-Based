// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackAbility.h"

void UAttackAbility::InitializeAttack(FAttackAttributes& AttackData)
{
	AttackAttributes = AttackData;
}

void UAttackAbility::Attack(AActor* Target)
{
	if(Target)
	{

		//Remove cost
		//Deal damage (Call damage func)

	}
}
