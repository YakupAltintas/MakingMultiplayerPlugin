// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

void UMenu::menuSetup(int32 NumberOfPublicConnections ,FString TypeOfMatch)
{
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
			world->ServerTravel(mapPath);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("session olusturulamadi! ")));
		}
	}
}

void UMenu::onFindSessions(const TArray<FOnlineSessionSearchResult>& sessionResult, bool bWasSuccessful)
{
}

void UMenu::onJoinSession(EOnJoinSessionCompleteResult::Type Result)
{
}

void UMenu::onDestroySession(bool bWasSuccesful)
{
}

void UMenu::onStartSession(bool bWasSuccesful)
{
}



void UMenu::hostButtonClicked()
{
	if (multiplayerSessionsSubsystem)
	{
		multiplayerSessionsSubsystem->createSession(numPublicConnections,matchType);
	}
}

void UMenu::joinButtonClicked()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Join Butona tiklandi")));
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
