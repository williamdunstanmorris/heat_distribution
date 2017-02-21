//
//  main.cpp
//  heatdistribution-DIPC
//
//  Created by William Morris on 14/02/2017.
//
//

//Assessment clarifications
//Posted on: Thursday, 9 February 2017 13:55:36 o'clock GMT
//There have been a few questions about the assessment:
//
//1: The brief says to "write the final estimate of the solution to a file that can be used to generate graphic representation of the solution”. The 'question' section 2.ii on page 4 also states that you should "show the calculated temperature distribution as an image". In other words, include the image in your report.
//2:Also in 2.ii, the "fixed problem size" could be fixed by the programmer or by the user - the essential point is that it should be the same for the calculation using first 1 and then 4 processors.
//3: In documenting your implementation, you don't need to go through every line of code, but you should explain your data structures, what algorithm you use for the computation, and what you did to distribute the processing (e.g. for a MPI implementation, what messages did you pass, how did you ensure the processes were all synchronised and so on.)
//4: The algorithm calculates a better value for the temperature at each step, starting from some initial solution.  Except for the fixed temperatures around the edge, your initial values for the temperatures could be random (or all some number such as zero - it shouldn't matter).
//5: Calculating the standard deviation:  if you have an array T representing the temperature of each point on the rectangle, your solution will calculate values for every element T[i][j]. The question asks you to do this using one processor, and using four processors. So, you will have two solutions for T, giving two values for each point T[i][j]. For each point, subtract one from the other to find the difference, then square it, then add all the squared differences together and divide by the number of things you added together. This is the variance of the differences, and then take the square root to get the standard deviation.

#include <iostream>
#include<math.h>
using namespace std;

int main() {
    
    //------------------------------------------
    const int columnInputX = 10;
    const int rowInputY = 10;
    const int threshold = 0;
    bool flag = true;
    double xyGrid[columnInputX][rowInputY]; //Declare an empty 2D array with size determined by use^I
    
    //------------------------------------------
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
            }
            cout<< "    [" << xyGrid[r][c] << "]";
        }//end for[column]
        std::cout << endl;
    }//end for[row]
    
    
    //------------------------------------------
    while (flag) {
        for(int r = 0; r < rowInputY; r++){
            for(int c = 0; c <columnInputX; c++){
                //Discrete heat calculation:
                //Examine whether there is a boundry index at any given neighbouring points.
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
                    //Why does (1/4) give a different output instead of 0.25 in C++?
                    //Test this
                    
                    int result = ((xyGrid[r-1][c])+
                                  (xyGrid[r][c+1])+
                                  (xyGrid[r+1][c])+
                                  (xyGrid[r][c-1]))/4; //calculate the mean value;
                    
                    int sd = sqrt((sqrt(xyGrid[r-1][c]-result))+
                               (sqrt(xyGrid[r][c+1]-result))+
                               (sqrt(xyGrid[r+1][c]-result))+
                               (sqrt(xyGrid[r][c-1]-result))/4); // calculate the standard deviation;
                    if(sd <= threshold){
                        flag = false;
                    } else {
                        xyGrid[r][c] = result;
                    }
                }
            }
        }
    }
    
    
    for(int r = 0; r < rowInputY; r++){
        for(int c = 0; c <columnInputX; c++){
            cout<< "    [" << xyGrid[r][c] << "]";
        }
        std::cout << endl;
    }
    
    return 0;
}


