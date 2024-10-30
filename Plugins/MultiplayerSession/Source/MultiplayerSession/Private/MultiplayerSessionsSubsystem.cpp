// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include <OnlineSubsystem.h>

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
	//OnlineSuubsystem pointer'� olu�turduk
	IOnlineSubsystem* subsystem = IOnlineSubsystem::Get();
	if (subsystem)
	{
		//olu�turdu�umuz pointer'� sessionInterface'imize atad�k
		sessionInterface = subsystem->GetSessionInterface();
	}
}
