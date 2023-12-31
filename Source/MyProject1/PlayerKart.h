
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerCartMovement.h"
#include "PlayerCartReplicator.h"
#include "PlayerKart.generated.h"


UCLASS()
class MYPROJECT1_API APlayerKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerKart();

protected:
	// Called when the game starts or when spawned
	 void BeginPlay();

public:
	// Called every frame
	 void Tick(float DeltaTime);
	 
	 void SetUp(int32 PlayerIndex);
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPlayerCartMovement* MovementComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UPlayerCartReplicator* MovementReplicator;
	int32 PlayerIndex;

private:

	void MoveForward(float Value);
	void MoveRight(float Value);

	void MyActionPressed();
	void SetSessionNumber(uint32 NewSessionNumber);

};
