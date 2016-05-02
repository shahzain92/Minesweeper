//-------------------------------------------
//Created By Shahzain Ali
//-------------------------------------------
#include "Windows.h"
#include <FL/Fl_Sys_Menu_Bar.H>
#include <FL/Fl.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Menu_Bar.h>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_message.H>
#include <ctime> //to implement a second counter
#include <cstdlib>
#include "mine2.h" //includes class definitions
#include <sstream>
#include <Fl/Fl_Widget.H>
#include <string>
#include <iostream>
#include <fstream> //for file manipulation
#include <vector> // for stat insertion
#include <map> //for highscore extraction
#include <iomanip>
#include <FL/Fl_JPEG_Image.h>  // to load jpeg images
using namespace std;

Fl_JPEG_Image m("m.jpg");
Fl_JPEG_Image flg("flg.jpg");
Fl_JPEG_Image Shahz("Shahz.jpg");


Mine **game; //actual game board
//Fl_Sys_Menu_Bar *menu;				//Not sure how to use it. (NEED HELP)
Fl_Box *imageBox;
Fl_Window *window;
Fl_Window *fl;
Fl_Button *about;
Fl_Button *quit;
Fl_Button *new_game; // button to generate new games
Fl_Box *clock_box; //clock box to see the score
int r; //variable to hold row number
int c; //variable to hold column number
int mines; // variable to hold how many mines
bool **board;// helper board to set mines
int flagsleft; //variable to hold flagsleft
Fl_Box* flags_left; //box to show how many flags left during the game
const int PAD=15; //width to be used initializing window
int timecheck=0; //check if the game has started
int t=0;//actual score stored in the time box
int score;// final score when someone wins
string username; //holds the username inputted at the beginning
int middle=0;//variable to stop revealing with middle click any further
int i;//used to split lines readed form stats.txt
int j;//used to split lines readed from stats.txt
bool stop;//to stop middle click revealing to reach next game
void revealMines(int r, int c);
void clearGame(int r, int c, int mines);
void setBoard(int r, int c, int mines);
void countMines(int r, int c);
void initNeighbors(int r, int c);
void setCalls(int r, int c);
void redoCells(int r, int c);
void makeWindow(int r, int c, int mines);
string tostr(int a);
int toint(string a);
int countDebug = 1;
int mineLeft = 10;


bool debug = false;

string tostr(int a) {// converts int a to stl string
	
	stringstream s;
    s<<a;
    return s.str();   
}

int toint(string a) {			//converts stl string a to int
	int i;

	stringstream s(a);
	s >> i;
	return  i;
}


void makeWindow(int r, int c,int mines) {//creates the window according to difficulty level. r is number of rows c is number of columns and mines is the number of mines              
	window = new Fl_Window (c*15, r*15 + 30, "MineSweeper");//the window
    new_game = new Fl_Button((c*15)/2-20,0,40,15,"New"); // new game button
    string val=tostr(mines);
    flags_left=new Fl_Box(3,0,30,15,"");//box to show how many flags left
    flags_left->copy_label(val.c_str());
    flags_left->labelcolor(FL_RED);
    flags_left->color(FL_BLACK);
    clock_box=new Fl_Box(c*15-40,0,40,15,"0");  //box to show the seconds passed
    clock_box->labelfont(FL_TIMES);
	about=new Fl_Button(3, r * 15 + 15, 40, 15, "About"); //button to show info
	quit=new Fl_Button(c * 15 - 42, r * 15 + 15, 40, 15, "Quit");//button to exit the game
}
void info(Fl_Widget* widget, void* vfl) {

	Fl_Output *output = new Fl_Output(0, 0, 500, 40, "Hello");
	output->value("Minesweeper FLTK Creaters: Shahzain Ali");
	Fl_Box *pic1 = new Fl_Box(0, 100, 500, 41, "");
	pic1->image(Shahz);
	fl->redraw();
	fl->end();
	fl->show();
	fl->make_current();
}


void exit(Fl_Widget* widget, void*) {  //close the program when quit pressed.
	
	exit(EXIT_SUCCESS);

}

void callback(void* user) {		//function to start and continue timing the only input is the box that keeps time

  string sval=tostr(t);

  ((Fl_Box*)user)->copy_label(sval.c_str());
  t++;
  Fl::repeat_timeout(1.0, callback, user);
}

