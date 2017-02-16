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
            //fill all boundry indices with temperature values.
            if (r == 0) {
                xyGrid[r][c]=0;
            }
            else if(c == 0 || c == columnInputX -1) {
                xyGrid[r][c]=100;
            }
            else if(r == rowInputY -1 ){
                xyGrid[r][c]=100;
            }//end boundry fill
            else {
                //fill all inner nodes with 0s.
                xyGrid[r][c]=0;
                
                //Discrete heat calculation:
                //examine whether there is a boundry index at any given neighbouring points.
                if((r-1==0)&&(c-1==0)){
                    //a:..//
                } else if((r-1==0)&&(c+1==rowInputY)) {
                    //b:..//
                } else if ((r+1==rowInputY)&&(c+1==columnInputX)){
                    //c:..//
                } else if((r+1==rowInputY)&&(c-1==0)){
                    //d:..//
                } else if (c+1==columnInputX){
                    //e:..//
                } else if (c-1==0){
                    //f:..//
                } else if (r-1==0) {
                    //g:..//
                } else if (r+1==rowInputY){
                    //h:..//
                } else if ((r-1==0)&&(r+1==rowInputY)&&(c-1==0)&&(c+1==columnInputX)){
                    //i:..//
                } else {
                    //j..//
                }
            }
            cout<< "    [" << xyGrid[r][c] << "]";
        }//end for[column]
        std::cout << endl;
    }//end for[row]
    
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
