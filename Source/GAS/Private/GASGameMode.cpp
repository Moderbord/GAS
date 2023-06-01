// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASGameMode.h"
#include "Character/GASCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGASGameMode::AGASGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
