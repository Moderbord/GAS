// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "GAS/GAS.h"
#include "Abilities/CharacterAbilitySystemComponent.h"
#include "Abilities/AttributeSets/CharacterAttributeSetBase.h"
#include "Abilities/CharacterGameplayAbility.h"
#include "GASCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, AGASCharacterBase*, character);

UCLASS(config=Game)
class AGASCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGASCharacterBase(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable, Category = "GAS|Character")
	FCharacterDiedDelegate OnCharacterDied;
	
	UFUNCTION(BlueprintCallable, Category = "GAS|Character")
	virtual bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|Character")
	virtual int32 GetAbilityLevel(GASAbilityID AbilityID) const;

	// Removes all CharacterAbilities. Can only be called by the Server. Removing on the Server will remove from Client too.
	UFUNCTION(BlueprintCallable, Category = "GAS|Character")
	virtual void RemoveCharacterAbilities();

	UFUNCTION(BlueprintCallable, Category = "GAS|Character")
	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "GAS|Character")
	virtual void FinishDying();

	/**
	* Getters for attributes from GDAttributeSetBase
	**/

	UFUNCTION(BlueprintCallable, Category = "GAS|Character|Attributes")
	int32 GetCharacterLevel() const;
	
	UFUNCTION(BlueprintCallable, Category = "GAS|Character|Attributes")
	float GetHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "GAS|Character|Attributes")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintCallable, Category = "GAS|Character|Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|Character|Attributes")
	float GetMaxMana() const;

	// Implemented IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Getters for subobjects
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:

	TWeakObjectPtr<class UCharacterAbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<class UCharacterAttributeSetBase> AttributeSetBase;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Character")
	FText CharacterName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Demo|Animation")
	UAnimMontage* DeathMontage;

	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
	TArray<TSubclassOf<class UCharacterGameplayAbility>> CharacterAbilities;

	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	//UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay();
	
	virtual void AddCharacterAbilites();

	virtual void InitializeAttributes();

	virtual void AddStartupEffets();

	virtual void SetHealth(float Health);

	virtual void SetMana(float Mana);

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

};

