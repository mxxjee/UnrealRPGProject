
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/CEnemy_AI.h"
#include "CUserWidget_Enemy.generated.h"

UCLASS()
class U2406_03_PRACTICE_API UCUserWidget_Enemy : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateHealth(float InHealth, float InMaxHealth);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateName(const FString& InName);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateControllerName(const FString& InName);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateLabelRotation(class UWidgetComponent* InLabelComponent,class ACEnemy_AI* InOwner);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateHealthColor_OnePhase();

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateHealthColor_TwoPhase();

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateHealthColor_ThreePhase();
};
