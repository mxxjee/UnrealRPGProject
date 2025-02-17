// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CParkourComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"


#include "Characters/CPlayer.h"

#include "Animation/AnimMontage.h"

#define LOG_UCParkourComponent

void FParkourData::PlayMontage(class ACharacter* InCharacter)
{
	if (bFixedCamera)
	{
		UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InCharacter);
		if (!!movement)
			movement->EnableFixedCamera();
	}
	InCharacter->PlayAnimMontage(Montage, PlayRate, SectionName);

}

//-----------------------------------------------------------

UCParkourComponent::UCParkourComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//CHelpers::GetAsset<UAnimMontage>(&TestData.Montage, "AnimMontage'/Game/Characters/Montages/Parkour/Run_Climb_Montage.Run_Climb_Montage'");
	CHelpers::GetAsset<UDataTable>(&DataTable, "DataTable'/Game/Parkour/DT_Parkour_shinbi.DT_Parkour_shinbi'");
}


// Called when the game starts
void UCParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	//데이터불러오기
	TArray<FParkourData*> rows;
	DataTable->GetAllRows<FParkourData>("", rows);

	for (int32 i = 0;i < (int32)EParkourType::Max;++i)
	{
		TArray<FParkourData> temp;
		for (FParkourData* data : rows)
		{
			if (data->Type == (EParkourType)i)
				temp.Add(*data);
		}

		DataMap.Add((EParkourType)i,temp);
	}


#ifdef LOG_UCParkourComponent
	/*for (TPair<EParkourType, TArray<FParkourData>> temp : DataMap)
	{
		CLog::Log(StaticEnum< EParkourType>()->GetNameStringByValue((uint8)temp.Key));

		for (FParkourData data : temp.Value)
		{
			CLog::Log(data.Montage->GetPathName());
		}
	}*/
#endif



	//오너캐릭터를 받아온다.
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	//오너캐릭터의 ArrowGroup중 자식컴포넌트들을 가져온다.
	USceneComponent* arrows = CHelpers::GetComponent<USceneComponent>(OwnerCharacter, "ArrowGroup");

	TArray<USceneComponent*> components;
	arrows->GetChildrenComponents(false,components);

	for (int32 i = 0;i < (int32)EParkourArrowType::Max;++i)
	{
		//CLog::Log(components[i]->GetName());
		Arrows[i] = Cast<UArrowComponent>(components[i]);
	}
	
}


// Called every frame
void UCParkourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);

	if (!!player)
	{
		CheckTrue(CHelpers::GetComponent<UCStateComponent>(player)->IsDamagedMode());
		CheckFalse(CHelpers::GetComponent<UCWeaponComponent>(player)->IsUnarmedMode());
	}
	CheckTrace_Land();
}

void UCParkourComponent::DoParkour(bool bLanded)
{
	//파쿠르상태가 아닐때만 실행

	CheckFalse(Type == EParkourType::Max);

	if (bLanded && Check_FallMode())
	{
		DoParkour_Fall();
		return;
	}

	HitObstacle = nullptr;
	HitObstacleExtent = FVector::ZeroVector;
	HitDistance = 0.0f;
	ToFrontYaw = 0.0f;

	CheckTrace_Center();

	//Center가 감지되었으면 
	if (!!HitObstacle)
	{
		//머리쪽에 닿는게있는지 감지.
		CheckTrace_Head();
		CheckTrace_Foot();
		CheckTrace_Side();
	}

	//파쿠르 장애물인지 체크. 아니라면 밑에줄을 시행안하겟다
	CheckFalse(Check_Obstacle());

	if (Check_ClimbMode())
	{
		DoParkour_Climb();
		return;
	}

	//슬라이드체크
	if (Check_SlideMode())
	{
		DoParkour_Slide();
		return;
	}

	FParkourData data;
	if (Check_ObstacleMode(EParkourType::Normal, data))
	{
		DoParkour_Obstacle(EParkourType::Normal, data);
		return;
	}
	if (Check_ObstacleMode(EParkourType::Short, data))
	{
		DoParkour_Obstacle(EParkourType::Short, data);
		return;
	}

	if (Check_ObstacleMode(EParkourType::Wall, data))
	{
		DoParkour_Obstacle(EParkourType::Wall, data);
		return;
	}


}

