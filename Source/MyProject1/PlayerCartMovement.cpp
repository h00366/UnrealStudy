
#include "PlayerCartMovement.h"
#include "Engine/EngineTypes.h"
#include "Engine/Engine.h"  

// Sets default values for this component's properties
UPlayerCartMovement::UPlayerCartMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerCartMovement::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

// Called every frame
void UPlayerCartMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwnerRole() == ROLE_SimulatedProxy || GetOwnerRole() == ROLE_AutonomousProxy || GetOwnerRole() == ROLE_Authority
		|| GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy || GetOwner()->GetRemoteRole() == ROLE_AutonomousProxy || GetOwner()->GetRemoteRole() == ROLE_Authority)
	{
		LastMove = CreateMove(DeltaTime);
		SimulateMove(LastMove);

	}

	//		if (GetOwner()->HasAuthority())
	//		if(GetOwner()->GetRemoteRole() == ROLE_Authority || GetOwner()->GetRemoteRole() == ROLE_AutonomousProxy)
	//	{
	//		LastMove = CreateMove(DeltaTime);
	//		SimulateMove(LastMove);
	//	}
	//
	//

//	if (GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("MoveMentROLE_SimulatedProxy"));
//	}
//	else if (GetOwner()->GetRemoteRole() == ROLE_AutonomousProxy)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("MoveMentROLE_AutonomousProxy"));
//	}
//	else if (GetOwner()->GetRemoteRole() == ROLE_Authority)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("MoveMentROLE_Authority"));
//	}
//
	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetOwnerRole()_ROLE_SimulatedProxy"));
	}
	else if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetOwnerRole()_ROLE_AutonomousProxy"));
	}
	else if (GetOwnerRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetOwnerRole()_MoveMentROLE_Authority"));
	}

//
//	if (GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("ROLE_SimulatedProxy"));
//	}
//	else if (GetOwner()->GetRemoteRole() == ROLE_AutonomousProxy)
//	{
//		GetOwner()->GetRemoteRole();
//		UE_LOG(LogTemp, Warning, TEXT("ROLE_AutonomousProxy"));
//	}
//

	 //		 LastMove = CreateMove(DeltaTime);
	 //		 SimulateMove(LastMove);
}


void UPlayerCartMovement::SimulateMove(const FGoKartMove& Move)
{
	FVector Force = GetOwner()->GetActorForwardVector() * MaxDrivingForce * Move.Throttle;

	Force += GetAirResistance();
	Force += GetRollingResistance();

	FVector Acceleration = Force / Mass;

	Velocity = Velocity + Acceleration * Move.DeltaTime;

	ApplyRotation(Move.DeltaTime, Move.SteeringThrow);


	UpdateLocationFromVelocity(Move.DeltaTime);
}

void UPlayerCartMovement::ServerSimulateMove(const FGoKartMove& Move)
{
	FVector Force = GetOwner()->GetActorForwardVector() * MaxDrivingForce * Move.Throttle;

	Force += GetAirResistance();
	Force += GetRollingResistance();

	FVector Acceleration = Force / Mass;

	Velocity = Velocity + Acceleration * Move.DeltaTime;

	ApplyRotation(Move.DeltaTime, Move.SteeringThrow);

	UpdateLocationFromVelocity(Move.DeltaTime);
}

FGoKartMove UPlayerCartMovement::CreateMove(float Delta)
{
	FGoKartMove Move;
	Move.DeltaTime = Delta;
	Move.SteeringThrow = SteeringThrow;
	Move.Throttle = Throttle;
	Move.Time = GetWorld()->TimeSeconds;


	return Move;
}

FVector UPlayerCartMovement::GetAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector UPlayerCartMovement::GetRollingResistance()
{
	float AccelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;
	float NormalForce = Mass * AccelerationDueToGravity;
	return -Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
}


void UPlayerCartMovement::ApplyRotation(float DeltaTime, float SteeringThrows)
{
	float DeltaLocation = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;
	float Rotationangle = DeltaLocation / MinTurningRadius * SteeringThrows;
	FQuat RotationDelta(GetOwner()->GetActorUpVector(), Rotationangle);

	Velocity = RotationDelta.RotateVector(Velocity);

	GetOwner()->AddActorWorldRotation(RotationDelta);
}

void UPlayerCartMovement::UpdateLocationFromVelocity(float DeltaTime)
{
	FVector Translation = Velocity * 100 * DeltaTime;

	FHitResult Hit;

	GetOwner()->AddActorWorldOffset(Translation, true, &Hit);

	if (Hit.IsValidBlockingHit())
	{
		Velocity = FVector(0, 0, 0);
	}
}