bool checkGame(int r, int c, int mines) {    //checks whether the game has been won. r is number of rows, c is no of columns mines is no of mines
  int same=0;
	for(int i=0;i<r;i++) {
		for(int j=0;j<c;j++) {
			if(game[i][j].isFlagged() && board[i+1][j+1]) {
				same++;
			}
		}
	} cout << "You Won " << endl;

	if(same==mines)
		return true;
	else
		return false;
}

void redoCells(int r, int c) {   //sets the cells to mined if the corresponding board elemenent is true.  r:no of rows, c:no of columns
	
	for(int i=0;i<r;i++) {
		for(int j=0; j<c;j++) {
			if(board[i+1][j+1]) {
				game[i][j].mined=true;
			}
		}
	}
}

void clearGame(int r, int c, int mines) {//clears the elements so the game is at the state like at the beginning, r:no of rows, c:no of columns, mines: no of mines

	for(int i=0; i<r;i++) {
		for(int j=0;j<c;j++) {
			game[i][j].setState(false);
			game[i][j].setFlag(false);
			game[i][j].mined=false;
			game[i][j].cell->color(FL_GRAY);
			game[i][j].cell->copy_label(" ");      //empties all the mine buttons.
			game[i][j].cell->color(FL_GRAY);
			flagsleft=mines;
			string fleft=tostr(flagsleft);
			flags_left->copy_label(fleft.c_str());
			t=0;
			timecheck=0;
			string tleft= tostr(t);
			clock_box->copy_label(tleft.c_str());
		}
    }
}

int countTimes = 0;
void revealMines(int r, int c) {//colors the mines to red or cyan when the game is lost. r:no of rows, c:no of columns

	for(int i=0; i<r;i++) {
		for(int j=0; j<c; j++) {
			if(game[i][j].isMined()) {
				
				
				game[i][j].cell->copy_label("X");
				game[i][j].cell->color(FL_RED);
				game[i][j].cell->labelcolor(FL_BLACK);
				
			}

			if (game[i][j].isMined()&&game[i][j].isFlagged()) {
				game[i][j].cell->copy_label("F");
				game[i][j].cell->color(FL_CYAN);
				game[i][j].cell->labelcolor(FL_BLACK);
			}
		}
    }
}
void showMines()
{
	for (int i = 0; i<r; i++) {
		for (int j = 0; j<c; j++) {
			if (game[i][j].isMined()) {
				game[i][j].cell->copy_label("");
				game[i][j].cell->color(FL_CYAN);
			}
		}
	}
}


void gen_new(Fl_Widget* widget, void*) {//function for new game button. It generates a new game. parameters are not used
	if (debug == false) {
		cout << " New Game Started " << endl;
		clearGame(r, c, mines);
		setBoard(r, c, mines);
		redoCells(r, c);
		countMines(r, c);
		initNeighbors(r, c);
		Fl::remove_timeout(callback, clock_box);//stop timer
	}
	else if(debug == true){
		clearGame(r, c, mines);
		setBoard(r, c, mines);
		redoCells(r, c);
		countMines(r, c);
		initNeighbors(r, c);
		Fl::remove_timeout(callback, clock_box);//stop timer
		showMines();

	}

}

