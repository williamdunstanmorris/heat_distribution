#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip>
#include <vector>
#include <time.h>
#include <unistd.h>

using namespace std;

/*
* Declarations:
*/
//------------------------------------------
// const int columnInputX(3), rowInputY(3);
// double xyGrid[columnInputX][rowInputY];

int row; int column;
std::vector<std::vector<int>> r;
std::vector<std::vector<int>> g;
std::vector<std::vector<int>> b;

//Declare an empty 2D array with size determined by use^I
//Make 3 global vectors r,g,b.
//Make them point to the size of row and column
//Also declare row and colum here but as empty. Must be defined in main method.

double difference, addedValues, curTemp;
double accumDifference, storedDiff = 0;
double diff = 1;
double threshold = 0.00001;
double dynamicRange;
int ic = -1;

//------------------------------------------
/*
*Function Declarations:
*/
//TODO: What is an easier/ less performance consumptive way of declaring row and column here? N.B: they iterate over.
//I want to just declare it and then point to its size when iterating over it in methods...
int main(int argc, const char * argv[]);
void generate(int rows, int columns, std::vector< std::vector<double> >& vec);
void init(int rows, int columns);

void fill(int rows, int columns, std::vector< std::vector<double> >& vec);
void hDOnePro(int row, int column, std::vector< std::vector<double> >& vec);
void hDFourPro();
void writePPM(int row, int column);
void calculateRGB(int row, int column, std::vector< std::vector<double> >& xyGrid);
void print(double **p, int &rowSize, int &colSize);


//------------------------------------------
/*
*Setup:
*/
void generate(int rows, int columns, std::vector< std::vector<double> >& vec)
{
  vec.resize(rows, std::vector<double>(1 << columns, 0));
  r.resize(rows, std::vector<int>(1<<columns,0));
  g.resize(rows, std::vector<int>(1<<columns,0));
  b.resize(rows, std::vector<int>(1<<columns,0));
  fill(rows, columns, vec);

}


//TODO: Point instead *rows? Is this better for performance here?
void fill(int rows, int columns, std::vector< std::vector<double> >& vec){

  //Define the array's size and elements:
  for(int r = 0; r < rows; r++){
    for(int c = 0; c <columns; c++){
      //Fill all boundary indices with temperature values.
      if (r == 0) {
        vec[r][c]=0;
      }
      else if(c == 0 || c == columns-1 ) {
        vec[r][c]=100;
      }
      else if(r == rows-1 ){
        vec[r][c]=100;
      }//end boundary fill
      else {
        //fill all inner nodes with 0s.
        vec[r][c]=0;
      }
      cout<<"[" << setw(8)<< vec[r][c] << "]";
    }//end for[column]
    cout << endl;
  }//end for[row]
}

