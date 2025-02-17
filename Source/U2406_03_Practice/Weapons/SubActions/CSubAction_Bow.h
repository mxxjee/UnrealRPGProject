// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Components/TimelineComponent.h"
#include "CSubAction_Bow.generated.h"

USTRUCT()
struct FAimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float TargetArmLength = 120;

	UPROPERTY(EditAnywhere)
		FVector SocketOffset = FVector(0, 50, 30);

	UPROPERTY(EditAnywhere)
		bool bEnableCameraLag;

	UPROPERTY(EditAnywhere)
		FVector CameraLocation;

};

UCLASS(Blueprintable)
class U2406_03_PRACTICE_API UCSubAction_Bow : public UCSubAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
		class APostProcessVolume* PostProcessVolume;
private:
	UPROPERTY(EditAnywhere, Category = "Aiming")
		class UCurveVector* Curve;
	UPROPERTY(EditAnywhere, Category = "Aiming")
		FAimData AimData;

	UPROPERTY(EditAnywhere, Category = "Aiming")
		float PlayRate = 200;
public:
	UCSubAction_Bow();

public:
	void BeginPlay(class ACharacter* InOwner, class ACAttachment* ISnAttachment, class UCDoAction* InDoAction) override;
	void Tick(float DeltaTime)override;
public:
	void Pressed() override;
	void Released() override;


private:
	UFUNCTION()
		void OnAiming(FVector Output);

protected:
	UFUNCTION(BlueprintNativeEvent)
		void OnVignetting(APostProcessVolume* InPostProcess, float Z);
	void OnVignetting_Implementation(APostProcessVolume* InPostProcess, float Z) {};
private:
	FAimData OriginData;

private:
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

private:
	FTimeline Timeline;

private:
	float* Bending;
	class ACPlayer* player;
	TArray <TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
};
