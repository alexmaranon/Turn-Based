// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "AbilitySystemInterface.h"
#include "AttackAbility.h"
#include "CharacterCombatComponent.generated.h"



class UAttackAbility;

DECLARE_DELEGATE_OneParam(FOnCharacterDied, AActor*);
DECLARE_DELEGATE(FOnAttackFinished)

USTRUCT(BlueprintType)
struct FAttackSlot
{
	GENERATED_BODY()
	//Posibility to add more things
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AttackRowName;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TURNBASED_API UCharacterCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	FOnCharacterDied OnCharacterDied;
	FOnAttackFinished OnAttackFinished;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentEnergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAttackAbility*> AvailableAbilities;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* TakeDamageAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAttackSlot> EquippedAttacks;

	UFUNCTION(BlueprintCallable)
	void Attack(UAttackAbility* Ability, UCharacterCombatComponent* Target);

	UFUNCTION(BlueprintCallable)
	void ReceiveDamage(float InDamage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* AttacksDT;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UTexture2D* CharacterIcon;

	UPROPERTY(EditAnywhere, Category="PruebaCarlius")
	bool MostrarOpciones;

	UPROPERTY(EditAnywhere, Category = "PruebaCarlius", meta = (EditCondition = "MostrarOpciones", EditConditionHides))
	float Mondongo;

	UPROPERTY(EditAnywhere, Category = "PruebaCarlius", meta = (EditCondition = "MostrarOpciones", EditConditionHides))
	FVector Patata;

	void initializeAttacks();
		
};
