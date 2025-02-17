
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"


UCLASS()
class U2406_03_PRACTICE_API ACAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
		class UAIPerceptionComponent* Perception;	//감지관련컴포넌트

public:
	ACAIController();

public:
	void SetSightRadius(float InSightRadius);
	void setUpdateTree(bool bInbool) { bCanUpdateTree = bInbool; }
protected:
	virtual void BeginPlay() override;

protected:
	virtual void OnPossess(APawn* InPawn) override;	//빙의 시작 시 호출
	virtual void OnUnPossess() override;		//빙의끝나면 호출

private:
	UFUNCTION()
		void OnPerceptionUpdate(const TArray<AActor*>& UpdatedActors);
private:
	class ACEnemy_AI* Enemy;
	class UCAIBehaviorComponent* Behavior;	//Enemy꺼 갖다씀
	
	class UAISenseConfig_Sight* Sight;

UPROPERTY(EditAnywhere)
	bool bCanUpdateTree=true;
};
