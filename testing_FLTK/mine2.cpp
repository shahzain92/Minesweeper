//-------------------------------------------
//Created By Shahzain Ali
//-------------------------------------------


#include "Windows.h"
#include "mine2.h"

const int Mine::getNum()
{
 return numOfMines;
}

bool Mine::isFlagged()
{
  return flagged;

}
bool Mine::isQuestion()
{
	return question;
}
void Mine::setQuestion(bool q) 
{
	question = q;
}

void Mine::setFlag(bool a)
{
  flagged=a;
}
bool Mine::getState()
{
 return state;    
}
void Mine::setState(bool s)
{
 state=s;    
}
void Mine::setNum(int a)
{
 numOfMines = a;
}
Mine::Mine(bool m, int a , int b)
{
	x=a;
	y=b;
	mined=false;
	state=false;
 
}

Mine::Mine()
{

 state=false;   
}
bool Mine::isMined()
{
     
return mined;     
     
}
