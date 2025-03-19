// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_BallController.h"

#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void AAIC_BallController::SortWayPoints()
{
	WayPoints.Sort([this](const ATargetPoint& A, const ATargetPoint& B)
	{
		return A.GetName() < B.GetName();
	});
}

void AAIC_BallController::AdjustSpeedBasedOnCurve(ACharacter* AICharacter, FVector CurrentDirection, FVector NextWaypointDirection)
{
	if (!AICharacter)
		return;
	
	UCharacterMovementComponent* CharacterMovementComponent = AICharacter->GetCharacterMovement();
	if (!CharacterMovementComponent)
		return;

	if (CurrentDirection.IsNearlyZero() || NextWaypointDirection.IsNearlyZero())
		return;
	
	float DotProduct = FVector::DotProduct(CurrentDirection.GetSafeNormal(), NextWaypointDirection.GetSafeNormal());
	DotProduct = FMath::Clamp(DotProduct, -1.0f, 1.0f);
	float Angle = FMath::Acos(DotProduct) * (180.0f / PI);

	if (Angle < 20.0f)
	{
		CharacterMovementComponent->MaxWalkSpeed = MaxSpeed;
	}
	else
	{
		CharacterMovementComponent->MaxWalkSpeed = MinSpeed;
	}
	
}

FVector AAIC_BallController::GetAdjustedDirection(FVector TargetDirection)
{
	if (TargetDirection.IsNearlyZero())
		return TargetDirection;

	float RandomOffsetX = FMath::RandRange(-DeviationStrenght, DeviationStrenght);
	float RandomOffsetY = FMath::RandRange(-DeviationStrenght, DeviationStrenght);

	FVector Offset(RandomOffsetX, RandomOffsetY, 0.0f);

	FVector AdjustedDirection = (TargetDirection + Offset);

	return AdjustedDirection;
}

void AAIC_BallController::CheckNavMeshProximity()
{
	FVector AgentLocation = GetActorLocation();

	// Obtenemos el área transitable más cercana
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem) return;

	// Usamos una función para obtener la ubicación más cercana dentro de la NavMesh
	FVector ClosestPoint = NavSystem->GetNavMeshBoundsVolume()->GetActorLocation(); // O ajusta esto según cómo sea la malla de navegación de tu mapa.

	// Realizamos un Line Trace desde la pelota hasta el borde de la NavMesh para verificar la proximidad
	FVector StartLocation = AgentLocation;
	FVector EndLocation = ClosestPoint;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // Ignoramos a la pelota en el trace

	// Realizamos el Line Trace
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);
	if (bHit)
	{
		// Comprobamos la distancia
		float DistanceToNavMeshEdge = HitResult.Distance;

		// Si está demasiado cerca del borde de la NavMesh, reducir la velocidad
		if (DistanceToNavMeshEdge < SafeDistanceFromEdge)
		{
			// Realizamos una corrección para evitar caer
			ApplyEdgeAvoidance();
		}
	}
}

