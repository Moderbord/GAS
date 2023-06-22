// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Abilities/GameplayEffectCalculation/GEDamageExecCalculation.h"
#include "Character/Abilities/CharacterAbilitySystemComponent.h"
#include "Character/Abilities/AttributeSets/CharacterAttributeSetBase.h"

struct GEDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	GEDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UCharacterAttributeSetBase, Damage, Source, true);
	}
};

static const GEDamageStatics& DamageStatics()
{
	static GEDamageStatics DStatics;
	return DStatics;
}

UGEDamageExecCalculation::UGEDamageExecCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
}

void UGEDamageExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Damage = 0.f;
	// Capture optional damage value based on 'CalculationModifier' 
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, Damage);
	// Add potential 'SetByCaller' damage
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false), 0.f);

	// Add any modifiers here
	float UnmodifiedDamage = Damage;
	float ModifiedDamage = UnmodifiedDamage;

	if (ModifiedDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, ModifiedDamage));
	}

	UCharacterAbilitySystemComponent* TargetASC = Cast<UCharacterAbilitySystemComponent>(TargetAbilitySystemComponent);
	if (TargetASC)
	{
		UCharacterAbilitySystemComponent* SourceASC = Cast<UCharacterAbilitySystemComponent>(SourceAbilitySystemComponent);
		TargetASC->ReceiveDamage(SourceASC, UnmodifiedDamage, ModifiedDamage);
	}
}
