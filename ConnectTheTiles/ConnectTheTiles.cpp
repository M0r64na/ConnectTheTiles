#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <iomanip>

unsigned const MIN_ROWS_COLS = 5, MAX_ROWS_COLS = 20;
unsigned const MIN_TILE_TYPES = 8, MAX_TILE_TYPES = 20;
unsigned const WHITE_COLOR_ATTRIBUTE = 7;
unsigned const ROW_TO_PLACE_TILES_SIZE = 8;

bool isSymbolValid(char symbol)
{
    return symbol != ' ' && symbol != '\n';
}

bool isSymbolPresent(size_t tileTypes, char symbol, char* symbolsForTilesByType)
{
    bool isPresent = false;

    for (int i = 0; i < tileTypes; ++i)
    {
        if (symbolsForTilesByType[i] != symbol) continue;

        isPresent = true;
        break;
    }

    return isPresent;
}

unsigned getSymbolColor(size_t tileTypes, char* symbolsForTilesByType, char currentSymbol)
{
    if (currentSymbol == ' ') return WHITE_COLOR_ATTRIBUTE;

    unsigned currentColorAttribute = 0;

    for (int i = 0; i < tileTypes; ++i)
    {
        if (symbolsForTilesByType[i] != currentSymbol) continue;

        currentColorAttribute = i + 1;
        break;
    }

    return currentColorAttribute;
}

void printHorizontalBoarderOfTable(size_t cols)
{
    std::cout << "+";
    for (int i = 0; i < cols; ++i)
    {
        std::cout << "---+";
    }
    std::cout << std::endl;
}

void displayLayerOnConsoleUsingColors(size_t rows, size_t cols, size_t tileTypes, char* symbolsForTilesByType, char** layer)
{
    printHorizontalBoarderOfTable(cols);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            char currentSymbol = layer[i][j];

            int currentColorAttribute = getSymbolColor(tileTypes, symbolsForTilesByType, currentSymbol);
            SetConsoleTextAttribute(hConsole, currentColorAttribute);

            std::cout << "|" << std::setw(2) << currentSymbol << " ";
        }
        std::cout << "|" << std::endl;

        printHorizontalBoarderOfTable(cols);
    }
    std::cout << std::endl;

    SetConsoleTextAttribute(hConsole, WHITE_COLOR_ATTRIBUTE);
}

int getFirstEmptyPosition(char rowToPlaceSelectedTiles[ROW_TO_PLACE_TILES_SIZE])
{
    for (int i = 0; i < ROW_TO_PLACE_TILES_SIZE; ++i)
    {
        if (rowToPlaceSelectedTiles[i] != ' ') continue;

        return i;
    }

    return -1;
}

bool isDefeatPresent(char rowToPlaceSelectedTiles[ROW_TO_PLACE_TILES_SIZE])
{
    int firstEmptyPosition = getFirstEmptyPosition(rowToPlaceSelectedTiles);

    if (firstEmptyPosition == -1) return true;
    return false;
}

bool isWinPresent(unsigned allTilesInLayer)
{
    return allTilesInLayer == 0;
}

bool areThereTilesToRemove(char& tile, char rowToPlaceSelectedTiles[ROW_TO_PLACE_TILES_SIZE])
{
    for (int i = 0; i < ROW_TO_PLACE_TILES_SIZE - 1; ++i)
    {
        char currentTile = rowToPlaceSelectedTiles[i];
        if (currentTile == ' ') continue;

        unsigned counter = 1;

        for (int j = i + 1; j < ROW_TO_PLACE_TILES_SIZE; ++j)
        {
            if (rowToPlaceSelectedTiles[j] != currentTile) continue;

            ++counter;
        }

        if (counter == 3)
        {
            tile = currentTile;
            return true;
        }
    }

    tile = ' ';
    return false;
}

void removeTileFromRowWithSelectedTiles(char tile, char(&rowToPlaceSelectedTiles)[ROW_TO_PLACE_TILES_SIZE])
{
    for (int i = 0; i < ROW_TO_PLACE_TILES_SIZE; ++i)
    {
        if (rowToPlaceSelectedTiles[i] != tile) continue;

        for (int j = i; j < ROW_TO_PLACE_TILES_SIZE - 1; ++j)
        {
            rowToPlaceSelectedTiles[j] = rowToPlaceSelectedTiles[j + 1];
        }
        
        --i;
    }
}

void displayRowToPlaceSelectedTilesOnConsole(char rowToPlaceSelectedTiles[ROW_TO_PLACE_TILES_SIZE])
{
    printHorizontalBoarderOfTable(ROW_TO_PLACE_TILES_SIZE);

    for (int i = 0; i < ROW_TO_PLACE_TILES_SIZE; ++i)
    {
        std::cout << "|" << std::setw(2) << rowToPlaceSelectedTiles[i] << " ";
    }
    std::cout << "|" << std::endl;

    printHorizontalBoarderOfTable(ROW_TO_PLACE_TILES_SIZE);

    std::cout << std::endl;
}

