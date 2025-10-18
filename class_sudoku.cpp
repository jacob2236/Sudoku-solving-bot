#include <iostream>
#include <functional>
using namespace std;

class SudokuBoard {

    //board in double array form. Row major order
    public:
        int board[9][9];
        int fitness = 0;
        int ascii0 = '0'; // integer value of the ascii character 0. Useful for converting the ascii digits to int digits
        SudokuBoard(string boardString){ //takes an 81 char string representing the board. '.' represents an empty space
            auto nextChar = boardString.begin(); // make an iterator to pass through the string representation of the board
            for(int row = 0; row < 9; row++) {
                for(int col = 0; col < 9; col++) {
                    if(*nextChar == '.') {
                        board[row][col] = 0;
                    } else{
                        board[row][col] = *nextChar - ascii0; // subtract
                    }
                    nextChar++;

                    
                }
            }
        }
        SudokuBoard(){} //no arg constructor to let the current test file continue working but I think I'd rather make things more explicit later

    //prints board
    void print() {
        for (int i = 0; i < 9; i++) {
            if ((i)%3 == 0 && i != 0) {
                    cout << " -  -  -   -  -  -   -  -  - \n";
                }
            for (int j = 0; j < 9; j++) {
                if ((j)%3 == 0 && j != 0) {
                    cout << "|";
                }
                    cout << " " << board[i][j] << " ";
            }
            cout << endl;
        }
    }

    //checks validity of board and assigns fitness value to board
    bool checkValid() {
        int checks[9];
        int temp;
        int fit = 0;
        bool valid = true;
        // checks rows for 0s or duplicates
        for (int i=0;i<9;i++) {
            std::fill_n(checks,9, 0);
            for (int j=0;j<9;j++) {
                temp = board[i][j];
                if (temp == 0 || temp > 9) {
                    valid = false;
                    fit++;
                }
                else if (checks[temp - 1] == temp) {
                    valid = false;
                    fit++;
                }
                checks[temp - 1] = temp;

            }
        }

        // checks columns for duplicates
        for (int i=0;i<9;i++) {
            std::fill_n(checks,9, 0);
            for (int j=0;j<9;j++) {
                temp = board[j][i];
                if (checks[temp - 1] == temp) {
                    valid = false;
                    fit++;
                }
                checks[temp - 1] = temp;

            }
        }

        //checks squares for duplicates
        for (int i=0;i<3;i++) {
            std::fill_n(checks,9,0);
            //checks vertical squares
            for (int j=i*3;j<(i+1)*3;j++) {
                for (int n=0;n<3;n++) {
                    temp = board[j][n];
                    if (checks[temp - 1] == temp) {
                    valid = false;
                    fit++;
                    }
                    checks[temp - 1] = temp;
                }
            }
            //checks horizontal squares
            std::fill_n(checks,9,0);
            for (int j=i*3;j<(i+1)*3;j++) {
                for (int n=3;n<6;n++) {
                    temp = board[n][j];
                    if (checks[temp - 1] == temp) {
                    valid = false;
                    fit++;
                    }
                    checks[temp - 1] = temp;
                }
            }
            
            std::fill_n(checks,9,0);
            for (int j=i*3;j<(i+1)*3;j++) {
                for (int n=6;n<9;n++) {
                    temp = board[j][n];
                    if (checks[temp - 1] == temp) {
                    valid = false;
                    fit++;
                    }
                    checks[temp - 1] = temp;
                }
            }
        }
        fitness = 243 - fit;
        return valid;
    }

    bool checkValidCell(int row, int col) {
        // check if a specific cell has any duplicates/is valid in its position
        int cellValue = board[row][col];
        if (cellValue == 0) {
            return false; // an unfilled cell is not valid
        }

        for(int i = 0; i < 9; i++) {
            if(i != col && board[row][i] == cellValue) {
                return false; //another instance of the cell's value in the row
            }
            if(i != row && board[i][col] == cellValue) {
                return false; //another instance in the col
            }

        }
        int top = (row / 3) * 3; //get the top left corner of the group the cell is in
        int left = (col / 3) * 3;
        for(int y = 0; y < 3; y++) {
            for(int x = 0; x < 3; x++) {
                if(row != top + y && col != left + x && board[top + y][left + x] == cellValue) {
                    return false;
                }
            }
        }
        return true;

    }

    //adds numbers to cells in the board
    void add(int row,int column, int num) {
        board[row][column] = num;
    }

    //deletes numbers from cells in the board
    void del(int row,int column) {
        board[row][column] = 0;
    }

    int get(int row, int col){
        return board[row][col];
    }

    /* Ok. So. C++ doesn't like returning arrays, which means I can't do the original methodology for the forEach functions
     Originally they'd return an array of the return values of each function call (in the case of forEachCell a 9x9 array).
     Now, I'm going to be lazy and expect any function passed in to be void and to handle it's "returning" through side effects.
     This is, frankly, terrible programming. Relying on side-effects is never good, but the best part of bad programming is it still works.
     I just have to trust neither of us does something even dumber with this.
     */
    void forEachCell(function<void(int)> func) {
        for(int row = 0; row < 9; row++) {
            for(int col = 0; col < 9; col++) {
                func(board[row][col]);
            }
        }
    }

    void forEachRow(function<void(int[9])> func) {
        for(int row = 0; row < 9; row++){
            func(board[row]);
        }
    }

    void forEachCol(function<void(int[9])> func) {
        for(int col = 0; col < 9; col++) {
            int column[9];
            // for parity/consistency, make each column its own array to pass
            for(int row = 0; row < 9; row++) {
                column[row] = board[row][col];
            }
            func(column);
        }
    }

    void forEachGroup(function<void(int[9])> func) {
        for(int top = 0; top < 9; top += 3){
            for(int left = 0; left < 9; left += 3){
                // Collapse each group into a nine-item array reading left to right top to bottom
                int group[9];
                int index = 0;
                for(int row; row < 3; row++) {
                    for(int col; col < 3; col++) {
                        group[index] = board[top+row][left+col];
                        index++;
                    }
                }
                func(group);
            }
        }
    }



};
