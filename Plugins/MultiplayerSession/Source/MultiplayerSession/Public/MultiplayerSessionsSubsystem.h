// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionsSubsystem.generated.h"

/**
 *
 */
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
protected:

	void onCreateSessionComplate(FName SessionName,bool bWasSuccesful);
	void onFindSessionsComplate(bool bWasSuccesful);
	void onJoinSessionComplate(FName SessionName,EOnJoinSessionCompleteResult::Type Result);
	void onDestroySessionComplate(FName SessionName, bool bWasSuccesful);
	void onStartSessionComplate(FName SessionName, bool bWasSuccesful);


private:
	IOnlineSessionPtr sessionInterface;
	TSharedPtr<FOnlineSessionSettings> lastSessionSettings;

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
