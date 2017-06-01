/*
  Author: Jasmine Wong
  Course: CS135
  Instructor: Alexey Nikolaev
  Assignment: Project 2 Tournament 1

*/

#include <cstdlib>
#include <iostream>
#include <cmath>
#include "bot.h"
#include "screen.h"
#include <fstream>

using namespace std;

int ROWS;
int COLS;
int iter = 0;
char boardarray[35][35];
int hit_row;
int hit_cols;
int place;
int length = 1;
int add_row;

/* Initialization procedure, called when the game starts:

   init (rows, cols, num, screen, log) 
 
   Arguments:
   rows, cols = the boards size
   num        = the number of ships 
   screen     = a screen to update your knowledge about the game
   log        = a cout-like output stream
*/
void init(int rows, int cols, int num, Screen &screen, ostream &log) 
{
  ROWS = rows;
  COLS = cols;
  log << "Start." << endl;
}


/* The procedure handling each turn of the game:
 
   next_turn(sml, lrg, num, gun, screen, log)
   
   Arguments:
   sml, lrg = the sizes of the smallest and the largest ships that are currently alive
   num      = the number of ships that are currently alive
   gun      = a gun.
   Call gun.shoot(row, col) to shoot: 
   Can be shot only once per turn. 
   Returns MISS, HIT, HIT_N_SUNK, ALREADY_HIT, or ALREADY_SHOT.
   screen   = a screen to update your knowledge about the game
   log      = a cout-like output stream
*/

//res = gun.shoot(ROWS-r, COLS-c);


void next_turn(int sml, int lrg, int num, Gun &gun, Screen &screen, ostream &log)
{
  //implement a rnandom number generator for r and c
  //build a 2D array to store where the ship has been hit, & miss
  //if it hits a ship, check the stuff around it
  //if its sinks a ship, automatically mark surroundings as a miss

  if (place ==0){
    int r,c;
    do {
      r = rand() % ROWS;
      c = rand() % COLS;
      //iter +=1;        //iterates through each space on the board one at a time
    
    }
    while (boardarray[r][c] == 'x' && boardarray[r][c] == '@' && boardarray[r][c] == 'S'); //a random coordinate will be skipped over if it is already assigned an 'x' or '@' or 'S'
    result res = gun.shoot(r,c);
    if (res == MISS){
      screen.mark(r, c, 'x', BLUE);
      boardarray[r][c] = 'x';
  
    }
    else if (res == HIT){
      screen.mark(r, c, '@', GREEN);
      boardarray[r][c] = '@';
      hit_row = r;     
      hit_cols = c; 
      length = 1;
      if (hit_cols == 0){
	place = 2;
      }
      else {
	place = 1;
      }
      
    }
    else if (res == HIT_N_SUNK){
      screen.mark(r, c, 'S', RED);
      boardarray[r][c] = 'S';
      
    }
    log << "Smallest: " << sml << " Largest: " << lrg << ". ";
    log << "Shoot at " << r << " " << c << endl;
   
  }
  if (place == 1){ //checks the row below
      result res_1 = gun.shoot(hit_row+length, hit_cols);
      if (res_1 == MISS){
	screen.mark(hit_row + length, hit_cols, 'x', BLUE);
	boardarray[hit_row+length][hit_cols] = 'x';
	length = 1;
	if (hit_cols == COLS){
	  place = 4;
	}
	else if (hit_row == 0 || hit_cols == 0){
	  place = 3;
	}
	else{
	  place = 2;
	}
      }
      else if (res_1 == HIT){
	screen.mark(hit_row+length, hit_cols, '@', GREEN);
	boardarray[hit_row+length][hit_cols] = '@';
	length++;  //will continue checking the successive rows if they are hits
	if (length == 5){ //does not check over the max length of the ship which is 5
	  length = 0;
	  place = 0;
	}
      }
      else if (res_1 == HIT_N_SUNK){
	screen.mark(hit_row+length,hit_cols, 'S', RED);
	boardarray[hit_row+length][hit_cols] = 'S';
	place = 0;	//once a ship is found, the program will go back to iterating space by space
      }
      
      log << "Smallest: " << sml << " Largest: " << lrg << ". ";
      log << "Shoot at " << hit_row + length << " " << hit_cols << endl;
  }
  
  else if (place == 2 && hit_row-length >= 0){
    result res_2 = gun.shoot(hit_row-length, hit_cols);
    if (res_2 == MISS){
      screen.mark(hit_row - length, hit_cols, 'x', BLUE);
      boardarray[hit_row - length][hit_cols] = 'x';
      place = 3;
      length = 1;
    } 
    else if (res_2 == HIT){
      screen.mark(hit_row - length, hit_cols, '@', GREEN);
      boardarray[hit_row - length][hit_cols] = '@';
      length++;
      if (length == 5){
	length = 0;
	place = 0;
      }
    }
    else if (res_2 == HIT_N_SUNK){
      screen.mark(hit_row-length,hit_cols, 'S', RED);
      boardarray[hit_row -length][hit_cols] = 'S';
      place = 0;
    }
    log << "Smallest: " << sml << " Largest: " << lrg << ". ";
    log << "Shoot at " << hit_row-length << " " << hit_cols << endl;
  }
  
  else if (place == 3){ //checks the next column
    result res_3 = gun.shoot(hit_row, hit_cols + length);
    if (res_3 == MISS){
      screen.mark(hit_row, hit_cols+length, 'x', BLUE);
      boardarray[hit_row][hit_cols+length] = 'x';
      if (hit_cols == 0){
	place = 0;
      }
      else {
	place = 4;
      }
      length = 1; //if the space next to the column is also a miss, the program will go back to iterating space by space
    }
    else if (res_3 == HIT){
      screen.mark(hit_row, hit_cols+length, '@', GREEN);
      boardarray[hit_row][hit_cols+length] = '@';
      length++;
      if (length == 5){
	length = 0;
	place = 0;
      }
    }
    else if (res_3 == HIT_N_SUNK){
      screen.mark(hit_row,hit_cols+length, 'S', RED);
      boardarray[hit_row][hit_cols+length] = 'S';
      place = 0;
    }
    log << "Smallest: " << sml << " Largest: " << lrg << ". ";
    log << "Shoot at " << hit_row << " " << hit_cols+length << endl;
 }
  
  else if (place == 4){
    result res_4 = gun.shoot(hit_row, hit_cols - length);
    if (res_4 == MISS){
      screen.mark(hit_row, hit_cols-length, 'x', BLUE);
      boardarray[hit_row][hit_cols-length] = 'x';
      place = 0;
      length = 1;
    }
    else if (res_4 == HIT){
      screen.mark(hit_row, hit_cols-length, '@', GREEN);
      boardarray[hit_row][hit_cols-length] = '@';
      length++;
      if (length == 5){
	length = 0;
	place = 0;
      }
    }
    else if (res_4 == HIT_N_SUNK){
      screen.mark(hit_row,hit_cols-length, 'S', RED);
      boardarray[hit_row][hit_cols-length] = 'S';
      place = 0;
    }
    log << "Smallest: " << sml << " Largest: " << lrg << ". ";
    log << "Shoot at " << hit_row << " " << hit_cols-length << endl;
    }
}


/* new things implemented:
   once the program encounters a hit, it will check to the right and below it for continuous hits until a ship is found. the program does not check to the left or above because the iteration space by space automatically accomplishes that.  */   
      
      
		   
		   

     
  



