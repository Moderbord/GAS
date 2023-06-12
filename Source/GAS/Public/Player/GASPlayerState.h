// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "Components/EnhancedInputAbilityComponent.h"
#include "Character/Abilities/AttributeSets/CharacterAttributeSetBase.h"
#include "GASPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API AGASPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGASPlayerState();

	class UEnhancedInputAbilityComponent* GetAbilitySystemComponent() const override;

	class UCharacterAttributeSetBase* GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable, Category = "Demo|GASPlayerState")
	bool IsAlive() const;
	
	UFUNCTION(BlueprintCallable, Category = "Demo|GASPlayerState")
	void ShowAbilityConfirmCancelText(bool ShowText);

	UFUNCTION(BlueprintCallable, Category = "Demo|GASPlayerState|Attributes")
	float GetCharacterLevel() const;
	
	UFUNCTION(BlueprintCallable, Category = "Demo|GASPlayerState|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Demo|GASPlayerState|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Demo|GASPlayerState|Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "Demo|GASPlayerState|Attributes")
	float GetMaxMana() const;

protected:

	UPROPERTY()
	class UEnhancedInputAbilityComponent* AbilitySystemComponent;
	
	UPROPERTY()
	class UCharacterAttributeSetBase* AttributeSetBase;

	FGameplayTag DeadTag;

	FDelegateHandle CharacterLevelChangedDelegateHandle;
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle ManaChangedDelegateHandle;
	FDelegateHandle MaxManaChangedDelegateHandle;

	virtual void BeginPlay() override;

	virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void ManaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxManaChanged(const FOnAttributeChangeData& Data);

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);


};
