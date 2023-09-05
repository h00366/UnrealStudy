// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatfrom.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT1_API AMovingPlatfrom : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AMovingPlatfrom();
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Editanywhere)
		float Speed = 300;

	UPROPERTY(EditAnyWhere, Meta = (MakeEditWidget = true))
		FVector targetLocation;

	void AddActiveTrigger();
	void RemoveActiveTrigger();


private:
	FVector GlobalTargetLocation;
	FVector GlobalStartLocation;


	UPROPERTY(EditAnywhere)
		int ActiveTrigger = 0;


};
