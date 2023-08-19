// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"
//#include ""

/**
 * 
 */
UCLASS()
class MYPROJECT1_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ServerName;

	void SetUp(class UMainManu* Parent, uint32 Index);
private:
	UPROPERTY(meta = (BindWidget))
		class UButton* RowButton;
	
	class UMainManu* Parents;
	uint32 Indexs;

	UFUNCTION()
		void OnClicked();	
};
