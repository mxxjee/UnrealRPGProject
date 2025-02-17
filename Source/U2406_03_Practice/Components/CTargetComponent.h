
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CTargetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class U2406_03_PRACTICE_API UCTargetComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Debug")
		TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;


	UPROPERTY(EditAnywhere, Category = "Settings")
		class UParticleSystem* ParticleAsset;

	UPROPERTY(EditAnywhere, Category = "Settings")
		float TraceDistance = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
		float RInterpTime = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
		float FinishAngle = 2.0f;
public:	
	UCTargetComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void Toggle();
	void MoveLeft();
	void MoveRight();


private:
	void Start();
	void End();

private:
	void Change(class ACharacter* InCandidate);
	void Move(bool bRight);

private:
	class ACharacter* GetNearlyFrontAngle(const TArray<FHitResult>& InHitResults);

private:
	class ACharacter* OwnerCharacter;	//컴포넌트의 오너
	class ACharacter* Target;		//타겟팅된 대상
	class UParticleSystemComponent* Particle;

private:
	bool bMovingFocus;
};
