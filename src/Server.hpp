//    Battleship game assignment for MSU CSCI 366
//    Copyright (C) 2020    Mike P. Wittie
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.


#include <stdio.h>
#include <memory.h>
#include <iostream>
#include <fstream>
#include <exception>
#include <cstdio>

#include <cereal/archives/json.hpp>

#define SHOT_FILE_PROCESSED 0
#define NO_SHOT_FILE 1

#define SHIPS "CBRSD" //Carrier, Battleship, cRuiser, Submarine, and Destroyer

using namespace std;


class ServerException: public exception
{
private:
   char *cstr;

public:
   ServerException(string message){
      cstr = new char[message.size() + 1];
      message.copy(cstr, message.size() + 1);
      cstr[message.size()] = '\0';
   }

   ~ServerException(){
      delete cstr;
   }

   virtual const char* what() const throw(){
      return cstr;
   }
};

class Server{
public:
   int board_size;

private:
   string p1_setup_board;
   string p2_setup_board;


public:
   /** To read a board while testing and debugging
    *
    * @param filename - the name of the file to print the contents from
    */
    void check_board(string filename);

   /**
    * Performs Server initialization
    *
    * Need to do initialization outside of the constructor, so that the object may be initialized inside the
    * googletest SetUp() method.
    * The method opens player setup boards containing ship positions and checks the size of boards compared to
    * board_size parameter.
    * @param board_size - the size of a square board
    * @param p1_setup_board - file name of player 1's board
    * @param p2_setup_board - file name of player 2's board
    */
   void initialize(unsigned int board_size,
                   string p1_setup_board,
                   string p2_setup_board);

   /**
    * Checks the coordinates of a shot against setup board of player
    *
    * Check that player number within bounds, checks that shot coordinates within bounds, determines if the shot
    * results in a HIT, or a MISS.
    * @param player - player number
    * @param x - coordinate
    * @param y - coordinate
    * @return returns shot result as either HIT, MISS, or OUT_OF_BOUNDS
    */
   int evaluate_shot(unsigned int player, unsigned int x, unsigned int y);

   /**
    * Processes a shot issued by player
    *
    * Gets the shot from player, extracts coordinates, passes the information of evaluate_shot, and writes the result
    * into player_#.result.json.
    * @param player - player number
    * @return returns SHOT_PROCESSED, or NO_SHOT_FILE if nothing to process
    */
   int process_shot(unsigned int player);
};
