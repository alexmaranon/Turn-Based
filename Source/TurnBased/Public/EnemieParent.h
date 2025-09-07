// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemieParent.generated.h"

class UCharacterCombatComponent;

UCLASS()
class TURNBASED_API AEnemieParent : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemieParent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCharacterCombatComponent* CombatComponent;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ReceiveDamage(float InDamage);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