void UCParkourComponent::CheckTrace_Center()
{
	EParkourArrowType type = EParkourArrowType::Center;
	//검출함수
	LineTrace(type);


	//계속 생성/삭제를 하지않도록 &붙여주기.
	const FHitResult& hitResult = HitResults[(int32)type];

	//검출되었으면 bBlockingHit==true , 우리는 parkour를 block으로 반응하도록했으므로.
	CheckFalse(hitResult.bBlockingHit);

	//충돌한액터의 staticmesh컴포넌트가져오기
	UStaticMeshComponent* mesh = CHelpers::GetComponent<UStaticMeshComponent>(hitResult.GetActor());
	CheckNull(mesh);

	//hit장애물저장
	HitObstacle = hitResult.GetActor();

	//Bounding:충돌체, 크기근사치 : 부피

	FVector minBound, maxBound;
	mesh->GetLocalBounds(minBound, maxBound);

	float x = FMath::Abs(minBound.X - maxBound.X);
	float y = FMath::Abs(minBound.Y - maxBound.Y);
	float z = FMath::Abs(minBound.Z - maxBound.Z);

	HitObstacleExtent = FVector(x, y, z);

	HitDistance = hitResult.Distance;

	ToFrontYaw = UKismetMathLibrary::MakeRotFromX(-hitResult.ImpactNormal).Yaw;


#ifdef LOG_UCParkourComponent
	//CLog::Print("HitObstacle : " + HitObstacle->GetName(), 1100);
	CLog::Print("HitObstacleExtent : " + HitObstacleExtent.ToString(), 1101,5,FColor::Green);
	CLog::Print("HitDistance : " + FString::SanitizeFloat(HitDistance), 1102);
	//CLog::Print("ToFrontYaw : " + FString::SanitizeFloat(ToFrontYaw), 1103);
#endif // !LOG_UCParkourComponent

	
}

void UCParkourComponent::LineTrace(EParkourArrowType InType)
{
	UArrowComponent* arrow = Arrows[(int32)InType];
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	//컴포넌트의 월드좌표를 가져온다(LineTrace사용하기위해)
	FTransform transform = arrow->GetComponentToWorld();

	FVector start = transform.GetLocation();
	FVector end = start + OwnerCharacter->GetActorForwardVector() * TraceDistance;

	//Ignore리스트
	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);	//자기자신플레이어 제외


	UKismetSystemLibrary::LineTraceSingle(GetWorld(),
		start,
		end,
		ETraceTypeQuery::TraceTypeQuery3,//우리가 새로생성한 트레이스채널
		false,
		ignores,
		DebugType,
		HitResults[(int32)InType],		//배열의 순서와 InTpye의 순서는같다.
		true,
		color,
		FLinearColor::White
	);

	CheckNull(HitResults[(int32)InType].GetActor());
	//CLog::Print(HitResults[(int32)InType].GetActor()->GetName());

}

void UCParkourComponent::CheckTrace_Head()
{
	LineTrace(EParkourArrowType::Head);
}

void UCParkourComponent::CheckTrace_Foot()
{
	LineTrace(EParkourArrowType::Foot);
}

void UCParkourComponent::CheckTrace_Side()
{
	LineTrace(EParkourArrowType::Left);
	LineTrace(EParkourArrowType::Right);
}

bool UCParkourComponent::Check_Obstacle()
{
	//hitobstacle이있는지 체크한다.
	CheckNullResult(HitObstacle, false);

	//각 arrowtype에서 쏴서 충돌되었는지
	bool b = true;
	b &= HitResults[(int32)EParkourArrowType::Center].bBlockingHit;
	b &= HitResults[(int32)EParkourArrowType::Left].bBlockingHit;
	b &= HitResults[(int32)EParkourArrowType::Right].bBlockingHit;

	CheckFalseResult(b, false);

	//모든 normal을 저장한다.
	FVector center = HitResults[(int32)EParkourArrowType::Center].ImpactNormal;
	FVector left = HitResults[(int32)EParkourArrowType::Left].ImpactNormal;
	FVector right = HitResults[(int32)EParkourArrowType::Right].ImpactNormal;

	CheckFalseResult(center.Equals(left),false);
	CheckFalseResult(center.Equals(right), false);

	//Center  LineTrace의 충돌지점
	FVector start = HitResults[(int32)EParkourArrowType::Center].ImpactPoint;
	FVector end = OwnerCharacter->GetActorLocation();

	float lookAt = UKismetMathLibrary::FindLookAtRotation(start, end).Yaw;

	//충돌한 벽의 법선벡터
	FVector impactNormal = HitResults[(int32)EParkourArrowType::Center].ImpactNormal;

	//법선벡터의 전방값 중 수평값
	float impactAt = UKismetMathLibrary::MakeRotFromX(impactNormal).Yaw;

	float yaw = FMath::Abs(FMath::Abs(lookAt) - FMath::Abs(impactAt));
	CheckFalseResult(yaw <= AvailableFrontAngle, false);



#ifdef LOG_UCParkourComponent
	//CLog::Print("ImpactPoint:" + start.ToString(), 1109);
	//CLog::Print("lookAt : " + FString::SanitizeFloat(lookAt), 1110);
	//CLog::Print("impactNormal : " + impactNormal.ToString(), 1111);
	//CLog::Print("impactAt : " + FString::SanitizeFloat(impactAt), 1112);

#endif // LOG_UCParkourComponent

	return true;
}