//------------------------------------------
/*
* Serial Program:
*/
void hDOnePro(int row, int column, std::vector< std::vector<double> >& vec){

  int hdOnePro_TICKS_AND_SECONDS_start = clock();

  do {
    ic++;
    //        cout<< "//--------------\nIteration [" << ic << "]\n" << endl;

    for(int r = 0; r < row; r++){
      for(int c = 0; c <column; c++){

        //Reference the temperature for comparison later:
        curTemp = vec[r][c];

        //Calculate values inside boundary:
        if((r != 0) && (r != row-1)) {
          if((c != 0) && (c != column-1)){
            //                        std::cout <<"Index Point: [" << r << "][" << c << "] @W (temperature) = " << vec[r][c] << endl;
            //Equation split up into a 'divide and conquer' algorithm
            addedValues = ((vec[r-1][c])+
            (vec[r][c+1])+
            (vec[r+1][c])+
            (vec[r][c-1])); //Steady state heat equation.
            //                        std::cout << "@addedValues = " << "(" << vec[r-1][c]<< "+" << vec[r][c+1]<< "+" << vec[r+1][c]<< "+" << vec[r][c-1]<< ") ="<< addedValues  <<endl;
            //@test(1) for corrent added values;

            //Successive estimate calculation: i.e the @new curTemp - current curTemp;
            //  std::cout << "@sum = "<< addedValues<< endl;

            vec[r][c] = (0.25)*addedValues;

            difference = vec[r][c] - curTemp;
            //  std::cout<<"@difference = " << vec[r][c] << " - " << curTemp << " = " << difference << endl;

            //Accumulative difference
            accumDifference+=difference;
            //  std::cout<<"@totalAllDifference = " << accumDifference << "\n\n"<< endl;
          }
        }
        cout<<"[" << setw(8)<< vec[r][c] << "]";
      } // End Column
      // usleep(10000);
      cout << endl;
    } // End Row


    //Store last difference calculation into a temporary variable
    storedDiff = diff;
    // Calculation of difference
    diff = accumDifference/(row*column);
    dynamicRange = diff - storedDiff;
    cout << "\n-------------------------------------------------\n"<<endl;

  } while(dynamicRange >= threshold);

  int hdOnePro_TICKS_AND_SECONDS_end = clock();
  std::cout << "One Processor: \nThreshold Level: " << threshold <<"\nFinal Dynamic Range to Break: " << dynamicRange <<"\nIteration Count: "<<ic<<"\nTicks: " << hdOnePro_TICKS_AND_SECONDS_end - hdOnePro_TICKS_AND_SECONDS_start << "\nSeconds: " << ((float)hdOnePro_TICKS_AND_SECONDS_end - hdOnePro_TICKS_AND_SECONDS_start)/CLOCKS_PER_SEC << "s" << std::endl;

//TODO: Shall I pass the reference vec here, or the global xyGrid?

}

void calculateRGB(int row, int column, std::vector< std::vector<double> >& xyGrid){

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

void writePPM(int row, int column){

  //TODO: Make a file that does appends and does not overwrite, and iterate over the file number
  ofstream img("tryout.ppm");
  img << "P3" << endl;
  img << 50 << " " << 50 << endl;
  img << "255" << endl; //maximum value of picture

  for (int j = 0; j < row; j++)
  {
    for (int i = 0; i < column; i++){
      //            cout<< "    [" << xyGrid[j][i] << "]";
      img <<" " << ((int)r[j][i])<< " " << ((int)g[j][i]) << " " <<((int)b[j][i]) << " ";
    }
    //        std::cout << endl;
    img << endl;
  }
  img.close();
}

void print(double **p, int rowSize, int &colSize){
  //Iterate over size of array instead.

  for(int row = 0; row < rowSize; row++){
    for(int col = 0; col < colSize; col++){
      std::cout << std::setw(5) << p[row][col];
    }
    std::cout << std::endl;
  }
}

int main(int argc, const char * argv[]) {

  //TODO: Decide how to determine a variable tolerence
  //TODO: Make the col and row ints small and run it on this computer.

  /*
  * pass by value will make a copy of the argument into the function parameter. In many cases,
  * this is a needless performance hit, as the original argument would have sufficed.
  */
  int row = 1000;
  int column = 1000;

  std::vector<std::vector<double> > v;
  generate(row, column, v);
  hDOnePro(row, column, v);
  calculateRGB(row, column, v);
  writePPM(row, column);








  //    Static/Dynamic Array?
  //
  //            The C++ array classes are better behaved than the low-level C array because they know a lot about themselves, and can answer questions C arrays can't. They are able to clean after themselves. And more importantly, they are usually written using templates and/or inlining, which means that what appears to a lot of code in debug resolves to little or no code produced in release build, meaning no difference with their built-in less safe competition.
  //
  //    All in all, it falls on two categories:
  //
  //    Dynamic arrays
  //
  //    Using a pointer to a malloc-ed/new-ed array will be at best as fast as the std::vector version, and a lot less safe (see litb's post).
  //
  //    So use a std::vector.
  //
  //            Static arrays
  //
  //    Using a static array will be at best:
  //
  //    as fast as the std::array version
  //            and a lot less safe.

  //Why?



  //    //// free dynamically allocated memory
  //    for( int i = 0 ; i < *row ; i++ )
  //    {
  //        delete [] matrix[i] ;
  //    }
  //    delete [] matrix ;
}
