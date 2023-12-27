#include <iostream>
#include <cstdlib>
#include <windows.h>

void createLevel()
{
    unsigned const MIN_ROWS_COLS = 5, MAX_ROWS_COLS = 20;
    unsigned const MIN_TILE_TYPES = 8, MAX_TILE_TYPES = 20;

    size_t rows, cols, tileTypes;

    // validate input
    do
    {
        std::cout << "Enter number of rows in the range [5, 20]: ";
        std::cin >> rows;
    } while (rows < MIN_ROWS_COLS || rows > MAX_ROWS_COLS);

    do
    {
        std::cout << "Enter number of cols in the range [5, 20]: ";
        std::cin >> cols;
    } while (cols < MIN_ROWS_COLS || cols > MAX_ROWS_COLS);

    do
    {
        std::cout << "Enter number of tile types in the range [8, 20]: ";
        std::cin >> tileTypes;
    } while (tileTypes < MIN_TILE_TYPES || tileTypes > MAX_TILE_TYPES || tileTypes * 3 >= rows * cols);  

    // create and initialize layer
    char** layer = new char* [rows];
    for (int i = 0; i < rows; ++i)
    {
        layer[i] = new char[cols];
        std::fill_n(layer[i], cols, ' ');
    }

    // initialize symbols for tile types
    char* symbolsForTilesByType = new char[tileTypes];
    for (int i = 0; i < tileTypes; ++i)
    {
        char symbol;

        std::cout << "Enter symbol for current tile type: ";
        std::cin >> symbol;

        // check if the symbol is present in the array already => if so - enter again until valid
        symbolsForTilesByType[i] = symbol;
    }

    // generate number of tiles by type
    size_t* numbersOfTilesByType = new size_t[tileTypes];
    srand(time(0));
    for (int i = 0; i < tileTypes; ++i)
    {
        size_t number;
        number = ((rand() % ((rows * cols) / tileTypes)) / 3 + 1) * 3;

        numbersOfTilesByType[i] = number;
    }
    // FOR DEBUGGING: numbers of tiles by type
    for (int i = 0; i < tileTypes; ++i) std::cout << numbersOfTilesByType[i] << " ";
    std::cout << std::endl << std::endl;

    // generate position for each tile
    for (int i = 0; i < tileTypes; ++i)
    {
        size_t currentNumberOfTilesByType = numbersOfTilesByType[i];
        char currentSymbolByTileType = symbolsForTilesByType[i];

        for (int j = 0; j < currentNumberOfTilesByType; ++j)
        {
            unsigned row;
            unsigned col;

            do
            {
                row = rand() % rows;
                col = rand() % cols;
            } while (layer[row][col] != ' ');

            layer[row][col] = currentSymbolByTileType;
        }
    }

    // display layer on console
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            std::cout << layer[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main()
{
    createLevel();
}