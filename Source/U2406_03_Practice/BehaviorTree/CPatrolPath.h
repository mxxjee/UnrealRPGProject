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
		bool bLoop;	//Loop체크시 0-1-2-3-4-0 , 체크X : 0-1-2-3-4-3-2-1

	UPROPERTY(EditAnywhere, Category = "Path")
		int Index;	//갈 wayPoint의 index

	UPROPERTY(EditAnywhere, Category = "Path")
		bool bReverse;	//Loop체크,bReverse체크 시 뒤로 다시 백,  Loop체크X시 -> 돌아갈때 켜줄것임.


private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
		class USplineComponent* Spline;	//곡선을 그려주는 컴포넌트

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
