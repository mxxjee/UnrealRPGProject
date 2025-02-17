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
	Center=0,	//�÷��̾��߾ӿ���  ��� arrow
	Head,//�Ӹ������� ���� arrow
	Foot,//���ʿ��� ���� arrow
	Left, //�÷��̾���ʿ��� �� ���� arrow
	Right,//�÷��̾� �����ʿ��� ���� arrow
	Land,//�÷��̾� ������ �Ǵ��� arrow
	Max
};

UENUM(BlueprintType)
//���� Ÿ������
enum class EParkourType : uint8
{
	Climb = 0,	//������ �ִϸ��̼�
	Fall,//�������鼭 ������
	Slide, //�����̵�
	Short, //ª�� ����(�Ÿ��� ���� �ִϸ��̼ǽ���)
	Normal, //��������
	Wall, //��
	Max

};

USTRUCT(BlueprintType)
struct FParkourData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		EParkourType Type;	//����Ÿ��

	UPROPERTY(EditAnywhere)
		UAnimMontage* Montage; //�÷����� ��Ÿ��

	UPROPERTY(EditAnywhere)
		float PlayRate = 1;//�÷��̽ð�

	UPROPERTY(EditAnywhere)
		FName SectionName;

	UPROPERTY(EditAnywhere)
		float MinDistance; //������ ������ �� �ִ� �ּҰŸ�

	UPROPERTY(EditAnywhere)
		float MaxDistance; //������ ������ �� �ִ� �ִ�Ÿ�

	UPROPERTY(EditAnywhere)
		float Extent; //��Ȳ������ �ٸ��Ծ���, 

	UPROPERTY(EditAnywhere)
		bool bFixedCamera;	//ī�޶�ȸ����뿩��

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
	void End_DoParkour();	//��Ƽ����ȣ��
private:
	void CheckTrace_Center();

	//�������� ���� üũƮ���̽�
	void CheckTrace_Head();
	void CheckTrace_Foot();
	void CheckTrace_Side();

	void CheckTrace_Land(); // �������Ӹ��� �پ���� ������

private:
	bool Check_Obstacle();//���� ��ֹ��̸´��� üũ
	bool Check_ClimbMode();//Ŭ���Ӹ������ üũ
	void DoParkour_Climb();
	void End_DoParkour_Climb();

	bool Check_FallMode(); //������ �Ÿ��� �Ÿ����ѿ� �´��� üũ
	void DoParkour_Fall();
	void End_DoParkour_Fall();

	//�����̵�
	bool Check_SlideMode();
	void DoParkour_Slide();
	void End_DoParkour_Slide();

	//Ÿ��,�Ÿ����� 
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
	//ȭ��ǥ���� ���� Hitresult��������.
	FHitResult HitResults[(int32)EParkourArrowType::Max];
private:
	//���� ����Ÿ��  (�⺻Ÿ�� : Max)
	EParkourType Type = EParkourType::Max;

private:
	//������ ������Ʈ���� ����
	AActor* HitObstacle; //Hit actor
	FVector HitObstacleExtent;// ������ ����
	float HitDistance; //�浹�� ���Ϳ��� �Ÿ�
	float ToFrontYaw;

private:
	bool bFirstFalling;	//���� 1ȸ��üũ

private:
	class AActor* BackupObstacle;
};
