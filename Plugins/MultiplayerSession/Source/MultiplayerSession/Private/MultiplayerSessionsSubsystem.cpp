// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include <OnlineSubsystem.h>
#include <OnlineSessionSettings.h>
#include <Online/OnlineSessionNames.h>

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
		bCreateSessionOnDestroy = true;
		lastNumPublicConnection = NumpublicConnections;
		lastMatchType = MatchType;
		destroySession();
	}
	createSessionComplateDelegateHandle = sessionInterface->AddOnCreateSessionCompleteDelegate_Handle(createSessionComplateDelegate);
	lastSessionSettings = MakeShareable(new FOnlineSessionSettings());

	lastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	lastSessionSettings->NumPublicConnections = NumpublicConnections;
	lastSessionSettings->bAllowJoinInProgress = true;
	lastSessionSettings->bAllowJoinViaPresence = true;
	lastSessionSettings->bShouldAdvertise = true;
	lastSessionSettings->bUsesPresence = true;
	lastSessionSettings->bUseLobbiesIfAvailable = true;
	lastSessionSettings->BuildUniqueId = 1;
	lastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::Type::ViaOnlineServiceAndPing);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	if (!sessionInterface->CreateSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *lastSessionSettings))
	{
		sessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(createSessionComplateDelegateHandle);

		multiplayerOnCreateSessionComplate.Broadcast(false);
	}

}

void UMultiplayerSessionsSubsystem::findSession(int32 MaxSearchResult)
{
	if (!sessionInterface.IsValid())
	{
		return;
	}
	findSessionsComplateDelegateHandle = sessionInterface->AddOnFindSessionsCompleteDelegate_Handle(findSessionsComplateDelegate);

	lastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	lastSessionSearch->MaxSearchResults = MaxSearchResult;
	lastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	lastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->FindSessions(*localPlayer->GetPreferredUniqueNetId(), lastSessionSearch.ToSharedRef()))
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(findSessionsComplateDelegateHandle);
		multiplayerOnFindSessionsComplate.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
}

void UMultiplayerSessionsSubsystem::joinSession(const FOnlineSessionSearchResult& SessionResult)
{
	if (!sessionInterface.IsValid())
	{
		multiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}
	joinSessionComplateDelegateHandle = sessionInterface->AddOnJoinSessionCompleteDelegate_Handle(joinSessionComplateDelegate);

	const ULocalPlayer* localPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!sessionInterface->JoinSession(*localPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(joinSessionComplateDelegateHandle);
		multiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

void UMultiplayerSessionsSubsystem::destroySession()
{
	if (!sessionInterface.IsValid())
	{
		multiplayerOnDestroySessionComplete.Broadcast(false);
		return;
	}
	destroySessionComplateDelegateHandle = sessionInterface->AddOnDestroySessionCompleteDelegate_Handle(destroySessionComplateDelegate);
	if (!sessionInterface->DestroySession(NAME_GameSession))
	{
		sessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(destroySessionComplateDelegateHandle); 
		multiplayerOnDestroySessionComplete.Broadcast(false);
	}
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
	if (sessionInterface)
	{
		sessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(findSessionsComplateDelegateHandle);
	}
	if (lastSessionSearch->SearchResults.Num() <= 0)
	{
		multiplayerOnFindSessionsComplate.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}
	multiplayerOnFindSessionsComplate.Broadcast(lastSessionSearch->SearchResults, bWasSuccesful);

}

void UMultiplayerSessionsSubsystem::onJoinSessionComplate(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (sessionInterface)
	{
		sessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(joinSessionComplateDelegateHandle);
	}
	multiplayerOnJoinSessionComplete.Broadcast(Result);
}

void UMultiplayerSessionsSubsystem::onDestroySessionComplate(FName SessionName, bool bWasSuccesful)
{
	if (sessionInterface)
	{
		sessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(destroySessionComplateDelegateHandle);
	}
	if (bWasSuccesful && bCreateSessionOnDestroy)
	{
		bCreateSessionOnDestroy = false;
		createSession(lastNumPublicConnection,lastMatchType);
	}
	multiplayerOnDestroySessionComplete.Broadcast(bWasSuccesful);
}

void UMultiplayerSessionsSubsystem::onStartSessionComplate(FName SessionName, bool bWasSuccesful)
{

}
