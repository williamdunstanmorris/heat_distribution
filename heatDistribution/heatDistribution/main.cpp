#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip>
#include <vector>
#include <time.h>
#include <unistd.h>
#include <string>

using namespace std;

/*
* Declarations:
*/
//------------------------------------------
// const int columnInputX(3), rowInputY(3);
// double xyGrid[columnInputX][rowInputY];

std::vector<std::vector<int > > r;
std::vector<std::vector<int > > g;
std::vector<std::vector<int > > b;

//Declare an empty 2D array with size determined by use^I
//Make 3 global vectors r,g,b.
//Make them point to the size of row and column
//Also declare row and colum here but as empty. Must be defined in main method.

//------------------------------------------
/*
*Function Declarations:
*/
//TODO: What is an easier/ less performance consumptive way of declaring row and column here? N.B: they iterate over.
//I want to just declare it and then point to its size when iterating over it in methods...
int main(int argc, const char * argv[]);
void generate(unsigned const int& rows, unsigned const int& columns, std::vector< std::vector<double> >& vec);
void init(int rows, int columns);
void fill(std::vector< std::vector<double> >& vec);
void hDOnePro(std::vector< std::vector<double> > &vec, const double &threshold, std::string &fname);
void hDFourPro();
void writePPM(std::vector< std::vector<double> >& vec, std::string &fname);
void calculateRGB(std::vector< std::vector<double> >& xyGrid);
void print(std::vector< std::vector<double> >& xyGrid, std::ofstream &file);
void SWITCH(int &i);


//------------------------------------------
/*
*Initialise all vectors: temperature, r,g,b from row and column size:
 * 1: unsigned to avoid negative row and col values
 * 2: const values to protect inadvertent modification
 * 3: All parameters are pass-by-reference to avoid any unnecessary overhead from cloning
*/
void generate(unsigned const int& rows, unsigned const int& columns, std::vector< std::vector<double> >& vec)
{
  vec.resize(rows, std::vector<double>(columns, 0));
  r.resize(rows, std::vector<int>(columns,0));
  g.resize(rows, std::vector<int>(columns,0));
  b.resize(rows, std::vector<int>(columns,0));
  //Fill temperature vector with boundary values and 0s.
  fill(vec);

}

//------------------------------------------
/*
*Fill temp boundaries:
*/
void fill(std::vector< std::vector<double> >& vec) {

  for (int r = 0; r < vec.size(); r++) {
    for (int c = 0; c < vec[r].size(); c++) {
      if (r == 0) {
        vec[r][c] = 0;
      } else if (c == 0 || c == vec[r].size() - 1) {
        vec[r][c] = 100;
      } else if (r == vec.size() - 1) {
        vec[r][c] = 100;
      }//end boundary fill
      else {
        //fill all inner nodes with 0s.
        vec[r][c] = 0;
      }
      cout<<"[" << setw(8)<< vec[r][c] << "]";
    }
    cout<<endl;
  }
}

