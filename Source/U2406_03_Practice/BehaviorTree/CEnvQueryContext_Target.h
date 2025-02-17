// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "CEnvQueryContext_Target.generated.h"

/**
 * 
 */
UCLASS()
class U2406_03_PRACTICE_API UCEnvQueryContext_Target : public UEnvQueryContext
{
	GENERATED_BODY()
	
private:
	void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
