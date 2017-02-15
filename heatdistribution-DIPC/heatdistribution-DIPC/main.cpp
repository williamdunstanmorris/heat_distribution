//
//  main.cpp
//  heatdistribution-DIPC
//
//  Created by William Morris on 14/02/2017.
//
//

#include <iostream>
using namespace std;

int main() {
   
    
    //user input for matrices:
    int columnInputX;
    int rowInputY;
    
    //column size(x)
    cout << "Enter column values to specify the x(no. of columns) in the matrix:" << endl;
    cin >> columnInputX;
    const int COLUMN = columnInputX;
    
    //row size(y)
    cout << "Enter row values to specify the y(no. of rows) in the matrix:" << endl;
    cin >> rowInputY;
    const int ROW = rowInputY;
    
    int xyGrid[COLUMN][ROW]; //Declare an empty 2D array with size determined by user.
    
    int c; //Declare inner looping variables outside the array for explicity when parallel processing
    int r;
    
    
    
    for(r=0; r<ROW; r++){
        for(c=0;c<COLUMN;c++){
            xyGrid[c][r] = (1/4) * (0 + (r-1) + (c-1) + 0);
        }
    }
    return 0;
}
