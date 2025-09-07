// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnManager.h"

#include "TurnBased/TurnBasedCharacter.h"
#include "EnemieParent.h"
#include <CharacterCombatComponent.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
ATurnManager::ATurnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATurnManager::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void ATurnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurnManager::StartBattle()
{
	TurnOrder.Empty();
	TotalEnemies.Empty();


	//Define who starts first and prob animation of battle start

	Player = Cast<ATurnBasedCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	CharacterCombatComponent = Player->FindComponentByClass<UCharacterCombatComponent>();


	for (AEnemieParent* Enemy : Player->Enemies)
	{
		TotalEnemies.Add(Enemy);
		UCharacterCombatComponent* EnemyCombatComponent = Enemy->FindComponentByClass<UCharacterCombatComponent>();

		TurnOrder.Add({ Enemy, EnemyCombatComponent->Speed, EnemyCombatComponent->CharacterIcon });

		EnemyCombatComponent->OnCharacterDied.Unbind();
		EnemyCombatComponent->OnCharacterDied.BindUObject(this, &ATurnManager::OnCharacterDied);

	}

	

	TurnOrder.Add({ Player, CharacterCombatComponent->Speed, CharacterCombatComponent->CharacterIcon });

	TurnOrder.Sort();

	IconTurn = SortIconTurns();
	
	DecideNextTurn();
}

void ATurnManager::DecideNextTurn()
{

	if(CurrentTurnVal>=TurnOrder.Num())
	{
		//Reset turn pos
		CurrentTurnVal = 0;
	}
	IconTurn = SortIconTurns();
	if(Cast<ATurnBasedCharacter>(TurnOrder[CurrentTurnVal].Actor))
	{

		PlayerTurnManager();
		
	}
	else
	{
		EnemyTurnManager(Cast<AEnemieParent>(TurnOrder[CurrentTurnVal].Actor));
	}
	//Get the map of actors and speed and start the next actor

	CurrentTurnVal++;

}
TArray<UTexture2D*>ATurnManager::SortIconTurns()
{
	IconTurn.Empty();
	TArray<UTexture2D*> Icons;


	if (TurnOrder.Num() == 0) return Icons;

	// Empezar desde el actor actual
	for (int32 i = 0; i < TurnOrder.Num(); i++)
	{
		int32 Index = (CurrentTurnVal + i) % TurnOrder.Num();
		if (TurnOrder[Index].CharacterIcon)
		{
			Icons.Add(TurnOrder[Index].CharacterIcon);
		}
	}

	return Icons;

}
void ATurnManager::PlayerTurnManager()
{

	//Set an animation
	//Start the character turn (Enable the attack UI and controls)

	UCharacterCombatComponent* PlayerCombat = Player->FindComponentByClass<UCharacterCombatComponent>();
	if (PlayerCombat)
	{
		PlayerCombat->OnAttackFinished.Unbind();
		PlayerCombat->OnAttackFinished.BindUObject(this, &ATurnManager::DecideNextTurn);
	}

	Player->CanAttack();

	

}

void ATurnManager::OnCharacterDied(AActor* DeadActor)
{

	TurnOrder.RemoveAll([DeadActor](const FTurnOrderStruct& Entry)
		{
			return Entry.Actor == DeadActor;
		});

	AEnemieParent* DeadEnemy = Cast<AEnemieParent>(DeadActor);
	if (DeadEnemy && Player)
	{
		Player->Enemies.Remove(DeadEnemy);
	}

	if(CheckBattleEnd())
	{
		//GG
	}

}

void ATurnManager::EnemyTurnManager(AEnemieParent* Enemy)
{

	UCharacterCombatComponent* EnemyCombatComponent = Enemy->FindComponentByClass<UCharacterCombatComponent>();

	if (EnemyCombatComponent)
	{
		EnemyCombatComponent->OnAttackFinished.Unbind();
		EnemyCombatComponent->OnAttackFinished.BindUObject(this, &ATurnManager::DecideNextTurn);
	}


	EnemyCombatComponent->Attack(EnemyCombatComponent->AvailableAbilities[0], CharacterCombatComponent);

	
	//Animation and camera focusing on the enemy
	//Start the attack manager of the enemy

}



bool ATurnManager::CheckBattleEnd()
{

	//At the end of each turn check if its over

	return false;

}

void ATurnManager::EndBattle()
{

	//Give the reward (Exp or whatever)
	//Change the mode and exit the attack

}

