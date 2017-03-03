#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

/*
 * Declarations:
 */
//------------------------------------------
const int columnInputX = 3;
const int rowInputY = 3;

double xyGrid[columnInputX][rowInputY]; //Declare an empty 2D array with size determined by use^I

double difference;
double totalDifference = 0;
double stopCond = 1;
double threshold = 0.01;
double ultimateVal;

double r[columnInputX][rowInputY] ;
double g[columnInputX][rowInputY] ;
double b[columnInputX][rowInputY] ;

int main(int argc, const char * argv[]);
void setup();
void hDOnePro();
void hDFourPro();
void printGraphic();
void calculateRGB();


//------------------------------------------
/*
 *Setup:
 */
void setup(){
    for(int c = 0; c < columnInputX; c++){
        for(int r = 0; r <rowInputY; r++){
            //Fill all boundary indices with temperature values.
            if (c == 0) {
                xyGrid[c][r]=0;
            }
            else if(r == 0 || r == rowInputY -1) {
                xyGrid[c][r]=100;
            }
            else if(c == columnInputX -1 ){
                xyGrid[c][r]=100;
            }//end boundary fill
            else {
                //fill all inner nodes with 0s.
                xyGrid[c][r]=0;
            }
            cout<< "    [" << xyGrid[r][c] << "]";
        }//end for[column]
        cout << "" <<endl;
    }//end for[row]
}

//------------------------------------------
/*
 * One Processor:
 */
void hDOnePro(){

    do {
//        ic++;
//        cout<< "//--------------\nIteration [" << ic << "]\n" << endl;
        for(int r = 0; r < rowInputY; r++){
            for(int c = 0; c <columnInputX; c++){

                //Reference the temperature for comparison later:
                double curTemp = xyGrid[r][c];

                //Calculate values inside boundary:
                if((r != 0) && (r != rowInputY-1)) {
                    if((c != 0) && (c != columnInputX-1)){
//                        std::cout <<"Index Point: [" << r << "][" << c << "] @W (temperature) = " << xyGrid[r][c] << endl;
                        double addedValues = ((xyGrid[r-1][c])+
                                (xyGrid[r][c+1])+
                                (xyGrid[r+1][c])+
                                (xyGrid[r][c-1])); //Steady state heat equation.
                        std::cout << "@addedValues = " << "(" << xyGrid[r-1][c]<< "+" << xyGrid[r][c+1]<< "+" << xyGrid[r+1][c]<< "+" << xyGrid[r][c-1]<< ") ="<< addedValues  <<endl;
                        //@test(1) for corrent added values;
                        double meanCalculation = (0.25)*addedValues;

                        //Successive estimate calculation: i.e the @new curTemp - current curTemp;
                        std::cout << "@meanCalculation = "<< meanCalculation<< endl;

                        xyGrid[r][c] = meanCalculation;

                        difference = xyGrid[r][c] - curTemp;
                        std::cout<<"@difference = " << xyGrid[r][c] << " - " << curTemp << " = " << difference << endl;

                        totalDifference+=difference;
                        std::cout<<"@totalAllDifference = " << totalDifference << "\n\n"<< endl;

                    }
                }
            } // End Column
        } // End Row

        double differenceLastGrid = stopCond;

        stopCond = totalDifference/(columnInputX*rowInputY);
//        std::cout<<"@stopCondition = " << stopCond<< "\n\n"<<endl;

        ultimateVal = stopCond - differenceLastGrid;
//        std::cout<<"@differencePoint = "<< ultimateVal << endl;

    } while(ultimateVal >= threshold);

    calculateRGB();
}

void calculateRGB(){
    for (int j = 0; j < columnInputX; j++) {
        for (int i = 0; i < rowInputY; i++) {
            double f = xyGrid[j][i]/100; //Get value between 0:1

            double a=(1-f)/0.25;	//invert and group
            int X=(int)floor(a);	//this is the integer part
            int Y=(int)floor(255*(a-X)); //fractional part from 0 to 255

            switch(X) {
                case 0:
                    r[j][i]=255;g[j][i]=Y;b[j][i]=0;
                    break;
                case 1:
                    r[j][i]=255-Y;g[j][i]=255;b[j][i]=0;
                    break;
                case 2:
                    r[j][i]=0;g[j][i]=255;b[j][i]=Y;
                    break;
                case 3:
                    r[j][i]=0;g[j][i]=255-Y;b[j][i]=255;
                    break;
                case 4:
                    r[j][i]=0;g[j][i]=0;b[j][i]=255;
                    break;
            }
        }
    }

}

void printGraphic(ofstream img){

    //TODO: Make a file that does appends and does not overwrite, and iterate over the file number

    img << "P3" << endl;
    img << columnInputX << " " << rowInputY << endl;
    img << "255" << endl;

    for (int j = 0; j < rowInputY; j++)
    {
        for (int i = 0; i < columnInputX; i++){
//            cout<< "    [" << xyGrid[j][i] << "]";
            img << ((int)r[j][i]) << " " << ((int)g[j][i]) << " " << ((int)b[j][i]) << " ";
        }
//        std::cout << endl;
        img << endl;
    }
    img.close();
    system("open picture.ppm");
}

void runallOneProc(){
    for(int i = 0; i < 10; i++){
        if(i == 0) {
            //TODO: Why can I not change the size of the array i.e. why does it have to be constant?
            columnInputX = 10;
            rowInputY = 10;
            hDOnePro();
            ofstream img("heatDistribution#1_10x10.ppm");
            //TODO: Why this this call incorrect?
            printGraphic(img);
        } else if(i == 1){

        } else if(i==2){

        } else if (i==3){

        } else if (i==4){

        } else if (i==5){

        }else if (i==6){

        }else if (i==7){

        }else if (i==8){

        }else{

        }
    }

}


int main(int argc, const char * argv[]) {

    //TODO: Decide how to determine a variable tolerence
    //TODO: Make the col and row ints small and run it on this computer.
    setup();
    hDOnePro();
    return 0;
}

