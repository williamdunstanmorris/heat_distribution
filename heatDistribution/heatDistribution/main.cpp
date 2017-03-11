#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip>
#include <vector>
#include <numeric>
#include <cstdlib>
#include <cmath>

using namespace std;

/*
* Declarations:
*/
//------------------------------------------

std::vector<std::vector<int> > r;
std::vector<std::vector<int> > g;
std::vector<std::vector<int> > b;




//------------------------------------------
/*
*Function Declarations:
*/
//TODO: What is an easier/ less performance consumptive way of declaring row and column here? N.B: they iterate over.
int main(int argc, const char *argv[]);

void generate(unsigned const int &rows, unsigned const int &columns, std::vector<std::vector<double> > &vec);

void fill(std::vector<std::vector<double> > &vec);

void hDOnePro(const unsigned int &row, const unsigned int &col, std::vector<std::vector<double> > &vec, const double &threshold, std::string &fname);

void writePPM(std::vector<std::vector<double> > &vec, std::string &fname);

void calculateRGB(std::vector<std::vector<double> > &xyGrid);

void print(std::vector<std::vector<double> > &xyGrid, std::ofstream &file);





//------------------------------------------
/*
*Initialise all vectors: temperature, r,g,b from row and column size:
* 1: unsigned to avoid negative row and col values
* 2: const values to protect inadvertent modification
* 3: All parameters are pass-by-reference to avoid any unnecessary overhead from cloning
*/
void generate(unsigned const int &rows, unsigned const int &columns, std::vector<std::vector<double> > &vec) {
    vec.resize(rows, std::vector<double>(columns, 0));
    r.resize(rows, std::vector<int>(columns, 0));
    g.resize(rows, std::vector<int>(columns, 0));
    b.resize(rows, std::vector<int>(columns, 0));
    fill(vec);
}




//------------------------------------------
/*
*Fill temp boundaries:
*/
void fill(std::vector<std::vector<double> > &vec) {

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
                vec[r][c] = 50;
            }
//            cout << "[" << setw(8) << vec[r][c] << "]";
        }
//        cout << endl;
    }
}




//------------------------------------------
/*
* Serial Program:
*/
void hDOnePro(const unsigned int &row, const unsigned int &col, std::vector<std::vector<double> > &vec, const double &threshold, std::string &fname) {

    /*File .txt output in format:
    * #FileNumber_MxN_threshold
    * */
    ofstream txtFile(fname.c_str(), std::ofstream::out);

    //Calculation parameters
    double storedTemp, storedDifference = 0;
    double dynamicRange;
    int ic = -1;

    //Difference tolerance parameters:
    std::vector<double> differences;
    double maxDifference;
    double differenceOfDifferences, diff = 0;

    //Clock ticks/seconds
    double hdOnePro_TICKS_AND_SECONDS_start = clock();

    do {
        ic++;
        maxDifference = 0;
        //---------------------------------------------------
        for (int r = 1; r < row - 1; r++) {
            for (int c = 1; c < col - 1; c++) {

                //Before calculating the new temperature value, store the old one for calculation later:
                storedTemp = vec[r][c];
//              cout<<"Index: ["<<r<<"],["<<c<<"]"<<endl;
//              cout<<"Current Temp = " << vec[r][c]<<endl;
//              cout<<"Stored Temp = "<<storedTemp<<endl;

                //New temperature calculation:
                vec[r][c] = ((0.25) * ((vec[r - 1][c]) +
                        (vec[r][c + 1]) +
                        (vec[r + 1][c]) +
                        (vec[r][c - 1]))
                );

//              cout<<"New Temp = " << vec[r][c]<<endl;
                //Initially set to 0. Will store the previous index's difference in temperature.
                storedDifference = diff;
//              cout<<"Stored Difference = " << storedDifference<<endl;
                diff = vec[r][c] - storedTemp;
//              cout<<"Difference = "<<diff<<endl;

                if (storedDifference > diff) {
                    differenceOfDifferences = storedDifference - diff;
//                  cout<<"DifferenceOfDiff = " << differenceOfDifferences<<endl;
                } else {
                    differenceOfDifferences = diff - storedDifference;
//                  cout<<"DifferenceOfDiff = " << differenceOfDifferences<<endl;

                }

                if (differenceOfDifferences > maxDifference) {
                    maxDifference = differenceOfDifferences;
                    cout << "Maximum DifferenceOfDifferences = " << maxDifference << endl;
                }

//              cout<<"********************************"<<endl;
//              usleep(200000);
            }
//          cout<<"********************************"<<endl;
//          cout<<endl;
        }
        //---------------------------------------------------
        cout << "NEW GRID\n********************************\n" << endl;

    } while (maxDifference > threshold);
    //---------------------------------------------------

    //Calculate the wall clock time spent on do{}while();
    double hdOnePro_TICKS_AND_SECONDS_end = clock();

    cout << "One Processor: \nThreshold Level: " << threshold << "\nFinal Dynamic Range to Break: " << dynamicRange
            << "\nIteration Count: " << ic << "\nTicks: "
            << hdOnePro_TICKS_AND_SECONDS_end - hdOnePro_TICKS_AND_SECONDS_start << "\nSeconds: "
            << ((float) hdOnePro_TICKS_AND_SECONDS_end - hdOnePro_TICKS_AND_SECONDS_start) / CLOCKS_PER_SEC << "s"
            << std::endl;

    //Print Specs to .txt File.

    txtFile << "One Processor: \nThreshold Level: " << threshold << "\nFinal Dynamic Range to Break: " << dynamicRange
            << "\nIteration Count: " << ic << "\nTicks: "
            << hdOnePro_TICKS_AND_SECONDS_end - hdOnePro_TICKS_AND_SECONDS_start << "\nSeconds: "
            << ((float) hdOnePro_TICKS_AND_SECONDS_end - hdOnePro_TICKS_AND_SECONDS_start) / CLOCKS_PER_SEC << "s"
            << std::endl;
    //Print Final Table Results to .txt File.

    print(vec, txtFile);
    txtFile.close();
}

