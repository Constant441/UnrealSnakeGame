// Snake Game, Copyright LifeEXE. All Rights Reserved.

#include "World/SG_Grid.h"
#include "Core/Grid.h"
#include "DrawDebugHelpers.h"
#include "Components/LineBatchComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWorldGrid, All, All);

ASG_Grid::ASG_Grid()
{
    PrimaryActorTick.bCanEverTick = false;

    Origin = CreateDefaultSubobject<USceneComponent>("OriginComp");
    check(Origin);
    SetRootComponent(Origin);

    GridMesh = CreateDefaultSubobject<UStaticMeshComponent>("GridMeshComp");
    check(GridMesh);
    GridMesh->SetupAttachment(Origin);
}

void ASG_Grid::BeginPlay()
{
    Super::BeginPlay();
}

void ASG_Grid::SetModel(const TSharedPtr<Snake::Grid>& Grid, uint32 InCellSize)
{
    if (!Grid.IsValid())
    {
        UE_LOG(LogWorldGrid, Fatal, TEXT("Grid is null, game aborted!"));
    }
    GridDim = Grid.Get()->dim();
    CellSize = InCellSize;
    WorldWidth = GridDim.width * CellSize;
    WorldHeight = GridDim.height * CellSize;

    check(GridMesh->GetStaticMesh());
    const FBox Box = GridMesh->GetStaticMesh()->GetBoundingBox();
    const auto Size = Box.GetSize();

    check(Size.X);
    check(Size.Y);
    GridMesh->SetRelativeScale3D(FVector(WorldHeight / Size.X, WorldWidth / Size.Y, 1.0));
    GridMesh->SetRelativeLocation(0.5 * FVector(WorldHeight, WorldWidth, -Size.Z * GridMesh->GetRelativeScale3D().Z));

    GridMaterial = GridMesh->CreateAndSetMaterialInstanceDynamic(0);
    if (GridMaterial)
    {
        GridMaterial->SetVectorParameterValue("Division", FVector(GridDim.height, GridDim.width, 0.0));
    }
}

void ASG_Grid::UpdateColors(const FSnakeColors& ColorSet)
{
    if (GridMaterial)
    {
        GridMaterial->SetVectorParameterValue("BackgroundColor", ColorSet.GridBackgroundColor);
        GridMaterial->SetVectorParameterValue("LineColor", ColorSet.GridLineColor);
        GridMaterial->SetVectorParameterValue("WallColor", ColorSet.GridWallColor);
    }
}

void ASG_Grid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // DrawGrid();
}

void ASG_Grid::DrawGrid()
{
    if (!GetWorld() || !GetWorld()->LineBatcher) return;

    for (uint32 i = 0; i < GridDim.height + 1; ++i)
    {
        const FVector StartLocation = GetActorLocation() + GetActorForwardVector() * CellSize * i;
        // DrawDebugLine(GetWorld(), StartLocation, StartLocation + GetActorRightVector() * WorldWidth, FColor::Red, false, -1.0f, 0, 2.0f);
        GetWorld()->LineBatcher->DrawLine(StartLocation, StartLocation + GetActorRightVector() * WorldWidth, FLinearColor::Red, 1, 2.0f);
    }

    for (uint32 i = 0; i < GridDim.width + 1; ++i)
    {
        const FVector StartLocation = GetActorLocation() + GetActorRightVector() * CellSize * i;
        // DrawDebugLine(GetWorld(), StartLocation, StartLocation + GetActorForwardVector() * WorldHeight, FColor::Red, false, -1.0f,
        // 0, 2.0f);
        GetWorld()->LineBatcher->DrawLine(StartLocation, StartLocation + GetActorForwardVector() * WorldHeight, FLinearColor::Red, 1, 2.0f);
    }
}
