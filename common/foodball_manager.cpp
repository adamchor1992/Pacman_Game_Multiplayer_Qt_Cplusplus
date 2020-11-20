#include "foodball_manager.h"

FoodballManager::FoodballManager()
{

}

QVector<QPoint> FoodballManager::GenerateFoodballPositions(const Map& map)
{
    int verticalLinesX[10]={35, 79, 144, 209, 274, 340, 406, 470, 536, 579};
    int horizontalLinesY[10]={35, 121, 187, 252, 318, 384, 449, 514, 580, 645};

    QVector<QPoint> foodballPositions;

    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            if(map.GetPacmanPaths().contains(QPoint(verticalLinesX[i], horizontalLinesY[j])))
            {
                if((verticalLinesX[i] == 35 && horizontalLinesY[j] == 514) || (verticalLinesX[i] == 579 && horizontalLinesY[j] == 514))
                {
                    /*Skip points where powerballs are*/
                    continue;
                }
                foodballPositions.push_back(QPoint(verticalLinesX[i],horizontalLinesY[j]));
            }
        }
    }

    /*Create a new vector without the duplicates*/
    QVector<QPoint> uniqueFoodballPositions;

    for (QVector<QPoint>::iterator iter = foodballPositions.begin(); iter != foodballPositions.end();iter++)
    {
        if(std::find(uniqueFoodballPositions.begin(), uniqueFoodballPositions.end(), *iter) == uniqueFoodballPositions.end())
        {
            uniqueFoodballPositions.push_back( *iter );
        }
    }

    std::swap(foodballPositions, uniqueFoodballPositions);

    return foodballPositions;
}
