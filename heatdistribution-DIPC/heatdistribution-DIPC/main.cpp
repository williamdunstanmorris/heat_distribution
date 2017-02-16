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
   
    const int columnInputX = 10;
    const int rowInputY = 10;
    double xyGrid[columnInputX][rowInputY]; //Declare an empty 2D array with size determined by use
    
    
    for(int r = 0; r < rowInputY; r++){
        for(int c = 0; c <columnInputX; c++){
            if (r == 0) {
                xyGrid[r][c]=0;
            }
            else if(c == 0 || c == columnInputX -1) {
                xyGrid[r][c]=100;
            }
            else if(r == rowInputY -1 ){
                xyGrid[r][c]=100;
            }
            else {
                xyGrid[r][c]=0;
            }
            cout<< "    [" << xyGrid[r][c] << "]";
        }
        std::cout << endl;
    }//end for
    
    
    int c,r; //Declare inner looping variables outside the array for explicity when parallel processing
    
    
//    n = 5;                 // we'll do the 5 x 5 case as an example
//    vars = (n-2)^2;        // an n x n grid gives (n-2)^2 variables
//    T = zeros(n)           // set up an n x n zero matrix
//    T(1,:) = 1.0           // set all temperatures in row 1 to be 1.0
//    T(:,1) = 1.0           // set all temperatures in column 1 to be 1.0
    
    

//    for(r=0; r<rowInputY; r++){
//        for(c=0;c<columnInputX;c++){
//            double w = (0 + r-1 + c-1 + 100) * 0.25;
//            xyGrid[r][c] = w;
//            std::cout << "["<< xyGrid[r][c]<<"]     ";
//        }
//        cout << endl;
//    }
    return 0;
}
