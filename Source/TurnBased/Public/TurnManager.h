// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurnManager.generated.h"


class UCharacterCombatComponent;
class AEnemieParent;
class ATurnBasedCharacter;


USTRUCT()
struct FTurnOrderStruct
{
	GENERATED_BODY()

	AActor* Actor;

	float Speed;

	UTexture2D* CharacterIcon;

	bool operator<(const FTurnOrderStruct& Other) const
	{
		return Speed > Other.Speed;  // Orden descendente (más rápido primero)
	}
};

UCLASS()
class TURNBASED_API ATurnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int CurrentTurnVal=0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ATurnBasedCharacter* Player;

	TArray<AEnemieParent*> TotalEnemies;

	UCharacterCombatComponent* CharacterCombatComponent;

	TArray<FTurnOrderStruct> TurnOrder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UTexture2D*> IconTurn;

	UFUNCTION(BlueprintCallable)
	void StartBattle();

	void DecideNextTurn();

	void PlayerTurnManager();

	void OnCharacterDied(AActor* DeadActor);

	void EnemyTurnManager(AEnemieParent* Enemy);

	TArray<UTexture2D*> SortIconTurns();

	bool CheckBattleEnd();

	void EndBattle();

};
