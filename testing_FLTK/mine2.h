//-------------------------------------------
//Created By Shahzain Ali
//-------------------------------------------

#ifndef _MINE2_H_
#define _MINE2_H_
#include <FL/Fl.H>
#include <FL/Fl_Button.H>
using namespace std;

class Mine
{
      
          private:
                      int numOfMines;//number of mines in the neighborhood
                      bool state;//has been clicked yet
					  bool flagged;//has been flagged yet
					  bool question;
          public:
                      Mine* up;//negihboring cells
                      Mine* down;
                      Mine* left;
                      Mine* right;
					  Mine* upright;
		   			  Mine* upleft;	      
		     		  Mine* downright;
					  Mine* downleft;
                 
                      bool mined;        //is mined
                      int x;
                      int y;
                      Mine(bool m, int x, int y);
                      Mine();
                      Fl_Button *cell;  //actual game button
                      const int getNum();//to get numOfMines
                      void setState(bool b);//to set state
                      bool isMined();//to check whether is mined
                      bool getState();//to get the state
		      		  bool isFlagged();//to check whether flagged
					  bool isQuestion();
		              void setFlag(bool a);//set the flagged to true or false
					  void setQuestion(bool q);
                      void setNum(int a);//set numOfMines to actuall number
};  
                 
#endif
