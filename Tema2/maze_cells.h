#pragma once
#include "components/simple_scene.h"



namespace cell
{
	class Cell
	{
	private:
		int x;
		int y;
		bool topWall;
		bool bottomWall;
		bool rightWall;
		bool leftWall;
		std::vector<char> options;

    public:

        Cell()
        {

        };

        Cell(int y, int x)
        {
            this->x = x;
            this->y = y;
            this->bottomWall = true;
            this->leftWall = true;
            this->topWall = true;
            this->rightWall = true;
            if (x > 0)
            {
                this->options.push_back('W');
            }
            if (y > 0)
            {
                this->options.push_back('N');
            }
        };

        float getX()
        {
            return this->x;
        };

        float getY()
        {
            return this->y;
        };

        bool getBottomWall()
        {
            return this->bottomWall;
        };

        bool getTopWall()
        {
            return this->topWall;
        };

        bool getRightWall()
        {
            return this->rightWall;
        };

        bool getLeftWall()
        {
            return this->leftWall;
        };

        std::vector<char> getOptions()
        {
            return this->options;
        };

        int getOptionsSize()
        {
            return this->options.size();
        };

        char getOption(int number)
        {
            return this->options[number];
        };

        void setBottomWall(bool newBottomWall)
        {
            this->bottomWall = newBottomWall;
        };

        void setTopWall(bool newTopWall)
        {
            this->topWall = newTopWall;
        };

        void setRightWall(bool newRightWall)
        {
            this->rightWall = newRightWall;
        };

        void setLeftWall(bool newLeftWall)
        {
            this->leftWall = newLeftWall;
        };
	};



    
}