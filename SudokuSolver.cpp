#include <iostream>
#include <algorithm>
#include <vector>

#define boardSize 9
using namespace std;

class Sudoku
{
    //Matrices to check numbers already present in a row/column/3*3 grid
    bool rowChecker[9][9] = {};
    bool columnChecker[9][9] = {};
    bool boxChecker[9][9] = {};

    //Check if the number is valid for its position
    bool validateNumber(int number, int row, int column)
    {
        if (!rowChecker[row][number] && !columnChecker[column][number] &&
            !boxChecker[(row / 3) * 3 + column / 3][number])
            return true;
        else
            return false;
    }

    //Find the solution for cell recursively using depth first search algorithm
    bool solutionFinder(vector<vector<int>> &sudokuBoard,
                        vector<pair<int, int>> &solutionsForIndex, int position)
    {
        // When the solution for every empty cell is found
        if (position >= solutionsForIndex.size())
            return true;

        int boardRow = solutionsForIndex[position].second / 9,
            boardColumn = solutionsForIndex[position].second % 9;

        //Find solutions from 1-9 for the cell whose position is indicated by boardRow & boardCOlumn
        for (int num = 0; num < 9; num++)
        {
            if (validateNumber(num, boardRow, boardColumn))
            {
                // If 'num' is a valid solution, add it to the empty cell in the sudoku board
                sudokuBoard[boardRow][boardColumn] = num + 1;

                /* As a result, 'num' does not remain as a possoble solution 
                for the concerned row/column/3*3 grid */
                rowChecker[boardRow][num] = true;
                columnChecker[boardColumn][num] = true;
                boxChecker[(boardRow / 3) * 3 + boardColumn / 3][num] = true;

                //recursively call the solutionFinder function for the remaining empty cells
                if (solutionFinder(sudokuBoard, solutionsForIndex, position + 1))
                    return true;

                //If false the solutionFinder function return false, backtrack and start again
                rowChecker[boardRow][num] = false;
                columnChecker[boardColumn][num] = false;
                boxChecker[(boardRow / 3) * 3 + boardColumn / 3][num] = false;
            }
        }
        return false;
    }

public:
    bool sudokuSolver(vector<vector<int>> &sudokuBoard)
    {
        //Update the checker matrices with information of numbers already present
        //To ensure non-repeating numbers in the range 1-9 in every row/column/3*3 grid

        int number;
        for (int row = 0; row < boardSize; row++)
        {
            for (int column = 0; column < boardSize; column++)
            {
                number = sudokuBoard[row][column] - 1;
                if (sudokuBoard[row][column] != 0)
                {
                    //If the number invalidates the constraints, exit loop and return
                    if (validateNumber(number, row, column))
                    {
                        rowChecker[row][number] = true;
                        columnChecker[column][number] = true;
                        boxChecker[(row / 3) * 3 + column / 3][number] = true;
                    }
                    else
                        return false;
                }
            }
        }

        vector<pair<int, int>> solutionsForIndex;
        int element;
        for (int row = 0; row < boardSize; row++)
        {
            for (int column = 0; column < boardSize; column++)
            {
                if (sudokuBoard[row][column] == 0)
                {
                    element = 0;
                    //Check the number of possibilities for each empty cell in the Sudoku board
                    /*If a number is present in the restricted row/column/3*3 grid 
                    then it is not a possibility for the corresponding cell*/
                    for (int probableNumber = 0; probableNumber < 9; probableNumber++)
                    {
                        element += !rowChecker[row][probableNumber] && !columnChecker[column][probableNumber] && !boxChecker[(row / 3) * 3 + column / 3][probableNumber];
                    }
                    solutionsForIndex.push_back({element, row * 9 + column});
                }
            }
        }
        //Sort the empty grid positions based on the number of possible solutions it has
        sort(begin(solutionsForIndex), end(solutionsForIndex));

        /*Start finding solutions using a depth first search approach
        starting from the grid position having the least number of possible solutions*/
        return solutionFinder(sudokuBoard, solutionsForIndex, 0);
    }

    void printSolution(vector<vector<int>> &sudokuBoard)
    {
        cout << endl;
        cout << "The solution to the Sudoku is: " << endl
             << endl;

        for (int row = 0; row < boardSize; row++)
        {
            for (int column = 0; column < boardSize; column++)
                cout << sudokuBoard[row][column] << " ";
            cout << endl;
        }
    }
};

//Starting point - Main function
int main()
{
    std::vector<vector<int>> sudokuBoard;
    int rowElement;

    cout << "Enter the elements of your Sudoku board" << endl << endl;
    cout << "Enter elements(1 - 9) row-wise separated by a space" << endl;
    cout << "***For blank spaces in your board, add zeroes***" << endl;

    /*Take input for the sudoku board from the user
    Start filling the sudoku board row-wise*/
    for (int row = 0; row < boardSize; row++)
    {
        vector<int> rowVector;
        cout << "Enter 9 elements of " << row + 1 << " row:" << endl;
        for (int column = 0; column < boardSize; column++)
        {
            try
            {
                cin >> rowElement;
                if (rowElement >= 0 && rowElement <= 9)
                    rowVector.push_back(rowElement);
                else
                {
                    throw(rowElement);
                }
            }
            //If the user enters an invalid number, exit the program
            catch (int rowElement)
            {
                cout << endl
                     << "The number entered is invalid";

                return 0;
            }
        }
        //add the filled rows to the sudoku board
        sudokuBoard.push_back(rowVector);
    }

    //Sample Sudoku puzzle to test the code
    // std::vector<vector<int>> sudokuBoard{
    //                             {0, 4, 0, 0, 2, 7, 0, 3, 1},
    //                             {0, 0, 0, 8, 0, 0, 0, 0, 0},
    //                             {1, 3, 0, 0, 0, 4, 0, 0, 0},
    //                             {3, 2, 0, 0, 7, 0, 4, 0, 0},
    //                             {0, 5, 0, 0, 0, 0, 0, 1, 8},
    //                             {0, 0, 0, 0, 0, 0, 0, 5, 0},
    //                             {6, 0, 0, 9, 8, 0, 0, 0, 0},
    //                             {0, 0, 0, 0, 0, 0, 0, 0, 5},
    //                             {7, 0, 0, 0, 0, 0, 0, 6, 0}};

    Sudoku puzzle;

    if (!puzzle.sudokuSolver(sudokuBoard))
    {
        cout << endl
             << "The board entered is invalid";
             
        return 0;
    }
    else
    {
        puzzle.printSolution(sudokuBoard);
    }

    return 0;
}