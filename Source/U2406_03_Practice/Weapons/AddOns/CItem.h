// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CItem.generated.h"

UENUM(BlueprintType)
enum class EITemType : uint8
{
	HEAL,
	MANA,
	MAX
};
UCLASS()
class U2406_03_PRACTICE_API ACItem : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EITemType Type;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class UCapsuleComponent* Capsule;
public:	
	// Sets default values for this actor's properties
	ACItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
		void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	UFUNCTION(BlueprintNativeEvent)
		void PlayEffect(class ACharacter* player);
	void PlayEffect_Implementation(class ACharacter* player) {};
};
