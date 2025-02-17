
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
		class UAIPerceptionComponent* Perception;	//��������������Ʈ

public:
	ACAIController();

public:
	void SetSightRadius(float InSightRadius);
	void setUpdateTree(bool bInbool) { bCanUpdateTree = bInbool; }
protected:
	virtual void BeginPlay() override;

protected:
	virtual void OnPossess(APawn* InPawn) override;	//���� ���� �� ȣ��
	virtual void OnUnPossess() override;		//���ǳ����� ȣ��

private:
	UFUNCTION()
		void OnPerceptionUpdate(const TArray<AActor*>& UpdatedActors);
private:
	class ACEnemy_AI* Enemy;
	class UCAIBehaviorComponent* Behavior;	//Enemy�� ���پ�
	
	class UAISenseConfig_Sight* Sight;

UPROPERTY(EditAnywhere)
	bool bCanUpdateTree=true;
};
