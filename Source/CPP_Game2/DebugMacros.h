#pragma once

#define DRAW_SPHERE(Location) DrawDebugSphere(GetWorld(), Location, 25,12,FColor::Red, true, -1, 0.f, 1.1f);
#define DRAW_SPHERE_COLOR(Location, Color) DrawDebugSphere(GetWorld(), Location, 8,12,Color, false, 5, 0.f, 1f);
#define DRAW_LINE(StartLocation, EndLocation) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1, 0.f, 1.5f);
#define DRAW_VECTOR(StartLocation, EndLocation) if(GetWorld()) \
	{\
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true, -1, 0.f, 1.5f);\
	DrawDebugPoint(GetWorld(), EndLocation, 25.f, FColor::Red, true);\
}
#define DRAW_SPHERE_SINGLEFRAME(Location) DrawDebugSphere(GetWorld(), Location, 25,12,FColor::Red, false, -1.f, 0.f, 1.1f);
#define DRAW_LINE_SINGLEFRAME(StartLocation, EndLocation) DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1.f, 0.f, 1.5f);
#define DRAW_VECTOR_SINGLEFRAME(StartLocation, EndLocation) if(GetWorld()) \
	{\
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, -1, 0.f, 1.5f);\
	DrawDebugPoint(GetWorld(), EndLocation, 25.f, FColor::Red, false, -1.f);\
}