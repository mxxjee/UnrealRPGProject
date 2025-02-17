
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Melee.generated.h"

UCLASS()
class U2406_03_PRACTICE_API UCBTService_Melee : public UBTService
{
	GENERATED_BODY()
	
private:


	UPROPERTY(EditAnywhere, Category = "Range")
		float ApproachRange = 600;

	UPROPERTY(EditAnywhere, Category = "Range")
		float ReadyRange = 400;

	UPROPERTY(EditAnywhere, Category = "Action")
		float ActionRange = 150;
public:
	UCBTService_Melee();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