//------------------------------------------
/*
* Serial Program:
*/
void hDOnePro(std::vector< std::vector<double> >& vec,const double &threshold, std::string &fname){

  /*File .txt output in format:
   * #FileNumber_MxN_threshold
   * */
   //TODO: this does not work...
  ofstream txtFile(fname.c_str(),std::ofstream::out);

  //Calculation parameters
  double difference, addedValues, curTemp;
  double accumDifference = 0; double storedDiff = 0;
  double diff = 1;
  double dynamicRange;
  int ic = -1;

  //Clock ticks/seconds
  double hdOnePro_TICKS_AND_SECONDS_start = clock();

  do {
    ic++;
    for(int r = 0; r < vec.size(); r++){
      for(int c = 0; c <vec[r].size(); c++){

        //Reference the temperature for comparison later:
        curTemp = vec[r][c];

        //Calculate values inside boundary:
        if((r != 0) && (r != vec.size()-1)) {
          if((c != 0) && (c != vec[r].size()-1)){
            //Equation split up into a 'divide and conquer' algorithm
            addedValues = ((vec[r-1][c])+
                    (vec[r][c+1])+
                    (vec[r+1][c])+
                    (vec[r][c-1])); //Steady state heat equation.

            vec[r][c] = (0.25)*addedValues;

            difference = vec[r][c] - curTemp;

            //Accumulative difference
            accumDifference+=difference;
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
    diff = accumDifference/(vec.size()-1*(vec[0].size()-1));
    dynamicRange = diff - storedDiff;
    // cout << "\n-------------------------------------------------\n"<<endl;

  } while(dynamicRange >= threshold);

  double hdOnePro_TICKS_AND_SECONDS_end = clock();
  print(vec, txtFile);

  std:cout<<"[1][1] Temp = "<<vec[1][1]<<endl;
  std::cout << "One Processor: \nThreshold Level: " << threshold <<"\nFinal Dynamic Range to Break: " << dynamicRange <<"\nIteration Count: "<<ic<<"\nTicks: " << hdOnePro_TICKS_AND_SECONDS_end - hdOnePro_TICKS_AND_SECONDS_start << "\nSeconds: " << ((float)hdOnePro_TICKS_AND_SECONDS_end - hdOnePro_TICKS_AND_SECONDS_start)/CLOCKS_PER_SEC << "s" << std::endl;
  txtFile << "One Processor: \nThreshold Level: " << threshold <<"\nFinal Dynamic Range to Break: " << dynamicRange <<"\nIteration Count: "<<ic<<"\nTicks: " << hdOnePro_TICKS_AND_SECONDS_end - hdOnePro_TICKS_AND_SECONDS_start << "\nSeconds: " << ((float)hdOnePro_TICKS_AND_SECONDS_end - hdOnePro_TICKS_AND_SECONDS_start)/CLOCKS_PER_SEC << "s" << std::endl;
  txtFile.close();
}


void calculateRGB(std::vector< std::vector<double> >& vec){

  for (int j = 0; j < vec.size(); j++) {
    for (int i = 0; i < vec[j].size(); i++) {
      double f = vec[j][i]/100; //Get value between 0:1

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

void writePPM(std::vector< std::vector<double> > &vec, std::string &fname){

 //TODO: Make a file that does appends and does not overwrite, and iterate over the file number

 ofstream img(fname.c_str(), std::ofstream::out);
 img << "P3" << endl;
 img << 250 << " " << 250 << endl;
 img << "255" << endl; //maximum value of picture

 for (int j = 0; j < vec.size(); j++)
 {
   for (int i = 0; i < vec[j].size(); i++){
//      img <<" " << ((int)r[j][i])<< " " << ((int)g[j][i]) << " " <<((int)b[j][i]) << " ";

   }
   img << endl;
 }
 img.close();
}

void SWITCH(int &i){

  switch(i){
    case 1: {
      const unsigned int row = 100;
      const unsigned int column = 100;
      const double threshold = 0.01;
      std::vector<std::vector<double> > v;
      generate(row, column, v);
      std::string fileString = "#01_100x100_0.01.txt";
      hDOnePro(v, threshold, fileString);

      std::string ppmstring = "#01_100x100_0.01.ppm";
      calculateRGB(v);
      writePPM(v, ppmstring);
      break;
    }
    case 2: {
      const unsigned int row = 250;
      const unsigned int column = 250;
      const double threshold = 0.01;
      std::vector<std::vector<double> > v;
      generate(row, column, v);
      std::string fileString = "#02_250x250.01.txt";
      hDOnePro(v, threshold, fileString);

      std::string ppmstring = "#02_250x250.01.ppm";
      calculateRGB(v);
      writePPM(v, ppmstring);
      break;
    }
    case 3: {
      const unsigned int row = 500;
      const unsigned int column = 500;
      const double threshold = 0.01;
      std::vector<std::vector<double> > v;
      generate(row, column, v);
      std::string fileString = "#03_500x500.01.txt";
      hDOnePro(v, threshold, fileString);

      std::string ppmstring = "#02_500x500.01.ppm";
      calculateRGB(v);
      writePPM(v, ppmstring);
      break;
    }
    case 4: {
      const unsigned int row = 1000;
      const unsigned int column = 1000;
      const double threshold = 0.01;
      std::vector<std::vector<double> > v;
      generate(row, column, v);
      std::string fileString = "#04_1000x1000.01.txt";
      hDOnePro(v, threshold, fileString);

      std::string ppmstring = "#04_1000x1000.01.ppm";
      calculateRGB(v);
      writePPM(v, ppmstring);
      break;
    }
    default: {
      cout << "ERROR: undefined number specified";
      break;
    }
  }

}

void print(std::vector< std::vector<double> >& vec, std::ofstream &file) {
  //Iterate over size of array instead.

  for (int r = 0; r < vec.size(); r++) {
    for (int c = 0; c < vec[r].size(); c++) {
      cout << "[" << setw(8) << vec[r][c] << "]";
      file << "[" << setw(8) << vec[r][c] << "]";
    }
    cout << endl;
    file<<endl;
  }
}


int main(int argc, const char * argv[]) {

  //TODO: Decide how to determine a variable tolerence
  //TODO: Make the col and row ints small and run it on this computer.

  /*
  * pass by value will make a copy of the argument into the function parameter. In many cases,
  * this is a needless performance hit, as the original argument would have sufficed.
  */

    int switch1 = 1;
    int switch2 = 2;
    int switch3 = 3;
    int switch4 = 4;

  SWITCH(switch1);
  SWITCH(switch2);
  SWITCH(switch3);
  SWITCH(switch4);

}
