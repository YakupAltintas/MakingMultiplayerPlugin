// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include <OnlineSubsystem.h>

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem():
createSessionComplateDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this,&ThisClass::onCreateSessionComplate)),
findSessionsComplateDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this,&ThisClass::onfindSessionsComplate)),
joinSessionComplateDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this,&ThisClass::onJoinSessionComplate)),
destroySessionComplateDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this,&ThisClass::onDestroySessionComplate)),
startSessionComplateDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this,&ThisClass::onStartSessionComplate))
{
	//OnlineSuubsystem pointer'ý oluþturduk
	IOnlineSubsystem* subsystem = IOnlineSubsystem::Get();
	if (subsystem)
	{
		//oluþturduðumuz pointer'ý sessionInterface'imize atadýk
		sessionInterface = subsystem->GetSessionInterface();
	}
}

void UMultiplayerSessionsSubsystem::createSession(int32 NumpublicConnections, FString MatchType)
{
}

void UMultiplayerSessionsSubsystem::findSession(int32 MaxSearchResult)
{
}

void UMultiplayerSessionsSubsystem::joinSession(const FOnlineSessionSearchResult& SessionResult)
{
}

void UMultiplayerSessionsSubsystem::DestroySession()
{
}

void UMultiplayerSessionsSubsystem::StartSession()
{
}

void UMultiplayerSessionsSubsystem::onCreateSessionComplate(FName SessionName, bool bWasSuccesful)
{
}

void UMultiplayerSessionsSubsystem::onfindSessionsComplate(bool bWasSuccesful)
{
}

void UMultiplayerSessionsSubsystem::onJoinSessionComplate(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
}

void UMultiplayerSessionsSubsystem::onDestroySessionComplate(FName SessionName, bool bWasSuccesful)
{
}

void UMultiplayerSessionsSubsystem::onStartSessionComplate(FName SessionName, bool bWasSuccesful)
{
}
