#include <iostream>
using namespace std;

class Sudokuboard {

    //board in double array form
    public:
        int b[9][9];

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
                    cout << " " << b[i][j] << " ";
            }
            cout << endl;
        }
    }

    //checks validity of board
    bool checkvalid() {
        int checks[9];
        int temp;
        // checks rows for 0s or duplicates
        for (int i=0;i<9;i++) {
            std::fill_n(checks,9, 0);
            for (int j=0;j<9;j++) {
                temp = b[i][j];
                if (temp == 0) {
                    return false;
                }
                else if (checks[temp - 1] == temp) {
                    return false;
                }
                checks[temp - 1] = temp;

            }
        }

        // checks columns for duplicates
        for (int i=0;i<9;i++) {
            std::fill_n(checks,9, 0);
            for (int j=0;j<9;j++) {
                temp = b[j][i];
                if (checks[temp - 1] == temp) {
                    return false;
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
                    temp = b[j][n];
                    if (checks[temp - 1] == temp) {
                    return false;
                    }
                    checks[temp - 1] = temp;
                }
            }
            //checks horizontal squares
            std::fill_n(checks,9,0);
            for (int j=i*3;j<(i+1)*3;j++) {
                for (int n=3;n<6;n++) {
                    temp = b[n][j];
                    if (checks[temp - 1] == temp) {
                    return false;
                    }
                    checks[temp - 1] = temp;
                }
            }
            
            std::fill_n(checks,9,0);
            for (int j=i*3;j<(i+1)*3;j++) {
                for (int n=6;n<9;n++) {
                    temp = b[j][n];
                    if (checks[temp - 1] == temp) {
                    return false;
                    }
                    checks[temp - 1] = temp;
                }
            }
        }
        return true;
    }

    //adds numbers to cells in the board
    void add(int row,int column, int num) {
        b[row][column] = num;
    }

    //deletes numbers from cells in the board
    void del(int row,int column) {
        b[row][column] = 0;
    }
};
