
#include "BehaviorTree/CPatrolPath.h"
#include "Global.h"
#include "Components/SplineComponent.h"
#include "Components/TextRenderComponent.h"

ACPatrolPath::ACPatrolPath()
{
	
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USplineComponent>(this, &Spline, "Spline", Root);
	CHelpers::CreateComponent<UTextRenderComponent>(this, &Text, "Text", Root);

	Spline->SetRelativeLocation(FVector(0, 0, 30));

	Text->SetRelativeLocation(FVector(0, 0, 120));
	Text->SetRelativeRotation(FRotator(0, 180, 0));
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	Text->TextRenderColor = FColor::Red;
	Text->bHiddenInGame = true;
}

void ACPatrolPath::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR
	Text->Text = FText::FromString(GetActorLabel());
#endif

	Spline->SetClosedLoop(bLoop);
}


void ACPatrolPath::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ACPatrolPath::GetMoveTo()
{
	//Index번째의  스플라인의 지점을 worldLocation으로 가져온다.
	return Spline->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);	
}

void ACPatrolPath::UpdateIndex()
{
	//spline의 정점개수
	int count = Spline->GetNumberOfSplinePoints();

	if (bReverse)
	{
		//마지막번호가아니라면,
		if (Index > 0)
		{
			Index--;
			return;
		}

		if (Spline->IsClosedLoop())
		{
			Index = count - 1;
			return;
		}

		Index = 1;
		bReverse = false;
	}
	//정방향 처리
	if (Index<count-1)
	{
		Index++;
		return;
	}

	//마지막인덱스에 도달했고, loop활성화되어있다면.
	if (Spline->IsClosedLoop())
	{
		Index = 0;
		return;
	}

	//마지막인덱스에 도달했고, loop비활성화라면,

	Index = count - 2;
	bReverse = true;
}
