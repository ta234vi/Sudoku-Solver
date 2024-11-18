#include<stdio.h>
#include<iostream>
#include<string>
#include<vector>
#include<array>
#include<tuple>

using std::cout;
using std::cin;
using std::string;
using std::to_string;
using std::vector;
using std::array;
using std::tuple;
using std::make_tuple;
using std::get;


array<array<int,9>,9> sudoku;
array<array<int,9>,9> temp;
bool solved = false;


int checkrow(int row,int num, array<array<int,9>,9> sudoku)
{
    int column;
    for(column = 0; column < 9; column ++)
        if(sudoku[row][column] == num)
            return 0;
    return 1;
}

int checkcolumn(int column, int num, array<array<int,9>,9> sudoku)
{
    int row;
    for(row = 0; row < 9; row ++)
        if(sudoku[row][column] == num)
            return 0;
    return 1;
}

int checkgrid(int grid_row, int grid_column, int num, array<array<int,9>,9> sudoku)
{
    int row, column;
    for(row = grid_row*3; row < (grid_row+1)*3; row ++)
        for(column = grid_column*3; column < (grid_column+1)*3; column++)
            if(sudoku[row][column] == num)
                return 0;
    return 1;
}

void draw(array<array<int,9>,9> sudoku)
{  
    int row, column;
    for(row = 0; row < 9; row ++)
    {
        for(column = 0; column < 9; column ++)
        {
            cout << to_string(sudoku[row][column]) + " ";
            if(column == 2 || column == 5)
                    cout << "| ";
        }
        cout << "\n";
        if(row == 2 || row == 5)
            cout << "---------------------\n";
    }
}

tuple<bool,array<array<vector<int>,9>,9>> alternatives(array<array<int,9>,9> sudoku)
{
    int row, column, n, counter;
    array<array<vector<int>,9>,9> alt;
    for(row = 0; row < 9; row ++)
        for(column = 0; column < 9; column ++)
            if(sudoku[row][column] == 0)
            {
                for(n = 1; n < 10; n ++)
                    if(checkrow(row,n,sudoku) && checkcolumn(column,n,sudoku) && checkgrid(row/3,column/3,n,sudoku))
                        alt[row][column].push_back(n);
                if(alt[row][column].empty())
                    return make_tuple(false,alt);
            }        
    return make_tuple(true,alt);
}

bool fill(array<array<int,9>,9> &sudoku, array<array<vector<int>,9>,9> alt)
{
    bool flag = false;
    for(int row = 0; row < 9; row ++)
        for(int column = 0; column < 9; column ++)
            if(alt[row][column].size() == 1 && alt[row][column][0]!=0)
            {
                sudoku[row][column] = alt[row][column][0];
                alt[row][column].clear();
                flag = true;
            }
    return flag;
}

tuple<int,int> best(array<array<vector<int>,9>,9> alt)
{
    for(int i = 2; i < 10; i ++)
        for(int row = 0; row < 9; row ++)
            for(int column = 0; column < 9; column ++)
                if(alt[row][column].size() == i)
                    return make_tuple(row,column);
    return make_tuple(-1,-1);
}

void solve(array<array<int,9>,9> state, array<array<vector<int>,9>,9> alt)
{
    if(solved)
        return;

    tuple<bool,array<array<vector<int>,9>,9>> val;

    while(fill(state, alt))
    {
        val = alternatives(state);
        if(!get<0>(val))
            return;
        else
            alt = get<1>(val);
    }

    tuple<int,int> pos = best(alt);
    int row = get<0>(pos);
    int column = get<1>(pos);

    if(row == -1 && column == -1)
    {
        if(!solved)
        {
            sudoku = state;
            solved = true;
        }
    }
    else
        for(int v : alt[row][column])
        {
            temp = state;
            temp[row][column] = v;
            val = alternatives(temp);
            if(!get<0>(val))
                continue;
            else
                solve(temp,get<1>(val));
        }
}

int main()
{
    int row, column;
    tuple<bool,array<array<vector<int>,9>,9>> val;

    string num;
    cout << "To see an example, press 'y'. To enter your own sudoku, press 'n'.\n";
    cin >> num;

    if(num == "y")
    {
        // easy
        // array<array<int,9>,9> d = {
        //                             0, 9, 3, 1, 0, 5, 6, 4, 0,
        //                             7, 0, 0, 0, 0, 0, 0, 0, 5,
        //                             5, 0, 1, 2, 0, 9, 3, 0, 7,
        //                             2, 0, 0, 0, 0, 0, 0, 0, 3,
        //                             0, 3, 6, 9, 0, 7, 5, 2, 0,
        //                             9, 0, 0, 0, 0, 0, 0, 0, 1,
        //                             3, 0, 2, 4, 0, 8, 1, 0, 9,
        //                             6, 0, 0, 0, 0, 0, 0, 0, 4,
        //                             0, 4, 7, 3, 0, 2, 8, 5, 0
        //                             };
        
        // hard
        array<array<int,9>,9> d = {
                                    0, 0, 0, 6, 2, 0, 0, 0, 3,
                                    0, 7, 0, 0, 8, 9, 0, 1, 0,
                                    0, 0, 3, 1, 0, 0, 6, 0, 0,
                                    0, 0, 4, 0, 0, 0, 7, 6, 0,
                                    0, 0, 0, 0, 4, 0, 0, 0, 0,
                                    0, 9, 2, 0, 0, 0, 5, 0, 0,
                                    0, 0, 6, 0, 0, 3, 1, 0, 0,
                                    0, 2, 0, 5, 9, 0, 0, 8, 0,
                                    3, 0, 0, 0, 6, 8, 0, 0, 0
                                    };

        sudoku = d;
    }
    else if(num == "n")
    {
        cout << "Please enter your sudoku, filling unknown cells with 0. Enter each number followed by pressing `Enter`. Fill the Sudoku row by row, starting from the top-left cell.\n";

        for(row = 0; row < 9; row ++)
            for(column = 0; column < 9; column ++)
            {
                cin >> num;
                if(stoi(num) >= 0 && stoi(num) <= 9)
                    sudoku[row][column] = stoi(num);
                else
                {
                    cout << "Invalid number.";
                    return 0;
                }
            }
    }
    else
    {
        cout << "Invalid character.";
        return 0;
    }

    cout << "Sudoku to be solved:\n";
    draw(sudoku);

    cout << "Solving...\n";
    val = alternatives(sudoku);
    if(get<0>(val))
        solve(sudoku,get<1>(val));

    if(solved)
    {
        cout << "Sudoku solved!\n";
        draw(sudoku);
    }
    else 
        cout << "Sudoku could not be solved.\n";
}

