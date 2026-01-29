// Copyright Epic Games, Inc. All Rights Reserved.

#include "IroncladGameMode.h"

AIroncladGameMode::AIroncladGameMode()
{
	// stub
}

void AIroncladGameMode::RequestRespawn(AController* Controller, float DelaySeconds)
{
    if (!Controller) return;

    FTimerHandle Handle;
    GetWorldTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([this, Controller]()
        {
            RestartPlayer(Controller);
        }), DelaySeconds, false);
}