#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPatrolPath.generated.h"

UCLASS()
class U2406_03_PRACTICE_API ACPatrolPath : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Path")
		bool bLoop;	//Loopüũ�� 0-1-2-3-4-0 , üũX : 0-1-2-3-4-3-2-1

	UPROPERTY(EditAnywhere, Category = "Path")
		int Index;	//�� wayPoint�� index

	UPROPERTY(EditAnywhere, Category = "Path")
		bool bReverse;	//Loopüũ,bReverseüũ �� �ڷ� �ٽ� ��,  LoopüũX�� -> ���ư��� ���ٰ���.


private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
		class USplineComponent* Spline;	//��� �׷��ִ� ������Ʈ

	UPROPERTY(VisibleDefaultsOnly)
		class UTextRenderComponent* Text;
public:	
	ACPatrolPath();


protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform)override;

public:
	FVector GetMoveTo();
	void UpdateIndex();

};
