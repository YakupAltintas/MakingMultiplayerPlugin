// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	void menuSetup();

protected:
	virtual bool Initialize() override;
private:
	UPROPERTY(meta = (BindWidget));
	class UButton* hostButton; 

	UPROPERTY(meta = (BindWidget));
	UButton* joinButton;

	UFUNCTION()
	void hostButtonClicked();

	UFUNCTION()
	void joinButtonClicked();

	class UMultiplayerSessionsSubsystem* multiplayerSessionsSubsystem;
};
