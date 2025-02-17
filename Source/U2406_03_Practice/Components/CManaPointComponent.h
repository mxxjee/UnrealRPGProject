// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CManaPointComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FManaPointChanged, float, InMana, float, InMaxMana);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U2406_03_PRACTICE_API UCManaPointComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Mana")
		float MaxMana = 256;
public:	
	// Sets default values for this component's properties
	UCManaPointComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FManaPointChanged OnManaPointChanged;
public:
	FORCEINLINE float GetMana() { return ManaPoint; }
	FORCEINLINE float GetMaxMana() { return MaxMana; }


public:
	bool CanUseSkill(int InSkillMP);
	void SetCurrentManaPoint(int InSkillMP);
	void SetAddValue(float InAddValue) { AddValue = InAddValue; }
	void SetAddManaPoint(int InSkillMP);
private:
	float AddValue=0.01f;
	float ManaPoint;
	

};