char** createLevel(size_t& currentRows, size_t& currentCols, size_t& currentTileTypes, 
    char*& currentSymbolsForTilesByType, unsigned& allTilesInLayer)
{
    size_t rows, cols, tileTypes;

    // validate input
    do
    {
        std::cout << "Enter number of rows in the range [5, 20]: ";
        std::cin >> rows;
    } while (rows < MIN_ROWS_COLS || rows > MAX_ROWS_COLS);
    currentRows = rows;

    do
    {
        std::cout << "Enter number of cols in the range [5, 20]: ";
        std::cin >> cols;
    } while (cols < MIN_ROWS_COLS || cols > MAX_ROWS_COLS);
    currentCols = cols;

    do
    {
        std::cout << "Enter number of tile types in the range [8, 20]: ";
        std::cin >> tileTypes;
    } while (tileTypes < MIN_TILE_TYPES || tileTypes > MAX_TILE_TYPES || tileTypes * 3 >= rows * cols);  
    currentTileTypes = tileTypes;


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

        do
        {
            std::cout << "Enter symbol for current tile type: ";
            std::cin >> symbol;
        } while (!isSymbolValid(symbol) || isSymbolPresent(tileTypes, symbol, symbolsForTilesByType));
        
        symbolsForTilesByType[i] = symbol;
    }
    delete[] currentSymbolsForTilesByType;
    currentSymbolsForTilesByType = symbolsForTilesByType;

    // generate number of tiles by type
    size_t* numbersOfTilesByType = new size_t[tileTypes];
    srand(time(0));
    for (int i = 0; i < tileTypes; ++i)
    {
        size_t number;
        number = ((rand() % ((rows * cols) / tileTypes)) / 3 + 1) * 3;

        numbersOfTilesByType[i] = number;
        allTilesInLayer += number;
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

    delete[] numbersOfTilesByType;

    return layer;
}

void playAtLevel(size_t rows, size_t cols, size_t tileTypes,
    char* symbolsForTilesByType, unsigned& allTilesInLayer, 
    char**& layer, char (&rowToPlaceSelectedTiles)[ROW_TO_PLACE_TILES_SIZE])
{
    unsigned row, col;
    char tile;
    int position;
    bool result;

    while (true)
    {
        if (isWinPresent(allTilesInLayer))
        {
            std::cout << "CoNgRAtUlAtIOnS! You won :)" << std::endl;
            break;
        }

        if (isDefeatPresent(rowToPlaceSelectedTiles))
        {
            std::cout << "DEFEAT!" << std::endl;
            break;
        }

        std::cout << "Enter row and col of tile you want to select: ";
        std::cin >> row >> col;

        while ((row < 0 || row >= rows) ||
            (col < 0 || col >= cols) ||
            layer[row][col] == ' ')
        {
            std::cout << "Invalid or empty position." << std::endl;

            std::cout << "Enter row and col of tile you want to select: ";
            std::cin >> row >> col;
        }

        tile = layer[row][col];
        layer[row][col] = ' ';

        --allTilesInLayer;

        position = getFirstEmptyPosition(rowToPlaceSelectedTiles);
        rowToPlaceSelectedTiles[position] = tile;

        displayLayerOnConsoleUsingColors(rows, cols, tileTypes, symbolsForTilesByType, layer);
        displayRowToPlaceSelectedTilesOnConsole(rowToPlaceSelectedTiles);

        result = areThereTilesToRemove(tile, rowToPlaceSelectedTiles);
        if (result)
        {
            removeTileFromRowWithSelectedTiles(tile, rowToPlaceSelectedTiles);
            displayRowToPlaceSelectedTilesOnConsole(rowToPlaceSelectedTiles);
        }
    }
}

int main()
{
    size_t rows, cols, tileTypes;
    char* symbolsForTilesByType = new char[MAX_TILE_TYPES];
    unsigned allTilesInLayer = 0;

    char** layer = createLevel(rows, cols, tileTypes, symbolsForTilesByType, allTilesInLayer);
    displayLayerOnConsoleUsingColors(rows, cols, tileTypes, symbolsForTilesByType, layer);

    char rowToPlaceSelectedTiles[ROW_TO_PLACE_TILES_SIZE];
    std::fill_n(rowToPlaceSelectedTiles, ROW_TO_PLACE_TILES_SIZE, ' ');
    displayRowToPlaceSelectedTilesOnConsole(rowToPlaceSelectedTiles);

    playAtLevel(rows, cols, tileTypes, symbolsForTilesByType, allTilesInLayer, layer, rowToPlaceSelectedTiles);
    



    delete[] symbolsForTilesByType;

    for (int i = 0; i < rows; ++i)
    {
        delete[] layer[i];
    }
    delete[] layer;
}