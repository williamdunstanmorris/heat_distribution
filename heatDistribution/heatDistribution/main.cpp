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
int i;

double xyGrid[columnInputX][rowInputY]; //Declare an empty 2D array with size determined by use^I

double difference, addedValues, curTemp, sum;
double accumDifference, storedDiff = 0;
double diff = 1;
double threshold = 0.01;
double dynamicRange;

double r[columnInputX][rowInputY] ;
double g[columnInputX][rowInputY] ;
double b[columnInputX][rowInputY] ;

//------------------------------------------
/*
 *Function Declarations:
 */
int main(int argc, const char * argv[]);
void init(const int row, const int column);
void hDOnePro(const int row, const int column);
void hDFourPro();
void writePPM(ofstream img);
void calculateRGB(const int row, const int column);


//------------------------------------------
/*
 *Setup:
 */
void init(const int row, const int column){
    for(int r = 0; r < row; r++){
        for(int c = 0; c <column; c++){
            //Fill all boundary indices with temperature values.
            if (r == 0) {
                xyGrid[r][c]=0;
            }
            else if(c == 0 || c == column-1 ) {
                xyGrid[r][c]=100;
            }
            else if(r == row-1 ){
                xyGrid[r][c]=100;
            }//end boundary fill
            else {
                //fill all inner nodes with 0s.
                xyGrid[r][c]=0;
            }
            cout<< "    [" << xyGrid[r][c] << "]";
        }//end for[column]
        cout << "" <<endl;
    }//end for[row]
}

//------------------------------------------
/*
 * Serial Program:
 */
void hDOnePro(const int row, const int column){

    //TODO: redo iteration variable names.
    do {
//        ic++;
//        cout<< "//--------------\nIteration [" << ic << "]\n" << endl;
        for(int r = 0; r < row; r++){
            for(int c = 0; c <column; c++){

                //Reference the temperature for comparison later:
                curTemp = xyGrid[r][c];

                //Calculate values inside boundary:
                if((r != 0) && (r != rowInputY-1)) {
                    if((c != 0) && (c != columnInputX-1)){
//                        std::cout <<"Index Point: [" << r << "][" << c << "] @W (temperature) = " << xyGrid[r][c] << endl;
                        //Equation split up into a 'divide and conquer' algorithm
                        addedValues = ((xyGrid[r-1][c])+
                                (xyGrid[r][c+1])+
                                (xyGrid[r+1][c])+
                                (xyGrid[r][c-1])); //Steady state heat equation.
//                        std::cout << "@addedValues = " << "(" << xyGrid[r-1][c]<< "+" << xyGrid[r][c+1]<< "+" << xyGrid[r+1][c]<< "+" << xyGrid[r][c-1]<< ") ="<< addedValues  <<endl;
                        //@test(1) for corrent added values;

                        //Successive estimate calculation: i.e the @new curTemp - current curTemp;
//                        std::cout << "@sum = "<< sum<< endl;

                        sum = (0.25)*addedValues;

                        xyGrid[r][c] = sum;

                        difference = xyGrid[r][c] - curTemp;
//                        std::cout<<"@difference = " << xyGrid[r][c] << " - " << curTemp << " = " << difference << endl;

                        //Accumulative difference
                        accumDifference+=difference;
//                        std::cout<<"@totalAllDifference = " << accumDifference << "\n\n"<< endl;

                    }
                }
            } // End Column
        } // End Row

        //Store last difference calculation into a temporary variable
        storedDiff = diff;

        //Calculation of difference
        diff = accumDifference/(columnInputX*rowInputY);
//        std::cout<<"@stopCondition = " << diff<< "\n\n"<<endl;
        dynamicRange = diff - storedDiff;
        std::cout<<"@differencePoint = "<< dynamicRange << endl;

    } while(dynamicRange >= threshold);

    calculateRGB(row, column);

}

void calculateRGB(const int row, const int column){
    for (int j = 0; j < row; j++) {
        for (int i = 0; i < column; i++) {
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

void writePPM(){

    //TODO: Make a file that does appends and does not overwrite, and iterate over the file number
    ofstream img("tryout.ppm");
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

int main(int argc, const char * argv[]) {

    //TODO: Decide how to determine a variable tolerence
    //TODO: Make the col and row ints small and run it on this computer.
    int r = 10; int c = 10;
    init(r,c);
    hDOnePro(r,c);
    ofstream img("tryout.ppm");
    writePPM();

    return 0;
}

