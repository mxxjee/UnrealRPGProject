// Copyright Epic Games, Inc. All Rights Reserved.


#include "CGameMode.h"
#include "Global.h"

ACGameMode::ACGameMode()
{
	//CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/Player/BP_CPlayer.BP_CPlayer_C'");
	CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/Player/BP_CPlayer_Shinbi.BP_CPlayer_Shinbi_C'");
}