void reveal(Fl_Widget* widget, void* mine) {//ultimate function to reveal cells. cases for left'right and middle mouse clicks
	bool gameProgBar = true;
	if(timecheck==0) {//to check whether timer has started
		callback(clock_box);//start time
		timecheck=1;//flag so not to start time counter again
	}
 
Mine* current=(Mine*) mine;
int value=current->getNum();
	if(Fl::event_button()==1) {      //left-click
		if(gameProgBar)
			cout << "Game In Progress.... " << endl;
		gameProgBar = false;
		if(current->isFlagged()) {      //if the cell is flagged erase the flag
			if(!current->isMined()) {
				current->setFlag(false);
				flagsleft++;
				string fleft=tostr(flagsleft);
				flags_left->copy_label(fleft.c_str());
			}
		 }
		if(!current->isMined()) {     // if the cell is not mined reveal the cell 
			if(!current->getState()) {
				Fl_Button* cell=(Fl_Button*)widget;
					if(value==0) {    //if value is 0 reveal the adjacent cells also
						string sval=tostr(value);
						cell->copy_label("");
						cell->color(FL_WHITE);
						current->setState(true);
						cell->labelfont(FL_BOLD);
						if(current->up!=NULL)
						reveal(current->up->cell,current->up);//recursions to reveal the neighboring cells
						if(current->down!=NULL)
						reveal(current->down->cell,current->down);
						if(current->right!=NULL)
						reveal(current->right->cell,current->right);
						if(current->left!=NULL)
						reveal(current->left->cell,current->left);
						if(current->downleft!=NULL)
						reveal(current->downleft->cell,current->downleft);
						if(current->downright!=NULL)
						reveal(current->downright->cell,current->downright);
						if(current->upleft!=NULL)
						reveal(current->upleft->cell,current->upleft);
						if(current->upright!=NULL)
						reveal(current->upright->cell,current->upright);
					}

				else {
						string sval=tostr(value);
						cell->copy_label(sval.c_str());

				if(value==1)
				cell->labelcolor(FL_BLUE);
				else if(value==2)
				cell->labelcolor(FL_DARK_GREEN);
				else if(value==3)
				cell->labelcolor(FL_DARK_RED);
				else if(value==4)
				cell->labelcolor(fl_rgb_color(128, 0, 128)); //purple
				else 
				cell->labelcolor(FL_BLACK);
				cell->color(FL_WHITE);
				current->setState(true);
				cell->labelfont(FL_BOLD);  
			}
		}
			
   }

	else {    //mine is clicked game is lost, starts a new game
	revealMines(r,c);
	Fl::remove_timeout(callback,clock_box);//stop timer
	cout << "Sorry. You Lost The Game " << endl;
	fl_alert("You lose. Press close to start a new game"); 
	clearGame(r,c,mines);
	setBoard(r,c,mines);
	redoCells(r,c);
	countMines(r,c);
     t=0;
     timecheck=0;
	}
}
 else if (Fl::event_button()==3) {//right-click
      current=(Mine*) mine;
     Fl_Button* cell=(Fl_Button*)widget;
	 bool isClick = false;

	 if (!current->isQuestion()) {
		 current->setQuestion(true);
		 cell->copy_label("");
	 }

	 else if (flagsleft > 0 && !current->getState() && !current->isFlagged()) {
		 //if() {//if there are enough flags left and the cell is not openned
		 if (current->isMined()) {
			 mineLeft--;
			 cout << " Mines Left: " << mineLeft << endl;
		 }
		 current->setFlag(true);
		 cell->copy_label("F");
		 cell->labelcolor(FL_RED);
		 cell->labelfont(FL_BOLD);
		 flagsleft--;
		 cout << "Flags Left: " <<flagsleft << endl;
		 countTimes++;

		 string fleft = tostr(flagsleft);
		 flags_left->copy_label(fleft.c_str());
		 if (flagsleft == 0) {     //if no flags left to position
			 if (checkGame(r, c, mines)) {
				 Fl::remove_timeout(callback, clock_box);//stop timer
				 score = t;
				 fl_alert("Hurraaaay You win :)");
				 clearGame(r, c, mines);
				 setBoard(r, c, mines);
				 redoCells(r, c);
				 countMines(r, c);
				 t = 0;
				 timecheck = 0;
			 }
		 }
	 }

	 else if (current->isFlagged()) {//if cell flagged remove flag
		current->setFlag(false);
		current->setQuestion(false);
		cell->copy_label("?");
		cell->labelcolor(FL_BLACK);
		flagsleft++;
		cout << "Flags Left: " <<flagsleft << endl;
		string fleft=tostr(flagsleft);
		flags_left->copy_label(fleft.c_str());
		countTimes++;
	}
}
   else if(Fl::event_button()==2) {       //if middle clicked
		if(!current->isFlagged()) { //if current cell is not flagged
			int count=0;
 if(stop==false) {
	if(!current->isMined()) {//this part is same as leftclick    
		if(!current->getState()) {
			Fl_Button* cell=(Fl_Button*)widget;
				if(value==0) {
				 string sval=tostr(value);
				 cell->copy_label("");
				 cell->color(FL_WHITE);
				 current->setState(true);
				 cell->labelfont(FL_BOLD);
                 if(current->up!=NULL&&current->up->getState()==false)//same as left-click but state checked beforehand to prevent infinite recursion
                 reveal(current->up->cell,current->up);
                 if(current->down!=NULL&&current->down->getState()==false)
                 reveal(current->down->cell,current->down);
                 if(current->right!=NULL&&current->right->getState()==false)
                 reveal(current->right->cell,current->right);
                 if(current->left!=NULL&&current->left->getState()==false)
                 reveal(current->left->cell,current->left);
                 if(current->downleft!=NULL&&current->downleft->getState()==false)
                 reveal(current->downleft->cell,current->downleft);
                 if(current->downright!=NULL&&current->downright->getState()==false)
                 reveal(current->downright->cell,current->downright);
                 if(current->upleft!=NULL&&current->upleft->getState()==false)
                 reveal(current->upleft->cell,current->upleft);
                 if(current->upright!=NULL&&current->upright->getState()==false)
                 reveal(current->upright->cell,current->upright);      
   }
 else {
     string sval=tostr(value);
     
     cell->copy_label(sval.c_str());

     if(value==1)
       cell->labelcolor(FL_BLUE);
     else if(value==2)
       cell->labelcolor(fl_rgb_color(123,54,235));
     else if(value==3)
       cell->labelcolor((Fl_Color)14);
     else if(value==4)
       cell->labelcolor(fl_rgb_color(213,104,53));
     else 
       cell->labelcolor(FL_BLACK);
     cell->color(FL_WHITE);
     current->setState(true);
     cell->labelfont(FL_BOLD);

   }
   }
   }
 else if(current->isMined() && !current->isFlagged())//if cell mined but not flagged
   {
     revealMines(r,c);
       Fl::remove_timeout(callback,clock_box);
     fl_alert("You lose. Press close to start a new game");
     clearGame(r,c,mines);
     setBoard(r,c,mines);
     redoCells(r,c);
     countMines(r,c);
     t=0;
     timecheck=0;//set timecheck to 0 to be able to start timing again
   
     stop=true;//to stop revealing to continue onto next game
   }
}
      if(!stop)//if revealing will be done for the first time
      {
        if(current->right!=NULL )
        {
              if(current->right->isFlagged())
              count++;             
        }
         if(current->left!=NULL)
        {
              if(current->left->isFlagged())
              count++;             
        }
         if(current->up!=NULL)
        {
              if(current->up->isFlagged())
              count++;             
        }
         if(current->down!=NULL)
        {
              if(current->down->isFlagged())
              count++;             
        }
         if(current->upleft!=NULL)
        {
              if(current->upleft->isFlagged())
              count++;             
        }
         if(current->upright!=NULL)
        {
              if(current->upright->isFlagged())
              count++;             
        }
         if(current->downright!=NULL)
        {
              if(current->downright->isFlagged())
              count++;             
        }
         if(current->downleft!=NULL)
        {
              if(current->downleft->isFlagged())
              count++;             
        }
        if(value==count && middle==0)//if flags positioned match the value and no recursion at the time
        {
        middle=1;//switch to prevent multilevel recursion
                 if(current->up!=NULL&&current->up->getState()==false&&stop==false)
                 reveal(current->up->cell,current->up);
                 if(current->down!=NULL&&current->down->getState()==false&&stop==false)
                 reveal(current->down->cell,current->down);
                 if(current->right!=NULL&&current->right->getState()==false&&stop==false)
                 reveal(current->right->cell,current->right);
                 if(current->left!=NULL&&current->left->getState()==false&&stop==false)
                 reveal(current->left->cell,current->left);
                 if(current->downleft!=NULL&&current->downleft->getState()==false&&stop==false)
                 reveal(current->downleft->cell,current->downleft);
                 if(current->downright!=NULL&&current->downright->getState()==false&&stop==false)
                 reveal(current->downright->cell,current->downright);
                 if(current->upleft!=NULL&&current->upleft->getState()==false&&stop==false)
                 reveal(current->upleft->cell,current->upleft);
                 if(current->upright!=NULL&&current->upright->getState()==false&&stop==false)
                 reveal(current->upright->cell,current->upright);  
                                   
         middle=0;//switch closed                   
        }
        stop=false;//revealing is valid again
			}
		}
   }
}

