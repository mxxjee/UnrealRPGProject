// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "CHorse.generated.h"

UCLASS()
class U2406_03_PRACTICE_API ACHorse : 
	public ACharacter,
	public IGenericTeamAgentInterface
{
	GENERATED_BODY()

private:
	uint8 TeamID = 3;

	UPROPERTY(EditAnywhere, Category = "Effect")
		TSubclassOf<class AActor> DisMountEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
		class USoundWave* DisMountSound;

	UPROPERTY(EditAnywhere, Category = "Effect")
		FVector LocationOffSet;


	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		class UCMovementComponent* Movement;

private:
	UFUNCTION(BlueprintCallable)
		void DisMount();
public:

	ACHorse();

protected:
	
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE void SetOwnerCharacter(class ACPlayer* InPlayer) { OwnerCharacter = InPlayer; }
	FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(TeamID); }
public:
	void OnSprint();
	void OffSprint();
private:
	UPROPERTY(EditAnywhere)
		class ACPlayer* OwnerCharacter;
};
