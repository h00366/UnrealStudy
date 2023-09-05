// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerRow.h"
#include "Components/Button.h" 
#include "MainManu.h"

void UServerRow::SetUp(class UMainManu* Parent, uint32 Index)
{
	Parents = Parent;
	Indexs = Index;
	RowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
}

void UServerRow::OnClicked()
{
	Parents->SelectIndex(Indexs);
}
 