//------------------------------------------
/*
* Calculate RGB Vectors from Temperature Grid:
*/
void calculateRGB(std::vector<std::vector<double> > &vec) {

    for (int j = 0; j < vec.size(); j++) {
        for (int i = 0; i < vec[j].size(); i++) {

            double f = vec[j][i] / 100; //Get value between 0:1

            double a = (1 - f) / 0.25;    //invert and group
            int X = (int) floor(a);    //this is the integer part
            int Y = (int) floor(255 * (a - X)); //fractional part from 0 to 255

            switch (X) {
                case 0:
                    r[j][i] = 255;
                    g[j][i] = Y;
                    b[j][i] = 0;
                    break;
                case 1:
                    r[j][i] = 255 - Y;
                    g[j][i] = 255;
                    b[j][i] = 0;
                    break;
                case 2:
                    r[j][i] = 0;
                    g[j][i] = 255;
                    b[j][i] = Y;
                    break;
                case 3:
                    r[j][i] = 0;
                    g[j][i] = 255 - Y;
                    b[j][i] = 255;
                    break;
                case 4:
                    r[j][i] = 0;
                    g[j][i] = 0;
                    b[j][i] = 255;
                    break;
            }
        }
    }
}
//------------------------------------------
/*
* Write PPM bitmap file from RGB Vectors
*/
void writePPM(std::vector<std::vector<double> > &vec, std::string &fname) {

    //TODO: Make a file that does appends and does not overwrite, and iterate over the file number

    ofstream img(fname.c_str(), std::ofstream::out);
    img << "P3" << endl;
    img << 250 << " " << 250 << endl;
    img << "255" << endl; //maximum value of picture

    for (int j = 0; j < vec.size(); j++) {
        for (int i = 0; i < vec[j].size(); i++) {
            img << ((int) r[j][i]) << " " << ((int) g[j][i]) << " " << ((int) b[j][i]) << " ";

        }
        img << endl;
    }
    img.close();
}

void print(std::vector<std::vector<double> > &vec, std::ofstream &file) {
    //Iterate over size of array instead.

    for (int r = 0; r < vec.size(); r++) {
        for (int c = 0; c < vec[r].size(); c++) {
//      cout << "[" << setw(8) << vec[r][c] << "]";
            file << "[" << setw(8) << vec[r][c] << "]";
        }
//    cout << endl;
        file << endl;
    }
}


int main(int argc, const char *argv[]) {

    /*
    * pass by value will make a copy of the argument into the function parameter. In many cases,
    * this is a needless performance hit, as the original argument would have sufficed.
    */
    const unsigned int row = 250;
    const unsigned int column = 250;
    const double tolerence = 0.001;
    std::vector<std::vector<double> > v;
    generate(row, column, v);

    cout << "Row size: " << v.size() << endl;
    cout << "Column size: " << v[0].size() << endl;

    std::string fileString = "#03_500x500.01.txt";
    hDOnePro(row, column, v, tolerence, fileString);

    std::string ppmstring = "#03_500x500.01.ppm";
    calculateRGB(v);
    writePPM(v, ppmstring);

}
