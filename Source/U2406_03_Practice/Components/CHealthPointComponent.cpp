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
	//InAmount�� �������.
	Health += (InAmount * (-1.0f));


	//�ִ�~�ּҰ�
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);


	if (OnHealthPointChanged.IsBound())
		OnHealthPointChanged.Broadcast(Health, MaxHealth);
}

void UCHealthPointComponent::Heal(float InAmount)
{
	//InAmount�� �������.
	Health += (InAmount);


	//�ִ�~�ּҰ�
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);


	if (OnHealthPointChanged.IsBound())
		OnHealthPointChanged.Broadcast(Health, MaxHealth);

	if (OnHeal.IsBound())
		OnHeal.Broadcast();

}



