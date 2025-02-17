// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/CEnvQueryContext_Target.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"


void UCEnvQueryContext_Target::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	//Owner:QueryContext를 실행시키는 대상

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(QueryInstance.Owner.Get());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);




	//ContextData,Items는 여러개(배열)/Querier(한개)
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, behavior->GetTarget());
}
