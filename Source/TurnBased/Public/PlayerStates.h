// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerState.h"
#include "PlayerStates.generated.h"

class UAttackAbility;
class UCharacterCombatComponent;
UCLASS()
class TURNBASED_API APlayerStates : public APlayerState
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	TArray<UAttackAbility*> GetCurrentAbilities();

	

};
