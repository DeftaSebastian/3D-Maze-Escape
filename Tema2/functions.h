#pragma once

#include "utils/glm_utils.h"
#include "lab_m1/Tema2/maze_cells.h"

using namespace std;
namespace functions
{
	void removeWalls(cell::Cell** matrix, int N)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (matrix[i][j].getOptionsSize() > 0)
                {
                    int number = rand() % matrix[i][j].getOptionsSize();
                    if (matrix[i][j].getOption(number) == 'W')
                    {
                        matrix[i][j].setLeftWall(false);
                        matrix[i][j - 1].setRightWall(false);
                    }
                    if (matrix[i][j].getOption(number) == 'N')
                    {
                        matrix[i][j].setTopWall(false);
                        matrix[i - 1][j].setBottomWall(false);
                    }
                }
            }
        }

        //now we remove a wall from the edge of the map so we can have an exit
        int number = rand() % 2; //we choose if we want to have the exit on left/right or top/bottom
        if (number == 1) //we chose left/right
        {
            number = rand() % 2; //we choose if we want left or right
            if (number == 1) // we chose left
            {
                number = rand() % 10; //we choose the height of the exit
                matrix[number][0].setLeftWall(false);
            }
            else //we chose right
            {
                number = rand() % 10; //we choose the height of the exit
                matrix[number][N - 1].setRightWall(false);
            }
        }
        else //we chose top/bottom
        {
            number = rand() % 2; //we choose if we want top or bottom
            if (number == 1) // we chose top
            {
                number = rand() % 10; //we choose the X pos of the exit
                matrix[0][number].setTopWall(false);
            }
            else
            {
                number = rand() % 10; //we choose the X pos of the exit
                matrix[N - 1][number].setBottomWall(false);
            }
        }
        return;
    }
}