void setBoard(int r, int c,int mines)//initializes the mines, r:no of rows, c:no of columns, mines: no of mines
{

  int counter=0;
  
  for(int i=0;i<r+2;i++)
  {
   for(int j=0;j<c+2;j++)
   {
       board[i][j]=false;    
   }        
  }
  
  
  while(counter<mines)//ensures there will exatcly #mines of mines
  {
   int x =rand()%r+1;//picks random positions for mines
   int y= rand()%c+1;
   
   if(!board[x][y])//ensures the number of mines
   {
                  
   board[x][y]=true; 
   counter++;  
   }  
                   
                  
  }

}
void initCells(int r,int c)//called once at the beginning to initialize cells and buttons according to level chosen. r:no of rows, c:no of columns
{
  
  for(int i=0; i<r; i++)
          {
               for(int j=0;j<c;j++)
               {
                       game[i][j]=Mine(board[i+1][j+1], (j*15)+1,(i*15)+1+PAD);//initializes game board with mine objects
                       game[i][j].cell=new Fl_Button(game[i][j].x,game[i][j].y,15,15,"");//creates buttons for the objects
                       game[i][j].cell->color(FL_GRAY);
		               game[i][j].setFlag(false);

		       if(board[i+1][j+1])
			 {
			   game[i][j].mined=true;//transfers mines' places
			 }
               }
               
           }

}
void countMines(int r, int  c) {	//count mines in the neighbors and sets that number to objects field numOfMines. r:no of rows, c:no of columns

  for(int i=1; i<r+1;i++) {
        for(int j=1; j<c+1; j++) {
                int mines=0;
                if(board[i+1][j])
                {
                 mines++;
                }
                if(board[i-1][j])
                {
                 mines++;
                }
                if(board[i][j+1])
                {
                 mines++;
                }
                if(board[i][j-1])
                {
                 mines++;
                }
                if(board[i+1][j+1])
                {
                 mines++;
                }
                if(board[i+1][j-1])
                {
                 mines++;
                }
                if(board[i-1][j+1])
                {
                 mines++;
                }
                if(board[i-1][j-1])
                {
                 mines++;
                }
                game[i-1][j-1].setNum(mines);
	
        }     
	}
}
void initNeighbors(int r, int c ) {			//initializes neighbor relations. r:no of rows, c:no of columns
	for(int i=0;i<r;i++) {
		for(int j=0; j<c;j++) {
			game[i][j].up=&game[i-1][j];
			game[i][j].upleft=&game[i-1][j-1];
			game[i][j].upright=&game[i-1][j+1];
			game[i][j].down=&game[i+1][j];
			game[i][j].downleft=&game[i+1][j-1];
			game[i][j].downright=&game[i+1][j+1];
			game[i][j].left=&game[i][j-1];
			game[i][j].right=&game[i][j+1];

		if(i==0) {
			game[i][j].up=NULL;
			game[i][j].upright=NULL;
			game[i][j].upleft=NULL;
	      }
	    if(j==0) {
			game[i][j].left=NULL;
			game[i][j].downleft=NULL;
			game[i][j].upleft=NULL;
	      }
	    if(j==c-1) {
			game[i][j].right=NULL;
			game[i][j].downright=NULL;
			game[i][j].upright=NULL;
	      }
			if(i==r-1) {
				game[i][j].down=NULL;
				game[i][j].downleft=NULL;
				game[i][j].downright=NULL;
			}
		}    
	}
}

