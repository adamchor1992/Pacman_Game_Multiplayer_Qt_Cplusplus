#include "foodball_manager.h"
#include "map.h"

FoodballManager::FoodballManager()
{
    m_FoodballCount = 0;

    Map referenceMap;
    m_PacmanMapForReference = referenceMap.GetPacmanPaths();

    CreateFoodballPositionsVector();
}

void FoodballManager::CreateFoodballPositionsVector()
{
    int verticalLinesX[10]={35, 79, 144, 209, 274, 340, 406, 470, 536, 579};
    int horizontalLinesY[10]={35, 121, 187, 252, 318, 384, 449, 514, 580, 645};

    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
                if(m_PacmanMapForReference.contains(QPoint(verticalLinesX[i],horizontalLinesY[j])))
                {
                    if((verticalLinesX[i] == 35 && horizontalLinesY[j] == 514) || (verticalLinesX[i] == 579 && horizontalLinesY[j] == 514)) //skip points where powerballs are
                    {
                        continue;
                    }
                    m_FoodballPositions.push_back(QPoint(verticalLinesX[i],horizontalLinesY[j]));
                }
        }
    }

    // Create a new vector without the duplicates
    QVector<QPoint> unique_foodballpositions;

    for (QVector<QPoint>::iterator iter = m_FoodballPositions.begin(); iter != m_FoodballPositions.end();iter++)
    {
        if(std::find(unique_foodballpositions.begin(),unique_foodballpositions.end(), *iter)==unique_foodballpositions.end())
        {
            unique_foodballpositions.push_back( *iter );
            m_FoodballCount++;
        }
    }
    std::swap( m_FoodballPositions, unique_foodballpositions );
}
