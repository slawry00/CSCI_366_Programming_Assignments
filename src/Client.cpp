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
#include "Client.hpp"

Client::~Client()
{
    board_name = "player_" + std::to_string(player) + ".action_board.json";
    remove(board_name.c_str());
}

void Client::initialize(unsigned int player, unsigned int board_size)
{
    if (player != 1 && player != 2)
    {
        throw ClientWrongPlayerNumberException();
    }
    else
    {
        this->player = player;
        this->board_size = board_size;
        this->board_name = "player_" + std::to_string(player) + ".action_board.json";
        std::ofstream action_board(this->board_name);
        vector<int> row(board_size, 0); // building a vector form of the board for cereal
        vector<vector<int>> board(board_size, row);
        { // useless scope with a local variable so the archive object is deconstructed because
            // "Some archives in cereal can only safely finish flushing their contents upon their destruction"
            cereal::JSONOutputArchive archive(action_board);
            archive(CEREAL_NVP(board));
        }
        action_board.close();
        this->initialized = true;
    }
}

vector<vector<int>> Client::load_board()
{
    vector<vector<int>> board;
    std::ifstream board_stream(this->board_name);
    {
        cereal::JSONInputArchive archive(board_stream);
        archive(board);
    }
    board_stream.close();
    return board;
}

void Client::fire(unsigned int x, unsigned int y)
{
    std::ofstream fire_board("player_" + std::to_string(this->player) + ".shot.json");
    {
        cereal::JSONOutputArchive archive(fire_board);
        archive(CEREAL_NVP(x), CEREAL_NVP(y));
    }
    fire_board.close();
}

// I chose to implement by checking when the shot file no longer exists to avoid a race condition
// which happens when you check for the result file existing before it is written to
// for good measure, I also check for the result file existing
bool Client::result_available()
{
    std::ifstream shot_stream("player_" + std::to_string(this->player) + ".shot.json");
    std::ifstream result_stream("player_" + std::to_string(this->player) + ".result.json");
    bool shot_available = (bool) shot_stream;
    bool result_available = (bool) result_stream;
    shot_stream.close();
    result_stream.close();
    return !shot_available && result_available;
}

int Client::get_result()
{
    int result;
    string result_file_name = "player_" + std::to_string(this->player) + ".result.json";
    std::ifstream result_stream2(result_file_name);
    {
        cereal::JSONInputArchive archive(result_stream2);
        archive(result);
    }
    result_stream2.close();
    remove(result_file_name.c_str());
    if (result < -1 || result > 1)
        throw ClientException("Bad Result");
    return result;
}

void Client::update_action_board(int result, unsigned int x, unsigned int y)
{
    vector<vector<int>> board = load_board();
    board[x][y] = result;
    std::ofstream action_board(this->board_name);
    {
        cereal::JSONOutputArchive archive(action_board);
        archive(CEREAL_NVP(board));
    }
    action_board.close();
}


string Client::render_action_board()
{
    vector<vector<int>> board = load_board();
    string board_string = "";
    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board[i].size(); j++)
        {
            if (board[i][j] == 0)
                board_string += "_ ";
            else if (board[i][j] == -1)
                board_string += "o ";
            else
                board_string += ("x ");
        }
        board_string += "\n";
    }
    return board_string;

}
