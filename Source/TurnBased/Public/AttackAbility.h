// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AttackAbility.generated.h"

/**
 * 
 */

USTRUCT()
struct FAttackAttributes : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName AttackName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString AttackDescription;
	//UPROPERTY(EditAnywhere) FGameplayTag AttackType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Cost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Anim;

};

UCLASS()
class TURNBASED_API UAttackAbility : public UObject
{
	GENERATED_BODY()

	

public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FAttackAttributes AttackAttributes;
	void InitializeAttack(FAttackAttributes& AttackData);
	virtual void Attack(AActor* Target);
};