bool UCParkourComponent::Check_ClimbMode()
{
	//Head가 닿아야 올라갈수있음.
	CheckFalseResult(HitResults[(int32)EParkourArrowType::Head].bBlockingHit, false);

	
	CheckFalseResult((*DataMap.Find(EParkourType::Climb))[0].MinDistance< HitDistance, false);
	CheckFalseResult((*DataMap.Find(EParkourType::Climb))[0].MaxDistance > HitDistance, false);
	//CheckFalseResult(TestData.MinDistance < HitDistance, false);
	//CheckFalseResult(TestData.MaxDistance > HitDistance, false);

	//CheckFalseResult(FMath::IsNearlyEqual(TestData.Extent, HitObstacleExtent.Z, 10),false);
	return true;
}

void UCParkourComponent::DoParkour_Climb()
{
	Type = EParkourType::Climb;

	//위치설정
	OwnerCharacter->SetActorLocation(HitResults[(int32)EParkourArrowType::Center].ImpactPoint );
	OwnerCharacter->SetActorRotation(FRotator(0,ToFrontYaw,0));

	(*DataMap.Find(EParkourType::Climb))[0].PlayMontage(OwnerCharacter);
	//TestData.PlayMontage(OwnerCharacter);

	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

}

void UCParkourComponent::End_DoParkour()
{
	switch (Type)
	{
		case EParkourType::Climb:
			End_DoParkour_Climb();
			break;

		case EParkourType::Fall:
			End_DoParkour_Fall();
			break;
		case EParkourType::Slide:
			End_DoParkour_Slide();
			break;

		case EParkourType::Normal:
		case EParkourType::Wall:
		case EParkourType::Short:
			End_DoParkour_Obstacle();
			break;
	}
	Type = EParkourType::Max;

	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
	if (!!movement)
	{
		if(movement->GetFixedCamera()==true)
			movement->DisableFixedCamera();
	}
		

}

void UCParkourComponent::End_DoParkour_Climb()
{
	FVector OwnerPosition = OwnerCharacter->GetActorLocation();
	OwnerCharacter->SetActorLocation(OwnerPosition+FVector(10, 0, 0)) ;
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UCParkourComponent::CheckTrace_Land()
{
	
	CheckFalse(CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter)->IsUnarmedMode());
	CheckFalse(OwnerCharacter->GetMovementComponent()->IsFalling());
	CheckTrue(bFirstFalling);

	bFirstFalling = true;


	//추락이 시작되는순간 한번만 밑으로 LineTrace
	UArrowComponent* arrow = Arrows[(int32)EParkourArrowType::Land];
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	//컴포넌트의 월드좌표를 가져온다(LineTrace사용하기위해)
	FTransform transform = arrow->GetComponentToWorld();

	FVector start = transform.GetLocation();
	FVector end = start + transform.GetRotation().GetForwardVector()*(*DataMap.Find(EParkourType::Fall))[0].Extent;

	//Ignore리스트
	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);	//자기자신플레이어 제외


	UKismetSystemLibrary::LineTraceSingle(GetWorld(),
		start,
		end,
		ETraceTypeQuery::TraceTypeQuery1,//기본추적이 block - visibility
		false,
		ignores,
		DebugType,
		HitResults[(int32)EParkourArrowType::Land],		//배열의 순서와 InTpye의 순서는같다.
		true,
		color,
		FLinearColor::White
	);

	
}

bool UCParkourComponent::Check_FallMode()

{
	CheckFalseResult(bFirstFalling,false);

	bFirstFalling = false;

	

	CheckFalseResult(HitResults[(int32)EParkourArrowType::Land].bBlockingHit, false);

	TArray<FParkourData>* data = DataMap.Find(EParkourType::Fall);
	float distance = HitResults[(int32)EParkourArrowType::Land].Distance;

	CheckFalseResult((*data)[0].MinDistance< distance, false);
	CheckFalseResult((*data)[0].MaxDistance > distance, false);


	return true;
}

