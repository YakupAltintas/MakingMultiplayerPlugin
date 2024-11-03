// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"

void UMenu::menuSetup(int32 NumberOfPublicConnections ,FString TypeOfMatch, FString lobbyPath)
{
	pathToLobby = FString::Printf(TEXT("%s?listen"),*lobbyPath);
	numPublicConnections = NumberOfPublicConnections;
	matchType = TypeOfMatch;

	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* world = GetWorld();
	if (world)	
	{
		APlayerController* playerController = world->GetFirstPlayerController();
		if (playerController)
		{
			FInputModeUIOnly inputModeData;
			inputModeData.SetWidgetToFocus(TakeWidget());
			inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			playerController->SetInputMode(inputModeData);
			playerController->SetShowMouseCursor(true);

		}
	}
	UGameInstance* gameInstance = GetGameInstance();
	if (gameInstance)
	{
		multiplayerSessionsSubsystem = gameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}
	if (multiplayerSessionsSubsystem)
	{
		multiplayerSessionsSubsystem->multiplayerOnCreateSessionComplate.AddDynamic(this,&ThisClass::onCreateSession);
		multiplayerSessionsSubsystem->multiplayerOnFindSessionsComplate.AddUObject(this,&ThisClass::onFindSessions);
		multiplayerSessionsSubsystem->multiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::onJoinSession);
		multiplayerSessionsSubsystem->multiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::onDestroySession);
		multiplayerSessionsSubsystem->multiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::onStartSession);
	}


}

bool UMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (hostButton)
	{
		hostButton->OnClicked.AddDynamic(this, &ThisClass::hostButtonClicked);
	}
	if (joinButton)
	{
		joinButton->OnClicked.AddDynamic(this, &ThisClass::joinButtonClicked);
	}
	return true;
}

void UMenu::NativeDestruct()
{
	menuTearDown();
	Super::NativeDestruct();
}

void UMenu::onCreateSession(bool bWasSuccesful)
{
	if (bWasSuccesful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("session basariyle olusturuldu! ")));
		}
		UWorld* world = GetWorld();
		if (world)
		{
			world->ServerTravel(pathToLobby);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("session olusturulamadi! ")));
		}
		hostButton->SetIsEnabled(true);
	}
}

void UMenu::onFindSessions(const TArray<FOnlineSessionSearchResult>& sessionResult, bool bWasSuccessful)
{
	if (multiplayerSessionsSubsystem == nullptr)
	{
		return;
	}

	for (auto Result : sessionResult)
	{
		FString settingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), settingsValue);
		if (settingsValue == matchType)
		{
			multiplayerSessionsSubsystem->joinSession(Result);
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("session bulundu! ")));
			}
			return;
		}
	}
	if (!bWasSuccessful || sessionResult.Num() == 0)
	{
		joinButton->SetIsEnabled(true);
	}
}

void UMenu::onJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* subsystem = IOnlineSubsystem::Get();
	if (subsystem)
	{
		IOnlineSessionPtr sessionInterface = subsystem->GetSessionInterface();
		if (sessionInterface.IsValid()) {
			FString adress;
			sessionInterface->GetResolvedConnectString(NAME_GameSession, adress);
			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(adress, ETravelType::TRAVEL_Absolute);
			}
		}
	}
	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		joinButton->SetIsEnabled(true);
	}
}

void UMenu::onDestroySession(bool bWasSuccesful)
{
}

void UMenu::onStartSession(bool bWasSuccesful)
{
}



void UMenu::hostButtonClicked()
{
	hostButton->SetIsEnabled(false);
	if (multiplayerSessionsSubsystem)
	{
		multiplayerSessionsSubsystem->createSession(numPublicConnections,matchType);
	}
}

void UMenu::joinButtonClicked()
{
	joinButton->SetIsEnabled(false);
	if (multiplayerSessionsSubsystem)
	{
		multiplayerSessionsSubsystem->findSession(10000);
	}
}

void UMenu::menuTearDown()
{
	RemoveFromParent();
	UWorld* world = GetWorld();
	if (world)
	{
		APlayerController* playerController = world->GetFirstPlayerController();
		if (playerController)
		{
			FInputModeGameOnly inputModeData;
			playerController->SetInputMode(inputModeData);
			playerController->SetShowMouseCursor(false);
		}
	}
}