void setCalls(int r, int c)	{		//initializes buttons for the game objects to recieve input for the mouse. r:no of rows, c:no of columns
	for(int i=0;i<r;i++) {
		for(int j=0; j<c;j++) {
			game[i][j].cell->callback(reveal,&game[i][j]);// sets the function reveal to the callback of the game buttons
		}
	}
}   

int main (int argc, char ** argv)
{

  srand((unsigned)time(0));//seeds the random function
    if(debug == false){

       r=9;
       c=9;
       mines=10;                           
       flagsleft=10;
       game = new Mine*[r];
	for(int i = 0; i < r; i++)
		game[i] = new Mine[c];
     board= new bool*[r+2];
 	for(int i = 0; i < r+2; i++)
		board[i] = new bool[c+2];        

      makeWindow(r,c,mines);
      setBoard(r,c,mines);
      initCells(r,c);
      countMines(r,c);
      initNeighbors(r,c);
      setCalls(r,c);
	  about->callback(info);
	  quit->callback(exit);
      new_game->callback(gen_new);
	  window->end ();
	  window->show();
	  fl = new Fl_Window(200 * 3, 200 * 3, 200 * 3, 200 * 3, "About");
  }
  else {

	  r = 9;
	  c = 9;
	  mines = 10;
	  flagsleft = 10;
	  game = new Mine*[r];
	  for (int i = 0; i < r; i++)
		 game[i] = new Mine[c];
	  board = new bool*[r + 2];
	  for (int i = 0; i < r + 2; i++)
	  board[i] = new bool[c + 2];
	  makeWindow(r, c, mines);
	  setBoard(r, c, mines);
	  initCells(r, c);
	  countMines(r, c);
	  initNeighbors(r, c);
	  setCalls(r, c);
	  new_game->callback(gen_new);
	  window->end();
	  window->show();
	  showMines();
  }
  return(Fl::run());
}