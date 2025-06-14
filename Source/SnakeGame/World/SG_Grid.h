// Snake Game, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SG_WorldTypes.h"
#include "GameFramework/Actor.h"
#include "Core/Types.h"
#include "SG_Grid.generated.h"

namespace Snake
{
class Grid;
}

UCLASS()
class SNAKEGAME_API ASG_Grid : public AActor
{
    GENERATED_BODY()

public:
    ASG_Grid();
    virtual void Tick(float DeltaTime) override;

    /**
     * Set core model to be observed by the grid world actor
     * @param Grid  model object
     * @param CellSize  world size of the cell
     */
    void SetModel(const TSharedPtr<Snake::Grid>& Grid, uint32 CellSize);

    void UpdateColors(const FSnakeColors& ColorSet);

protected:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    USceneComponent* Origin;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* GridMesh;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UMaterialInstanceDynamic* GridMaterial;

    Snake::Dim GridDim;
    uint32 CellSize;
    uint32 WorldWidth;
    uint32 WorldHeight;

    void DrawGrid();
};
