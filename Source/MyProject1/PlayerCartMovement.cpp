#include "PlayerCartMovement.h"

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

	if (GetOwnerRole() == ROLE_AutonomousProxy || GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy || GetOwnerRole() == ROLE_Authority)
	{
		LastMove = CreateMove(DeltaTime);
		SimulateMove(LastMove);
		UE_LOG(LogTemp, Warning, TEXT("Not LastMove.Throttle : %f"), LastMove.Throttle);
	}
//	if (GetOwnerRole() == ROLE_AutonomousProxy || GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
//	{
//		LastMove = CreateMove(DeltaTime);
//		SimulateMove(LastMove);
//		UE_LOG(LogTemp, Warning, TEXT("Not LastMove.Throttle : %f"), LastMove.Throttle);
//	}
}

void UPlayerCartMovement::SimulateMove(const FGoKartMove& Move)
{
	UE_LOG(LogTemp, Warning, TEXT("Test"));
	FVector Force = GetOwner()->GetActorForwardVector() * MaxDrivingForce * Move.Throttle;

	Force += GetAirResistance();
	Force += GetRollingResistance();

	FVector Acceleration = Force / Mass;

	Velocity = Velocity + Acceleration * Move.DeltaTime;

	ApplyRotation(Move.DeltaTime, Move.SteeringThrow);

	UE_LOG(LogTemp, Warning, TEXT("%d"), Move.SteeringThrow);
	UE_LOG(LogTemp, Warning, TEXT("%d"), Move.Throttle);

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