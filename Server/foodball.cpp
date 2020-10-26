#include "foodball.h"
#include "map.h"

Foodball::Foodball()
{
    m_FoodballCount = 0;

    Map referencemap;
    m_PacmanMapForReference = referencemap.GetPacmanPaths();

    CreateFoodballPositionsVector();
}

void Foodball::CreateFoodballPositionsVector()
{
    int VerticalLinesX[10] = {35, 79, 144, 209, 274, 340, 406, 470, 536, 579};
    int HorizontalLinesY[10] = {35, 121, 187, 252, 318, 384, 449, 514, 580, 645};

    for(int i = 0; i < 10; i++)
    {
        for(int j=0; j < 10; j++)
        {
                if(m_PacmanMapForReference.contains(QPoint(VerticalLinesX[i], HorizontalLinesY[j])))
                {
                    /*Skip points where powerballs are*/
                    if((VerticalLinesX[i] == 35 && HorizontalLinesY[j] == 514) || (VerticalLinesX[i] == 579 && HorizontalLinesY[j] == 514))
                    {
                        continue;
                    }
                    m_FoodballPositions.push_back(QPoint(VerticalLinesX[i], HorizontalLinesY[j]));
                }
        }
    }

    // Create a new vector without the duplicates
    QVector<QPoint> unique_foodballpositions;

    for (QVector<QPoint>::iterator iter = m_FoodballPositions.begin(); iter != m_FoodballPositions.end(); iter++)
    {
        if(std::find(unique_foodballpositions.begin(),unique_foodballpositions.end(), *iter) == unique_foodballpositions.end())
        {
            unique_foodballpositions.push_back(*iter);
            ++m_FoodballCount;
        }
    }
    std::swap( m_FoodballPositions, unique_foodballpositions );
}
