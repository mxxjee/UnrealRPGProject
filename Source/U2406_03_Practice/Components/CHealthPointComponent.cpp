// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CHealthPointComponent.h"

// Sets default values for this component's properties
UCHealthPointComponent::UCHealthPointComponent()
{
	
}


// Called when the game starts
void UCHealthPointComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	
	
}


void UCHealthPointComponent::Damage(float InAmount)
{
	//InAmount는 양수값임.
	Health += (InAmount * (-1.0f));


	//최대~최소값
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);


	if (OnHealthPointChanged.IsBound())
		OnHealthPointChanged.Broadcast(Health, MaxHealth);
}

void UCHealthPointComponent::Heal(float InAmount)
{
	//InAmount는 양수값임.
	Health += (InAmount);


	//최대~최소값
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);


	if (OnHealthPointChanged.IsBound())
		OnHealthPointChanged.Broadcast(Health, MaxHealth);

	if (OnHeal.IsBound())
		OnHeal.Broadcast();

}



