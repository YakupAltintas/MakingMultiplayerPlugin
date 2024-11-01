// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include <OnlineSubsystem.h>
#include <OnlineSessionSettings.h>


UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem() :
	createSessionComplateDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::onCreateSessionComplate)),
	findSessionsComplateDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::onFindSessionsComplate)),
	joinSessionComplateDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::onJoinSessionComplate)),
	destroySessionComplateDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::onDestroySessionComplate)),
	startSessionComplateDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::onStartSessionComplate))
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
	if (!sessionInterface.IsValid())
	{
		return;
	}
	auto existingSession = sessionInterface->GetNamedSession(NAME_GameSession);
	if (existingSession != nullptr)
	{
		sessionInterface->DestroySession(NAME_GameSession);
	}
	createSessionComplateDelegateHandle = sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(createSessionComplateDelegate);
	lastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	lastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false ;
	lastSessionSettings->NumPublicConnections = NumpublicConnections;
	lastSessionSettings->bAllowJoinInProgress = true;
	lastSessionSettings->bAllowJoinViaPresence = true;
	lastSessionSettings->bShouldAdvertise= true;
	lastSessionSettings->bUsesPresence = true;
	lastSessionSettings->Set(FName("MatchType"),MatchType,EOnlineDataAdvertisementType::Type::ViaOnlineServiceAndPing);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	
	if (!sessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *lastSessionSettings))
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(createSessionComplateDelegateHandle);

		multiplayerOnCreateSessionComplate.Broadcast(false);
	}

}

void UMultiplayerSessionsSubsystem::findSession(int32 MaxSearchResult)
{

}

void UMultiplayerSessionsSubsystem::joinSession(const FOnlineSessionSearchResult& SessionResult)
{

}

void UMultiplayerSessionsSubsystem::destroySession()
{

}

void UMultiplayerSessionsSubsystem::startSession()
{

}

void UMultiplayerSessionsSubsystem::onCreateSessionComplate(FName SessionName, bool bWasSuccesful)
{
	if (sessionInterface)
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(createSessionComplateDelegateHandle);
	}

	multiplayerOnCreateSessionComplate.Broadcast(bWasSuccesful);
}

void UMultiplayerSessionsSubsystem::onFindSessionsComplate(bool bWasSuccesful)
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
