// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASGameMode.h"
#include "Character/Hero/GASHeroCharacter.h"
#include "Player/GASPlayerController.h"
#include "Player/GASPlayerState.h"
#include "UObject/ConstructorHelpers.h"

AGASGameMode::AGASGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/Characters/Hero/BP_GASHeroCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	//static ConstructorHelpers::FClassFinder<APlayerState> PlayerStateBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_GASHeroCharacter"));

	//static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_GASHeroCharacter"));

}
