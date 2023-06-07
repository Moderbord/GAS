// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/GASPlayerController.h"
#include "Player/GASPlayerState.h"

void AGASPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AGASPlayerState* PlayerState = GetPlayerState<AGASPlayerState>();
	if (PlayerState)
	{
		PlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerState, InPawn);
	}
}
