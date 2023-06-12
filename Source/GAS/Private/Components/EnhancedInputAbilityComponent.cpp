// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EnhancedInputAbilityComponent.h"

namespace EnhancedInputAbilityComponent
{
	constexpr int32 InvalidInputID = 0;
	int32 IncrementingInputID = InvalidInputID;

	static int32 GetNextInputID()
	{
		return ++IncrementingInputID;
	}
}

void UEnhancedInputAbilityComponent::SetInputBinding(UInputAction* InputAction, FGameplayAbilitySpecHandle AbilityHandle)
{
	using namespace EnhancedInputAbilityComponent;

	FGameplayAbilitySpec* BindingAbility = FindAbilitySpec(AbilityHandle);

	FAbilityInputBinding* AbilityInputBinding = MappedAbilities.Find(InputAction);
	if (AbilityInputBinding)
	{
		FGameplayAbilitySpec* OldBoundAbility = FindAbilitySpec(AbilityInputBinding->BoundAbilitiesStack.Top());
		if (OldBoundAbility && OldBoundAbility->InputID == AbilityInputBinding->InputID)
		{
			OldBoundAbility->InputID = InvalidInputID;
		}
	}
	else
	{
		AbilityInputBinding = &MappedAbilities.Add(InputAction);
		AbilityInputBinding->InputID = GetNextInputID();
	}

	if (BindingAbility)
	{
		BindingAbility->InputID = AbilityInputBinding->InputID;
	}

	AbilityInputBinding->BoundAbilitiesStack.Push(AbilityHandle);
	TryBindAbilityInput(InputAction, *AbilityInputBinding);
}

void UEnhancedInputAbilityComponent::ClearInputBinding(FGameplayAbilitySpecHandle AbilityHandle)
{
	using namespace EnhancedInputAbilityComponent;

	if (FGameplayAbilitySpec* FoundAbility = FindAbilitySpec(AbilityHandle))
	{
		// Find the mapping for this ability
		auto MappedIterator = MappedAbilities.CreateIterator();
		while (MappedIterator)
		{
			if (MappedIterator.Value().InputID == FoundAbility->InputID)
			{
				break;
			}

			++MappedIterator;
		}

		if (MappedIterator)
		{
			FAbilityInputBinding& AbilityInputBinding = MappedIterator.Value();

			if (AbilityInputBinding.BoundAbilitiesStack.Remove(AbilityHandle) > 0)
			{
				if (AbilityInputBinding.BoundAbilitiesStack.Num() > 0)
				{
					FGameplayAbilitySpec* StackedAbility = FindAbilitySpec(AbilityInputBinding.BoundAbilitiesStack.Top());
					if (StackedAbility && StackedAbility->InputID == 0)
					{
						StackedAbility->InputID = AbilityInputBinding.InputID;
					}
				}
				else
				{
					// NOTE: This will invalidate the `AbilityInputBinding` ref above
					RemoveEntry(MappedIterator.Key());
				}

				// DO NOT act on `AbilityInputBinding` after here (it could have been removed)
				FoundAbility->InputID = InvalidInputID;
			}
		}
	}
}

void UEnhancedInputAbilityComponent::ClearAbilityBindings(UInputAction* InputAction)
{
	RemoveEntry(InputAction);
}

void UEnhancedInputAbilityComponent::SetEnhancedInputComponent(UEnhancedInputComponent* InputComponent)
{
	ActorInputComponent = InputComponent;
}

void UEnhancedInputAbilityComponent::OnAbilityInputPressed(UInputAction* InputAction)
{
	using namespace EnhancedInputAbilityComponent;

	FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction);
	if (FoundBinding && ensure(FoundBinding->InputID != InvalidInputID))
	{
		AbilityLocalInputPressed(FoundBinding->InputID);
	}
}

void UEnhancedInputAbilityComponent::OnAbilityInputReleased(UInputAction* InputAction)
{
	using namespace EnhancedInputAbilityComponent;

	FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction);
	if (FoundBinding && ensure(FoundBinding->InputID != InvalidInputID))
	{
		AbilityLocalInputReleased(FoundBinding->InputID);
	}
}

void UEnhancedInputAbilityComponent::RemoveEntry(UInputAction* InputAction)
{
	if (FAbilityInputBinding* Bindings = MappedAbilities.Find(InputAction))
	{
		if (ActorInputComponent)
		{
			ActorInputComponent->RemoveBindingByHandle(Bindings->OnPressedHandle);
			ActorInputComponent->RemoveBindingByHandle(Bindings->OnReleasedHandle);
		}

		for (FGameplayAbilitySpecHandle AbilityHandle : Bindings->BoundAbilitiesStack)
		{
			using namespace EnhancedInputAbilityComponent;

			FGameplayAbilitySpec* AbilitySpec = FindAbilitySpec(AbilityHandle);
			if (AbilitySpec && AbilitySpec->InputID == Bindings->InputID)
			{
				AbilitySpec->InputID = InvalidInputID;
			}
		}

		MappedAbilities.Remove(InputAction);
	}
}

void UEnhancedInputAbilityComponent::TryBindAbilityInput(UInputAction* InputAction, FAbilityInputBinding& AbilityInputBinding)
{
	if (ActorInputComponent)
	{
		// Pressed event
		if (AbilityInputBinding.OnPressedHandle == 0)
		{
			AbilityInputBinding.OnPressedHandle = ActorInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &UEnhancedInputAbilityComponent::OnAbilityInputPressed, InputAction).GetHandle();
		}

		// Released event
		if (AbilityInputBinding.OnReleasedHandle == 0)
		{
			AbilityInputBinding.OnReleasedHandle = ActorInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &UEnhancedInputAbilityComponent::OnAbilityInputReleased, InputAction).GetHandle();
		}
	}
}

FGameplayAbilitySpec* UEnhancedInputAbilityComponent::FindAbilitySpec(FGameplayAbilitySpecHandle Handle)
{
	FGameplayAbilitySpec* FoundAbility = nullptr;
	FoundAbility = FindAbilitySpecFromHandle(Handle);
	return FoundAbility;
}

void UEnhancedInputAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
}
