// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"

void UMenu::menuSetup()
{
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

void UMenu::hostButtonClicked()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Host Butona tiklandi")));
	}
	if (multiplayerSessionsSubsystem)
	{
		multiplayerSessionsSubsystem->createSession(4,"FreeForAll");
	}
}

void UMenu::joinButtonClicked()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Join Butona tiklandi")));
	}
}
