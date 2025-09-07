// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemieParent.h"
#include "EnemieParent.h"

#include "CharacterCombatComponent.h"

// Sets default values
AEnemieParent::AEnemieParent()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CombatComponent = CreateDefaultSubobject<UCharacterCombatComponent>(TEXT("CombatComponent"));

}

// Called when the game starts or when spawned
void AEnemieParent::BeginPlay()
{
	Super::BeginPlay();


	
}

// Called every frame
void AEnemieParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemieParent::ReceiveDamage(float InDamage)
{

	CombatComponent->CurrentHP -= InDamage;
	if (CombatComponent->CurrentHP <=0 )
	{
		//Destroy(this);
	} 

}

// Called to bind functionality to input
void AEnemieParent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

