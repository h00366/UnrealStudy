// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatfrom.h"

AMovingPlatfrom::AMovingPlatfrom()
{
	PrimaryActorTick.bCanEverTick = true;			// PrimaryActorTick = ���� ƽ, bCanEverTick = �̺�Ʈ ����;

	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatfrom::BeginPlay()
{
	Super::BeginPlay();		

	if (HasAuthority())
	{
		SetReplicates(true);							// �� ���Ͱ� ��Ʈ��ũ Ŭ���̾�Ʈ�� �����Ǵ��� ���θ� �����մϴ�.
		SetReplicateMovement(true);						// �� ������ �������� ��Ʈ��ũ Ŭ���̾�Ʈ�� �����Ǵ��� ���θ� �����մϴ�.
	}
	GlobalStartLocation = GetActorLocation(); 
	GlobalTargetLocation = GetTransform().TransformPosition(targetLocation);

}	
																										 
void AMovingPlatfrom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);								// super : �θ� ȣ��
	if (ActiveTrigger < 1) return;

	if (HasAuthority())
	{		
		FVector Location = GetActorLocation();			// Normalize = Vector ��ü�� �ٲ�
		float JournyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
		float JournyTravelled = (Location - GlobalStartLocation).Size();

		if (JournyTravelled >= JournyLength)
		{
			FVector Swap = GlobalStartLocation;
			GlobalStartLocation = GlobalTargetLocation;
			GlobalTargetLocation = Swap;
		}
		FVector Direction = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();	// GetSafeNormal = ������ ����ȭ�� ���纻�� �������� �������� Ȯ��
		Location += Speed * DeltaTime * Direction;
		SetActorLocation(Location);
	}
//	HasAuthority();										// ������ �������� Ŭ������ �����ϴ� �Լ�
}

void AMovingPlatfrom::AddActiveTrigger()
{
	ActiveTrigger++;
	UE_LOG(LogTemp, Warning, TEXT("ActiveTrigger"));
}

void AMovingPlatfrom::RemoveActiveTrigger()
{
	if(ActiveTrigger > 0)
	ActiveTrigger--;

	UE_LOG(LogTemp, Warning, TEXT("ActiveTriggeroff"));
	 
}