void UCParkourComponent::DoParkour_Fall()
{
	Type = EParkourType::Fall;

	(*DataMap.Find(EParkourType::Fall))[0].PlayMontage(OwnerCharacter);
}

void UCParkourComponent::End_DoParkour_Fall()
{

}

bool UCParkourComponent::Check_SlideMode()
{
	//발이 닿으면안되므로 
	CheckTrueResult(HitResults[(int32)EParkourArrowType::Foot].bBlockingHit, false);

	const TArray<FParkourData>* datas = DataMap.Find(EParkourType::Slide);


	CheckFalseResult((*datas)[0].MinDistance < HitDistance, false);
	CheckFalseResult((*datas)[0].MaxDistance > HitDistance, false);

	//밑공간 추적
	UArrowComponent* arrow = Arrows[(int32)EParkourArrowType::Foot];
	FLinearColor color = FLinearColor(arrow->ArrowColor);

	FTransform transform = arrow->GetComponentToWorld();

	FVector arrowLocation = transform.GetLocation();
	FVector ownerLocation = OwnerCharacter->GetActorLocation();

	//Extent- sliding 가능한 높이인지.
	float extent = (*datas)[0].Extent;	//30
	float z = arrowLocation.Z + extent;		//수직으로 30정도높음

	FVector forward = OwnerCharacter->GetActorForwardVector();
	forward = FVector(forward.X, forward.Y, 0);		//수직높이는 필요없고 x,y위치만 필요.

	FVector start = FVector(arrowLocation.X, arrowLocation.Y, z);
	FVector end = start + forward * TraceDistance;		//Onwercharacter의 전방 x,y 방향으로 Distance만큼

	TArray<AActor*> ignores;
	//ignores.Add(OwnCharacter);
	FHitResult hitResult;

	//박스에 닿는게 없어야 슬라이딩 가능.								
	UKismetSystemLibrary::BoxTraceSingle(GetWorld(), start, end,
		FVector(0, extent, extent), //부피  x=0이므로 앞,뒤부피가없다.		
		OwnerCharacter->GetActorRotation(),//방향
		ETraceTypeQuery::TraceTypeQuery1, false, ignores, DebugType, hitResult, true, color, FLinearColor::White);

	CheckTrueResult(hitResult.bBlockingHit, false);



	return true;
}

void UCParkourComponent::DoParkour_Slide()
{
	Type = EParkourType::Slide;

	//위치설정
	OwnerCharacter->SetActorRotation(FRotator(0, ToFrontYaw, 0));

	(*DataMap.Find(EParkourType::Slide))[0].PlayMontage(OwnerCharacter);

	BackupObstacle = HitObstacle;
	BackupObstacle->SetActorEnableCollision(false);

}

void UCParkourComponent::End_DoParkour_Slide()
{
	BackupObstacle->SetActorEnableCollision(true);
	BackupObstacle = nullptr;
}

bool UCParkourComponent::Check_ObstacleMode(EParkourType InType, FParkourData& OutData)
{
	//뛰어넘을때 할 체크
	//뛰어 넘어야하는것이므로 Head가 닿으면안됨.
	CheckTrueResult(HitResults[(int32)EParkourArrowType::Head].bBlockingHit, false);


	const TArray<FParkourData>* datas = DataMap.Find(InType);

	for (int32 i = 0;i < (*datas).Num();i++)
	{
		bool b = true;
		b &= (*datas)[i].MinDistance < HitDistance;
		b &= (*datas)[i].MaxDistance > HitDistance;
		b &= FMath::IsNearlyEqual((*datas)[i].Extent, HitObstacleExtent.Y, 10);

		OutData = (*datas)[i];
		CheckTrueResult(b, true);
	}


	return false;
}

void UCParkourComponent::DoParkour_Obstacle(EParkourType InType, FParkourData& InData)
{
	Type = InType;



	OwnerCharacter->SetActorRotation(FRotator(0, ToFrontYaw, 0));
	InData.PlayMontage(OwnerCharacter);

	BackupObstacle = HitObstacle;
	BackupObstacle->SetActorEnableCollision(false);


}

void UCParkourComponent::End_DoParkour_Obstacle()
{
	BackupObstacle->SetActorEnableCollision(true);
	BackupObstacle = nullptr;
}