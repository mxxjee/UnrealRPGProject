// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CManaPointComponent.h"
#include "CManaPointComponent.h"
#include "Global.h"

UCManaPointComponent::UCManaPointComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

	ManaPoint = MaxMana;


}


// Called when the game starts
void UCManaPointComponent::BeginPlay()
{
	Super::BeginPlay();


	
}


// Called every frame
void UCManaPointComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckTrue(ManaPoint == MaxMana);
	
	FTimerHandle timerhandle;
	FTimerDelegate timerDelegate;

	//1초마다 마나는 증가
	timerDelegate.BindLambda([=] {
		ManaPoint += 0.01f;

		if (OnManaPointChanged.IsBound())
			OnManaPointChanged.Broadcast(ManaPoint, MaxMana);
	});

	GetWorld()->GetTimerManager().SetTimer(timerhandle, timerDelegate, 1, false);
}

bool UCManaPointComponent::CanUseSkill(int InSkillMP)
{
	if (ManaPoint - InSkillMP >= 0)
		return true;

	else
		return false;
}

void UCManaPointComponent::SetCurrentManaPoint(int InSkillMP)
{


	ManaPoint -= InSkillMP;

	ManaPoint = FMath::Clamp(ManaPoint, 0.0f, MaxMana);

	if (OnManaPointChanged.IsBound())
		OnManaPointChanged.Broadcast(ManaPoint, MaxMana);
}

void UCManaPointComponent::SetAddManaPoint(int InSkillMP)
{

	ManaPoint += InSkillMP;

	ManaPoint = FMath::Clamp(ManaPoint, 0.0f, MaxMana);

	if (OnManaPointChanged.IsBound())	//Heal효과(Particle 생성)
		OnManaPointChanged.Broadcast(ManaPoint, MaxMana);
}

