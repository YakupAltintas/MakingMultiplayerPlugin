// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionsSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplate,bool,bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionsComplate,const TArray<FOnlineSessionSearchResult>& sessionResult,bool bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSuccessful);

UCLASS()
class MULTIPLAYERSESSION_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UMultiplayerSessionsSubsystem();//yapýcý methodun oluþturduk

	void createSession(int32 NumpublicConnections, FString MatchType);
	void findSession(int32 MaxSearchResult);
	void joinSession(const FOnlineSessionSearchResult &SessionResult);
	void destroySession();
	void startSession();

	FMultiplayerOnCreateSessionComplate multiplayerOnCreateSessionComplate;
	FMultiplayerOnFindSessionsComplate multiplayerOnFindSessionsComplate;
	FMultiplayerOnJoinSessionComplete multiplayerOnJoinSessionComplete;
	FMultiplayerOnDestroySessionComplete multiplayerOnDestroySessionComplete;
	FMultiplayerOnStartSessionComplete multiplayerOnStartSessionComplete;

protected:

	void onCreateSessionComplate(FName SessionName,bool bWasSuccesful);
	void onFindSessionsComplate(bool bWasSuccesful);
	void onJoinSessionComplate(FName SessionName,EOnJoinSessionCompleteResult::Type Result);
	void onDestroySessionComplate(FName SessionName, bool bWasSuccesful);
	void onStartSessionComplate(FName SessionName, bool bWasSuccesful);


private:
	IOnlineSessionPtr sessionInterface;
	TSharedPtr<FOnlineSessionSettings> lastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> lastSessionSearch;

	FOnCreateSessionCompleteDelegate createSessionComplateDelegate;
	FDelegateHandle createSessionComplateDelegateHandle;

	FOnFindSessionsCompleteDelegate findSessionsComplateDelegate;
	FDelegateHandle findSessionsComplateDelegateHandle;

	FOnJoinSessionCompleteDelegate joinSessionComplateDelegate;
	FDelegateHandle joinSessionComplateDelegateHandle;

	FOnDestroySessionCompleteDelegate destroySessionComplateDelegate;
	FDelegateHandle destroySessionComplateDelegateHandle;

	FOnStartSessionCompleteDelegate startSessionComplateDelegate;
	FDelegateHandle startSessionComplateDelegateHandle;

};
