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
    //row size(y)
    cout << "Enter row values to specify the y(no. of rows) in the matrix:" << endl;
    cin >> rowInputY;
    
    int c,r; //Declare inner looping variables outside the array for explicity when parallel processing
    double xyGrid[columnInputX][rowInputY]; //Declare an empty 2D array with size determined by use

    
    for(r=0; r<rowInputY; r++){
        for(c=0;c<columnInputX;c++){
            double w = (0 + r-1 + c-1 + 0) * 0.25;
            xyGrid[r][c] = w;
            std::cout << "Values: " << xyGrid[r][c] << endl;
        }
    }
    return 0;
}
