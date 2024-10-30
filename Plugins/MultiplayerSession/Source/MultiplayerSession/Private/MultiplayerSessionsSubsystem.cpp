// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include <OnlineSubsystem.h>

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
	//OnlineSuubsystem pointer'ý oluþturduk
	IOnlineSubsystem* subsystem = IOnlineSubsystem::Get();
	if (subsystem)
	{
		//oluþturduðumuz pointer'ý sessionInterface'imize atadýk
		sessionInterface = subsystem->GetSessionInterface();
	}
}
