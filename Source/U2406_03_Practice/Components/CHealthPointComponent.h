// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CHealthPointComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthPointChanged, float, InHealth, float, InMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHeal);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U2406_03_PRACTICE_API UCHealthPointComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 100;

public:	
	// Sets default values for this component's properties
	UCHealthPointComponent();

public:
	FHealthPointChanged OnHealthPointChanged;
	FOnHeal	OnHeal;

public:
	FORCEINLINE bool IsDead() { return Health <= 0.0f; }
	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE void SetMaxHealth(float InMaxHealth) { MaxHealth=InMaxHealth; }
	FORCEINLINE void InitHealth() { Health = MaxHealth; }
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void Damage(float InAmount);
	void Heal(float InAmount);


private:
	float Health;	//ÇöÀç HP	

};
