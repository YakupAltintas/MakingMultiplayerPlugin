// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Menu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSION_API UMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void menuSetup(int32 NumberOfPublicConnections=4,FString TypeOfMatch=FString(TEXT("FreeForAll")),FString lobbyPath = FString(TEXT("/Game/ThirdPerson/Maps/LobbyMap")));

protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void onCreateSession(bool bWasSuccesful);
	void onFindSessions(const TArray<FOnlineSessionSearchResult>& sessionResult, bool bWasSuccessful);
	void onJoinSession(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void onDestroySession(bool bWasSuccesful);
	UFUNCTION()	
	void onStartSession(bool bWasSuccesful);
private:
	UPROPERTY(meta = (BindWidget));
	class UButton* hostButton; 

	UPROPERTY(meta = (BindWidget));
	UButton* joinButton;

	UFUNCTION()
	void hostButtonClicked();

	UFUNCTION()
	void joinButtonClicked();

	void menuTearDown();

	class UMultiplayerSessionsSubsystem* multiplayerSessionsSubsystem;

	int32 numPublicConnections{ 4 };
	FString matchType{TEXT("FreeForAll")};
	FString pathToLobby{TEXT("")};

};
