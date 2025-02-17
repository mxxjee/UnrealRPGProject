// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/DataTable.h"
#include "CParkourComponent.generated.h"

UENUM(BlueprintType)
enum class EParkourArrowType : uint8
{
	Center=0,	//플레이어중앙에서  쏘는 arrow
	Head,//머리쪽으로 나갈 arrow
	Foot,//발쪽에서 나갈 arrow
	Left, //플레이어왼쪽에서 ㄴ ㅏ갈 arrow
	Right,//플레이어 오른쪽에서 나갈 arrow
	Land,//플레이어 착지때 판단할 arrow
	Max
};

UENUM(BlueprintType)
//파쿠르 타입정리
enum class EParkourType : uint8
{
	Climb = 0,	//오르는 애니메이션
	Fall,//떨어지면서 구를때
	Slide, //슬라이딩
	Short, //짧은 점프(거리에 따라 애니메이션실행)
	Normal, //보통점프
	Wall, //벽
	Max

};

USTRUCT(BlueprintType)
struct FParkourData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EParkourType Type;	//파쿠르타입

	UPROPERTY(EditAnywhere)
		UAnimMontage* Montage; //플레이할 몽타주

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;//플레이시간

	UPROPERTY(EditAnywhere)
		FName SectionName;

	UPROPERTY(EditAnywhere)
		float MinDistance; //파쿠르를 실행할 수 있는 최소거리

	UPROPERTY(EditAnywhere)
		float MaxDistance; //파쿠르를 실행할 수 있는 최대거리

	UPROPERTY(EditAnywhere)
		float Extent; //상황에따라 다르게쓰임, 

	UPROPERTY(EditAnywhere)
		bool bFixedCamera;	//카메라회전허용여부

public:
	void PlayMontage(class ACharacter* InCharacter);

};

UCLASS()
class U2406_03_PRACTICE_API UCParkourComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Trace")
		float TraceDistance = 600;

	UPROPERTY(EditAnywhere, Category = "Trace")
		TEnumAsByte<EDrawDebugTrace::Type> DebugType;

	UPROPERTY(EditAnywhere, Category = "Trace")
		float AvailableFrontAngle=15;

	/*UPROPERTY(EditAnywhere, Category = "Temp")
		FParkourData TestData;*/

	UPROPERTY(EditAnywhere, Category = "Data")
		UDataTable* DataTable;
public:	
	// Sets default values for this component's properties
	UCParkourComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



public:
	void DoParkour(bool bLanded=false);
	void End_DoParkour();	//노티파이호출
private:
	void CheckTrace_Center();

	//각부위에 따른 체크트레이스
	void CheckTrace_Head();
	void CheckTrace_Foot();
	void CheckTrace_Side();

	void CheckTrace_Land(); // 매프레임마다 뛰어내리는 중인지

private:
	bool Check_Obstacle();//파쿠르 장애물이맞는지 체크
	bool Check_ClimbMode();//클라임모드인지 체크
	void DoParkour_Climb();
	void End_DoParkour_Climb();

	bool Check_FallMode(); //떨어진 거리가 거리제한에 맞는지 체크
	void DoParkour_Fall();
	void End_DoParkour_Fall();

	//슬라이딩
	bool Check_SlideMode();
	void DoParkour_Slide();
	void End_DoParkour_Slide();

	//타입,거리별로 
	bool Check_ObstacleMode(EParkourType InType, FParkourData& OutData);
	void DoParkour_Obstacle(EParkourType InType, FParkourData& InData);
	void End_DoParkour_Obstacle();
private:
	TMap<EParkourType, TArray<FParkourData>> DataMap;
private:
	void LineTrace(EParkourArrowType InType);

private:
	class ACharacter* OwnerCharacter;
	class UArrowComponent* Arrows[(int32)EParkourArrowType::Max];
	

public:
	EParkourType GetParkourType() { return Type; }
private:
	//화살표마다 각각 Hitresult를가진다.
	FHitResult HitResults[(int32)EParkourArrowType::Max];
private:
	//현재 파쿠르타입  (기본타입 : Max)
	EParkourType Type = EParkourType::Max;

private:
	//감지한 오브젝트관련 정보
	AActor* HitObstacle; //Hit actor
	FVector HitObstacleExtent;// 액터의 부피
	float HitDistance; //충돌한 액터와의 거리
	float ToFrontYaw;

private:
	bool bFirstFalling;	//최초 1회만체크

private:
	class AActor* BackupObstacle;
};
