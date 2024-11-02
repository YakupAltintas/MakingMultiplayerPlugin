// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

void ALobbyGameMode::PostLogin(APlayerController* newPlayer)
{
	Super::PostLogin(newPlayer);
	if (GameState)
	{
		int32 numberOfPlayers = GameState.Get()->PlayerArray.Num();
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Blue, FString::Printf(TEXT("Oyuncu sayisi : %d "),numberOfPlayers));
			APlayerState* playerState =newPlayer->GetPlayerState<APlayerState>();
			if (playerState)
			{
				FString playerName=playerState->GetPlayerName();
				GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Blue, FString::Printf(TEXT("%s Oyuna katildi "), *playerName));
			}
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	APlayerState* playerState = Exiting->GetPlayerState<APlayerState>();
	if (playerState)
	{
		FString playerName = playerState->GetPlayerName();
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Blue, FString::Printf(TEXT("%s Oyundan Ayrildi "), *playerName));
		int32 numberOfPlayers = GameState.Get()->PlayerArray.Num();
		GEngine->AddOnScreenDebugMessage(1, 60.f, FColor::Blue, FString::Printf(TEXT("Oyuncu sayisi : %d "), numberOfPlayers ));
	}
}
