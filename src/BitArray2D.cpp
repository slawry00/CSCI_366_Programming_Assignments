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

#include <math.h>
#include "BitArray2D.hpp"

BitArray2D::BitArray2D(unsigned int rows, unsigned int columns)
{
    this->rows = rows;
    this->columns = columns;
    int array_length = rows*columns/8; // 8 bits in each char which will hold our bit array
    if (rows*columns%8 != 0)
        array_length++;
    this->array = new char[array_length](); // using 0 value initialization "()" at the end
}


BitArray2D::~BitArray2D()
{
    delete[] array;
}


bool BitArray2D::get(unsigned int row, unsigned int column)
{// check array bounds
    if (row >= this->rows)
    {
        throw BitArray2DException("BitArray row index out of bounds");
    }
    else if (column >= this->columns)
    {
        throw BitArray2DException("BitArray column index out of bounds");
    }

   // get the element
    return get_bit_elem(array, columns, row, column);
}



void BitArray2D::set(unsigned int row, unsigned int column)
{// check array bounds
    if (row >= this->rows)
    {
        throw BitArray2DException("BitArray row index out of bounds");
    }
    else if (column >= this->columns)
    {
        throw BitArray2DException("BitArray column index out of bounds");
    }

   // set the element
   set_bit_elem(array, columns, row, column);
}
