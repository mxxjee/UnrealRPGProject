
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
	//Index��°��  ���ö����� ������ worldLocation���� �����´�.
	return Spline->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);	
}

void ACPatrolPath::UpdateIndex()
{
	//spline�� ��������
	int count = Spline->GetNumberOfSplinePoints();

	if (bReverse)
	{
		//��������ȣ���ƴ϶��,
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
	//������ ó��
	if (Index<count-1)
	{
		Index++;
		return;
	}

	//�������ε����� �����߰�, loopȰ��ȭ�Ǿ��ִٸ�.
	if (Spline->IsClosedLoop())
	{
		Index = 0;
		return;
	}

	//�������ε����� �����߰�, loop��Ȱ��ȭ���,

	Index = count - 2;
	bReverse = true;
}
