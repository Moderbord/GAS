// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEDamageExecCalculation.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UGEDamageExecCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	

public:
	UGEDamageExecCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
