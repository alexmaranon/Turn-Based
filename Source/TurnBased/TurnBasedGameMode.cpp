// Copyright Epic Games, Inc. All Rights Reserved.

#include "TurnBasedGameMode.h"
#include "TurnBasedCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATurnBasedGameMode::ATurnBasedGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
