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
                //
                if((r-1 == 0) && (c-1 == 0)){ //examine whether there is a boundry index at any given neighbouring points.
                    //...//
                } else if(r-1==0) {
                    //..//
                } else if (c-1==0){
                    //..//
                } else if((r+1==rowInputY)&&(c+1==columnInputX)){
                    //..//
                } else if (r+1==rowInputY){
                    //..//
                } else if (c+1==columnInputX){
                    //..//
                }
            }
            cout<< "    [" << xyGrid[r][c] << "]";
        }
        std::cout << endl;
    }//end for
    
    //Put values into a grid.
    
    
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
