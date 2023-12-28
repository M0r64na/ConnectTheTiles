#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <iomanip>
#include <chrono>

const unsigned MIN_ROWS_COLS = 5, MAX_ROWS_COLS = 20;
const unsigned MIN_TILE_TYPES = 8, MAX_TILE_TYPES = 20;
const unsigned WHITE_COLOR_ATTRIBUTE = 7;
const unsigned ROW_TO_PLACE_TILES_SIZE = 8;
const char EMPTY_POSITION = ' ';
const unsigned MIN_LEVELS_IN_GAME = 1, MAX_LEVELS_IN_GAME = 10;
const unsigned TOP_LEVEL_INDEX = 0;

bool isSymbolValid(char symbol)
{
    return symbol != EMPTY_POSITION && symbol != '\n';
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
    if (currentSymbol == EMPTY_POSITION) return WHITE_COLOR_ATTRIBUTE;

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

void displayGameOnConsoleUsingColors(size_t rows, size_t cols, size_t tileTypes, char* symbolsForTilesByType, char*** levels)
{
    printHorizontalBoarderOfTable(cols);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    char** currentLevel = levels[TOP_LEVEL_INDEX];

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            char currentSymbol = currentLevel[i][j];

            SetConsoleTextAttribute(hConsole, WHITE_COLOR_ATTRIBUTE);
            std::cout << "|";

            int currentColorAttribute = getSymbolColor(tileTypes, symbolsForTilesByType, currentSymbol);
            SetConsoleTextAttribute(hConsole, currentColorAttribute);

            std::cout << std::setw(2) << currentSymbol << " ";
        }
        SetConsoleTextAttribute(hConsole, WHITE_COLOR_ATTRIBUTE);
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
        if (rowToPlaceSelectedTiles[i] != EMPTY_POSITION) continue;

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

bool isWinPresent(unsigned visibleTiles)
{
    return visibleTiles == 0;
}

bool areThereTilesToRemove(char& tile, char rowToPlaceSelectedTiles[ROW_TO_PLACE_TILES_SIZE])
{
    for (int i = 0; i < ROW_TO_PLACE_TILES_SIZE - 1; ++i)
    {
        char currentTile = rowToPlaceSelectedTiles[i];
        if (currentTile == EMPTY_POSITION) continue;

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

    tile = EMPTY_POSITION;
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

void initializeRowInLevel(char* row, size_t cols)
{
    std::fill_n(row, cols, EMPTY_POSITION);
}

void initializeLevel(size_t rows, size_t cols, char**& level)
{
    for (int i = 0; i < rows; ++i)
    {
        initializeRowInLevel(level[i], cols);
    }
}

char** createAndInitializeLevel(size_t rows, size_t cols)
{
    char** level = new char* [rows];

    for (int i = 0; i < rows; ++i)
    {
        level[i] = new char[cols];
    }

    initializeLevel(rows, cols, level);

    return level;
}

void initializeRowToPlaceSelectedTiles(char(&rowToPlaceSelectedTiles)[ROW_TO_PLACE_TILES_SIZE])
{
    std::fill_n(rowToPlaceSelectedTiles, ROW_TO_PLACE_TILES_SIZE, EMPTY_POSITION);
}

size_t* generateNumberOfTilesByType(size_t rows, size_t cols, size_t tileTypes)
{
    size_t* numbersOfTilesByType = new size_t[tileTypes];

    for (int i = 0; i < tileTypes; ++i)
    {
        size_t number;
        number = ((rand() % ((rows * cols) / tileTypes)) / 3 + 1) * 3;

        numbersOfTilesByType[i] = number;
    }

    return numbersOfTilesByType;
}


void generateValidPositionForCurrentTile(unsigned& row, unsigned& col, size_t rows, size_t cols, char**& level)
{
    do
    {
        row = rand() % rows;
        col = rand() % cols;
    } while (level[row][col] != EMPTY_POSITION);
}

void placeAllTilesOnRandomPositions(size_t rows, size_t cols, size_t tileTypes,
    size_t* numbersOfTilesByType, char* symbolsForTilesByType, char**& level)
{
    for (int i = 0; i < tileTypes; ++i)
    {
        size_t currentNumberOfTilesByType = numbersOfTilesByType[i];
        char currentSymbolByTileType = symbolsForTilesByType[i];

        for (int j = 0; j < currentNumberOfTilesByType; ++j)
        {
            unsigned row;
            unsigned col;

            generateValidPositionForCurrentTile(row, col, rows, cols, level);

            level[row][col] = currentSymbolByTileType;
        }
    }
}

void readAndValidateInput(size_t& rows, size_t& cols, size_t& tileTypes, size_t& numberOfLevels)
{
    // validate rows
    std::cout << "Enter number of rows in the range [5, 20]: ";
    std::cin >> rows;

    while (rows < MIN_ROWS_COLS || rows > MAX_ROWS_COLS)
    {
        std::cout << "Invalid number of rows." << std::endl;

        std::cout << "Enter number of rows in the range [5, 20]: ";
        std::cin >> rows;
    }

    // validate cols
    std::cout << "Enter number of cols in the range [5, 20]: ";
    std::cin >> cols;

    while (cols < MIN_ROWS_COLS || cols > MAX_ROWS_COLS)
    {
        std::cout << "Invalid number of cols." << std::endl;

        std::cout << "Enter number of cols in the range [5, 20]: ";
        std::cin >> cols;
    }

    // validate tile types
    std::cout << "Enter number of tile types in the range [8, 20]: ";
    std::cin >> tileTypes;

    while (tileTypes < MIN_TILE_TYPES || tileTypes > MAX_TILE_TYPES || tileTypes * 3 >= rows * cols)
    {
        std::cout << "Invalid number of tile types." << std::endl;

        std::cout << "Enter number of tile types in the range [8, 20]: ";
        std::cin >> tileTypes;
    }

    // validate levels
    std::cout << "Enter number of levels in the range [1, 10]: ";
    std::cin >> numberOfLevels;

    while (numberOfLevels < MIN_LEVELS_IN_GAME || numberOfLevels > MAX_LEVELS_IN_GAME)
    {
        std::cout << "Invalid number of levels." << std::endl;

        std::cout << "Enter number of levels in the range [1, 10]: ";
        std::cin >> numberOfLevels;
    }
}

void validateSymbol(size_t tileTypes, char& symbol, char* symbolsForTilesByType)
{
    std::cout << "Enter symbol for current tile type: ";
    std::cin >> symbol;

    while (!isSymbolValid(symbol) || isSymbolPresent(tileTypes, symbol, symbolsForTilesByType))
    {
        std::cout << "Invalid symbol." << std::endl;

        std::cout << "Enter symbol for current tile type: ";
        std::cin >> symbol;
    }
}

char* readAndValidateSymbolsForTilesByType(size_t tileTypes)
{
    char* symbolsForTilesByType = new char[tileTypes];

    for (int i = 0; i < tileTypes; ++i)
    {
        char symbol;
        validateSymbol(tileTypes, symbol, symbolsForTilesByType);

        symbolsForTilesByType[i] = symbol;
    }

    return symbolsForTilesByType;
}

void fillCurrentEmptyPositionInCurrentLevel(size_t numbersOfLevels, int row, int col, unsigned& visibleTiles, char**& currentLevel, char***& levels)
{
    for (int k = TOP_LEVEL_INDEX + 1; k < numbersOfLevels; ++k)
    {
        if (levels[k][row][col] == EMPTY_POSITION) continue;

        currentLevel[row][col] = levels[k][row][col];
        levels[k][row][col] = EMPTY_POSITION;
        ++visibleTiles;

        break;
    }
}

void fillEmptyPositionsInCurrentLevel(size_t rows, size_t cols, size_t numberOfLevels, char***& levels, unsigned& visibleTiles)
{
    char** currentLevel = levels[TOP_LEVEL_INDEX];

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            if (currentLevel[i][j] != EMPTY_POSITION) continue;

            fillCurrentEmptyPositionInCurrentLevel(numberOfLevels, i, j, visibleTiles, currentLevel, levels);
        }
    }
}

void countInitialTilesInTopLayer(size_t tileTypes, unsigned& visibleTiles, size_t** numbersOfTilesByTypeByLevel)
{
    size_t* numbersOfTilesByType = numbersOfTilesByTypeByLevel[TOP_LEVEL_INDEX];

    for (int i = 0; i < tileTypes; ++i)
    {
        visibleTiles += numbersOfTilesByType[i];
    }
}

char** createLevel(size_t rows, size_t cols, size_t tileTypes, 
    char* symbolsForTilesByType, int currentLevelIndex, size_t**& numbersOfTilesByTypeByLevel)
{
    // create and initialize level
    char** level = createAndInitializeLevel(rows, cols);

    // generate number of tiles by type
    size_t* numbersOfTilesByType = generateNumberOfTilesByType(rows, cols, tileTypes);
    numbersOfTilesByTypeByLevel[currentLevelIndex] = numbersOfTilesByType;

    // generate position for each tile
    placeAllTilesOnRandomPositions(rows, cols, tileTypes, numbersOfTilesByType, symbolsForTilesByType, level);

    return level;
}

void restartGame(size_t rowsInLevel, size_t colsInLevel, size_t tileTypes, size_t numberOfLevels,
    size_t** numbersOfTilesByTypeByLevel, char* symbolsForTilesByType, unsigned& visibleTiles,
    char***& levels, char(&rowToPlaceSelectedTiles)[ROW_TO_PLACE_TILES_SIZE])
{
    for (int i = 0; i < numberOfLevels; ++i)
    {
        char** level = levels[i];

        initializeLevel(rowsInLevel, colsInLevel, level);
        placeAllTilesOnRandomPositions(rowsInLevel, colsInLevel, tileTypes, numbersOfTilesByTypeByLevel[i], symbolsForTilesByType, level);
    }

    fillEmptyPositionsInCurrentLevel(rowsInLevel, colsInLevel, numberOfLevels, levels, visibleTiles);
    displayGameOnConsoleUsingColors(rowsInLevel, colsInLevel, tileTypes, symbolsForTilesByType, levels);

    initializeRowToPlaceSelectedTiles(rowToPlaceSelectedTiles);
    displayRowToPlaceSelectedTilesOnConsole(rowToPlaceSelectedTiles);
}

void playGame(size_t rowsInLevel, size_t colsInLevel, size_t tileTypes, size_t numberOfLevels,
    size_t** numbersOfTilesByTypeByLevel, char* symbolsForTilesByType, 
    unsigned& visibleTiles,
    char***& levels, char (&rowToPlaceSelectedTiles)[ROW_TO_PLACE_TILES_SIZE])
{
    unsigned row, col;
    char tile;
    int position;
    bool result;

    char** currentLevel = levels[TOP_LEVEL_INDEX];

    while (true)
    {
        if (isWinPresent(visibleTiles))
        {
            std::cout << "CoNgRAtUlAtIOnS! You won :)" << std::endl;
            break;
        }

        if (isDefeatPresent(rowToPlaceSelectedTiles))
        {
            std::cout << "DEFEAT!" << std::endl;

            char answer;
            std::cout << "Do you want to restart the game? (Y / N)" << std::endl << ">> ";
            std::cin >> answer;

            if (answer == 'N') break;

            restartGame(rowsInLevel, colsInLevel, tileTypes, numberOfLevels,
                numbersOfTilesByTypeByLevel, symbolsForTilesByType,
                visibleTiles, levels, rowToPlaceSelectedTiles);
        }

        std::cout << "Enter row and col of tile you want to select: ";
        std::cin >> row >> col;

        while ((row < 0 || row >= rowsInLevel) ||
            (col < 0 || col >= colsInLevel) ||
            currentLevel[row][col] == EMPTY_POSITION)
        {
            std::cout << "Invalid or empty position." << std::endl;

            std::cout << "Enter row and col of tile you want to select: ";
            std::cin >> row >> col;
        }

        tile = currentLevel[row][col];
        currentLevel[row][col] = EMPTY_POSITION;

        --visibleTiles;

        position = getFirstEmptyPosition(rowToPlaceSelectedTiles);
        rowToPlaceSelectedTiles[position] = tile;

        fillEmptyPositionsInCurrentLevel(rowsInLevel, colsInLevel, numberOfLevels, levels, visibleTiles);

        displayGameOnConsoleUsingColors(rowsInLevel, colsInLevel, tileTypes, symbolsForTilesByType, levels);
        displayRowToPlaceSelectedTilesOnConsole(rowToPlaceSelectedTiles);

        result = areThereTilesToRemove(tile, rowToPlaceSelectedTiles);
        if (result)
        {
            removeTileFromRowWithSelectedTiles(tile, rowToPlaceSelectedTiles);
            displayRowToPlaceSelectedTilesOnConsole(rowToPlaceSelectedTiles);
        }
    }
}

char*** createGame(size_t& rowsInLevel, size_t& colsInLevel, size_t& tileTypes, size_t& numberOfLevels,
    unsigned& visibleTiles, char*& symbolsForTilesByType, size_t**& numbersOfTilesByTypeByLevel)
{
    readAndValidateInput(rowsInLevel, colsInLevel, tileTypes, numberOfLevels);

    delete[] symbolsForTilesByType;
    symbolsForTilesByType = readAndValidateSymbolsForTilesByType(tileTypes);

    char*** levels = new char** [numberOfLevels];

    delete[] numbersOfTilesByTypeByLevel;
    numbersOfTilesByTypeByLevel = new size_t * [numberOfLevels];

    for (int i = 0; i < numberOfLevels; ++i)
    {
        levels[i] = createLevel(rowsInLevel, colsInLevel, tileTypes, symbolsForTilesByType, i, numbersOfTilesByTypeByLevel);
        
        // DEBUG
        std::cout << "Layer index: " << i << std::endl;
        for (int j = 0; j < rowsInLevel; ++j)
        {
            for (int k = 0; k < colsInLevel; ++k)
            {
                std::cout << levels[i][j][k] << "\t";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    countInitialTilesInTopLayer(tileTypes, visibleTiles, numbersOfTilesByTypeByLevel);
    fillEmptyPositionsInCurrentLevel(rowsInLevel, colsInLevel, numberOfLevels, levels, visibleTiles);

    std::cout << "Visible tiles: " << visibleTiles << std::endl;

    displayGameOnConsoleUsingColors(rowsInLevel, colsInLevel, tileTypes, symbolsForTilesByType, levels);

    return levels;
}

int main()
{
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    srand((unsigned) seed);

    size_t rowsInLevel, colsInLevel, tileTypes, numberOfLevels;
    
    unsigned visibleTiles = 0;
    char* symbolsForTilesByType = new char[MAX_TILE_TYPES];
    size_t** numbersOfTilesByTypeByLevel = new size_t*[MAX_LEVELS_IN_GAME];

    char*** levels = createGame(rowsInLevel, colsInLevel, tileTypes, numberOfLevels, visibleTiles, symbolsForTilesByType, numbersOfTilesByTypeByLevel);

    char rowToPlaceSelectedTiles[ROW_TO_PLACE_TILES_SIZE];
    initializeRowToPlaceSelectedTiles(rowToPlaceSelectedTiles);
    displayRowToPlaceSelectedTilesOnConsole(rowToPlaceSelectedTiles);

    playGame(rowsInLevel, colsInLevel, tileTypes, numberOfLevels,
        numbersOfTilesByTypeByLevel, symbolsForTilesByType,
        visibleTiles, levels, rowToPlaceSelectedTiles);

    delete[] symbolsForTilesByType;

    for (int i = 0; i < numberOfLevels; ++i)
    {
        delete[] numbersOfTilesByTypeByLevel[i];
    }
    delete[] numbersOfTilesByTypeByLevel;

    for (int i = 0; i < numberOfLevels; ++i)
    {
        for (int j = 0; j < rowsInLevel; ++j)
        {
            delete[] levels[i][j];
        }
        delete[] levels[i];
    }
    delete[] levels;
}