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


#include "common.hpp"
#include "Server.hpp"

/**
 * Calculate the length of a file (helper function)
 *
 * @param file - the name of a file whose length we want to query
 * @return length of the file in bytes
 */
int get_file_length(string file)
{
    std::ifstream file_stream(file, std::ios::binary | std::ios::ate); // opens file and seeks to end
    int bytes = file_stream.tellg(); // tells the byte position (how many from the start of the file)
    file_stream.close();
    return bytes;
}


void Server::initialize(unsigned int board_size,
                        string p1_setup_board,
                        string p2_setup_board)
{
    int board_1_bytes = get_file_length(p1_setup_board);
    int board_2_bytes = get_file_length(p2_setup_board);
    int calc_bytes = board_size*(board_size+1); // for the newlines at the end
    if (calc_bytes != board_1_bytes || calc_bytes != board_2_bytes)
    {
        string mess = "setup board(s) not correctly formatted\n"
                      "the size of the files were: \n\t board1: " + to_string(board_1_bytes) + "\n\t board2: "
                      + to_string(board_2_bytes) + "\nExpected a board size of: " + to_string(board_size) + "x"
                      + to_string(board_size) + " with " + to_string(calc_bytes) + " bytes";
        throw ServerException(mess);
    }
    else if (p1_setup_board == p2_setup_board)
    {
        throw ServerException("Player files are the same");
    }
    this->board_size = board_size;
    this->p1_setup_board = scan_setup_board(p1_setup_board);
    this->p2_setup_board = scan_setup_board(p2_setup_board);
}

void Server::check_board(string filename)
{
    string myText;
    std::ifstream file_stream(filename);
    if (file_stream)
    {
        while (getline(file_stream, myText)) {
            cout << myText << endl;
        }
        file_stream.close();
    }
}

int Server::evaluate_shot(unsigned int player, unsigned int x, unsigned int y)
{
    if (player != 1 && player != 2)
    {
        throw ServerException("Player can only be 1 or 2");
    }
    if (x >= board_size || x < 0 || y >= board_size || y < 0)
    {
        return OUT_OF_BOUNDS;
    }
    BitArray2D* opp_board;
    if (player == 1)
        opp_board = p2_setup_board;
    else
        opp_board = p1_setup_board;
    if (opp_board->get(y, x))
    {
        return HIT;
    }
    return MISS;
}
Server::~Server()
{
    delete this->p1_setup_board;
    delete this->p2_setup_board;
}


BitArray2D* Server::scan_setup_board(string setup_board_name)
{
    BitArray2D* board = new BitArray2D(this->board_size, this->board_size);
    char c;
    int col = 0;
    int row = 0;
    std::ifstream board_stream(setup_board_name);
    while (board_stream.get(c))
    {
        if (c != '\n')
        {
            if (c != '_')
            {
                board->set(row, col);
            }
            col++;
        }
        else
        {
            row++;
            col = 0;
        }
    }
    return board;

}

int Server::process_shot(unsigned int player)
{
    if (player != 1 && player != 2)
    {
        throw ServerException("Player can only be 1 or 2");
    }
    string shot_file_name = "player_" + std::to_string(player) + ".shot.json";
    std::ifstream shot_stream(shot_file_name);
    // To be extra careful of race conditions, I'm making sure the result file was deleted already
    string result_file_name = "player_" + std::to_string(player) + ".result.json";
    std::ifstream check_result_stream(result_file_name);
    if (shot_stream && !check_result_stream)
    {
        check_result_stream.close();
        int x, y;
        {
            cereal::JSONInputArchive in_archive(shot_stream);
            in_archive(x, y);
        }
        shot_stream.close();
        int result = evaluate_shot(player, x, y);
        std::ofstream result_stream("player_" + std::to_string(player) + ".result.json");
        {
            cereal::JSONOutputArchive out_archive(result_stream);
            out_archive(CEREAL_NVP(result));
        }
        result_stream.close();
        remove(shot_file_name.c_str());
        check_board("player_" + std::to_string(player) + ".result.json");
        return SHOT_FILE_PROCESSED;
    }
    return NO_SHOT_FILE;
}
