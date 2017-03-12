#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip>
#include <vector>
#include <numeric>
#include <cstdlib>
#include <cmath>
#include "heatDistribution.h"

//#include <usleep>

using namespace std;

/*
 * Declarations:
 */
//------------------------------------------

//std::vector<std::vector<int> > r;
//std::vector<std::vector<int> > g;
//std::vector<std::vector<int> > b;

//------------------------------------------
/*
 *Function Declarations:
 */
int main(int argc, const char *argv[]);

//------------------------------------------
/*
 *Initialise all vectors: temperature, r,g,b from row and column size:
 * 1: unsigned to avoid negative row and col values
 * 2: const values to protect inadvertent modification
 * 3: All parameters are pass-by-reference to avoid any unnecessary overhead from cloning
 */
void generate(unsigned const int &rows,
              unsigned const int &columns,
              std::vector<std::vector<double> > &vec,
              std::vector<std::vector<int> > &r,
              std::vector<std::vector<int> > &g,
              std::vector<std::vector<int> > &b)
{
    vec.resize(rows, std::vector<double>(columns, 0));
    r.resize(rows, std::vector<int>(columns, 0));
    g.resize(rows, std::vector<int>(columns, 0));
    b.resize(rows, std::vector<int>(columns, 0));
    fill(vec);
}

double preciseTol(double val, double tolerence)
{
    double tempA = val / tolerence;
    int ta = (int) tempA;
    tempA = ((double) ta) * tolerence;

    return tempA;
}

//------------------------------------------
/*
 *Fill temp boundaries:
 */
void fill(std::vector<std::vector<double> > &vec)
{
    for (int r = 0; r < vec.size(); r++)
    {
        for (int c = 0; c < vec[r].size(); c++)
        {
            if (r == 0)
            {
                vec[r][c] = 0;
            }
            else if (c == 0 || c == vec[r].size() - 1)
            {
                vec[r][c] = 100;
            }
            else if (r == vec.size() - 1)
            {
                vec[r][c] = 100;
            }//end boundary fill
            else
            {
                //fill all inner nodes with 0s.
                vec[r][c] = 0;
            }

            cout << "[" << setw(8) << vec[r][c] << "]";
        }

        cout << endl;
    }
}

//------------------------------------------
/*
 * Serial Program:
 */
void hDOnePro(const unsigned int &row,
              const unsigned int &col,
              std::vector<std::vector<double> > &vec,
              const double &tolerence,
              std::string &fname)
{
    /*File .txt output in format:
     * #FileNumber_MxN_threshold
     * */
    ofstream txtFile(fname.c_str(), std::ofstream::out);

    //Count the amount of iterations:
    int ic = -1;

    //Difference tolerance parameters:
    std::vector<double> differences;

    //Calculation parameters
    double previousTotal = 0;
    double currentTotal =0;
    double diff = 0;
    double hdOnePro_TICKS_AND_SECONDS_start = clock();

    do
    {
        ic++;
        currentTotal = 0;
        //---------------------------------------------------
        for (int r = 1; r < row - 1; r++)
        {
            for (int c = 1; c < col - 1; c++)
            {
                vec[r][c] = ((0.25)
                          * ( (vec[r - 1][c]) + (vec[r][c + 1]) + (vec[r + 1][c]) + (vec[r][c - 1]) ) );

//                cout<<"[" <<setw(8)<< vec[r][c]<<"]";

                //Store the temperature in an accumulative total of temp values.
                currentTotal+=vec[r][c];

            }
//            cout<<endl;
        }

        cout<<"CurTotal = "<<currentTotal<<endl;
        cout<<"PreviousTotal = "<<previousTotal<<endl;

        //Calculate the total difference between the previous total and current total.
        diff = (previousTotal - currentTotal);

        //Invert any negative values i.e. find the absolute value:
        if (diff<0.0)
        {
            diff=diff*-1;
        }

        if (diff>tolerence)
        {
            previousTotal = currentTotal;
        }

        //This difference should decrease per iteration:
        cout<<"Difference between grids: "<<diff<<endl;
    }
    while (diff>tolerence);
    //---------------------------------------------------

    //Calculate the wall clock time spent on do{}while();
    double hdOnePro_TICKS_AND_SECONDS_end = clock();

    cout << "One Processor: \nTolerence Level: " <<tolerence<<"\nFinal Difference Between Grids: "<<diff<< "\nIteration Count: " << ic << "\nTicks: "
    << hdOnePro_TICKS_AND_SECONDS_end - hdOnePro_TICKS_AND_SECONDS_start << "\nSeconds: "
    << ((float) hdOnePro_TICKS_AND_SECONDS_end - hdOnePro_TICKS_AND_SECONDS_start) / CLOCKS_PER_SEC << "s"
    << std::endl;

    //Print Specs to .txt File.
    txtFile << "One Processor: \nTolerence Level: " <<tolerence<<"\nFinal Difference Between Grids: "<<diff<< "\nIteration Count: " << ic << "\nTicks: "
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
void calculateRGB(std::vector<std::vector<double> > &vec,
                  std::vector<std::vector<int> > &r,
                  std::vector<std::vector<int> > &g,
                  std::vector<std::vector<int> > &b )
{
    for (int j = 0; j < vec.size(); j++)
    {
        for (int i = 0; i < vec[j].size(); i++)
        {

            double f = vec[j][i] / 100; //Get value between 0:1
            double a = (1 - f) / 0.25;    //invert and group

            int X = (int) floor(a);    //this is the integer part
            int Y = (int) floor(255 * (a - X)); //fractional part from 0 to 255

            switch (X)
            {
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
void writePPM(const unsigned int &row,
              const unsigned int &col,
              std::vector<std::vector<double> > &vec,
              std::vector<std::vector<int> > &r,
              std::vector<std::vector<int> > &g,
              std::vector<std::vector<int> > &b ,
              std::string &fname)
{
    //TODO: Make a file that does appends and does not overwrite, and iterate over the file number

    ofstream img(fname.c_str(), std::ofstream::out);
    img << "P3" << endl;
    img << col << " " << row << endl;
    img << "255" << endl; //maximum value of picture

    for (int j = 0; j < vec.size(); j++)
    {
        for (int i = 0; i < vec[j].size(); i++)
        {
            img << ((int) r[j][i]) << " " << ((int) g[j][i]) << " " << ((int) b[j][i]) << " ";

        }

        img << endl;
    }

    img.close();
}

void print(std::vector<std::vector<double> > &vec, std::ofstream &file)
{
    //Iterate over size of array instead.

    for (int r = 0; r < vec.size(); r++)
    {
        for (int c = 0; c < vec[r].size(); c++)
        {
            //      cout << "[" << setw(8) << vec[r][c] << "]";
            file << "[" << setw(8) << vec[r][c] << "]";
        }

        //    cout << endl;
        file << endl;
    }
}


int main(int argc, const char *argv[])
{
    /*
     * pass by value will make a copy of the argument into the function parameter. In many cases,
     * this is a needless performance hit, as the original argument would have sufficed.
     */
    const unsigned int row = 500;
    const unsigned int column = 500;
    const double tolerence = 0.1;
    std::vector<std::vector<double> > v;
    std::vector<std::vector<int> > r;
    std::vector<std::vector<int> > g;
    std::vector<std::vector<int> > b;
    
    std::string fileString = "#01_50x50.01.txt";
    std::string ppmstring = "#01_50x50.01.ppm";

    generate(row, column, v,r,g,b);
    hDOnePro(row, column, v, tolerence, fileString);
    calculateRGB(v, r,g,b );
    writePPM(row, column,v,r,g,b, ppmstring);

    
//    const unsigned int row2 = 50;
//    const unsigned int column2 = 80;
//    const double tolerence2 = 0.1;
//    std::vector<std::vector<double> > v2;
//    std::vector<std::vector<int> > r2;
//    std::vector<std::vector<int> > g2;
//    std::vector<std::vector<int> > b2;
//    
//    std::string fileString2 = "#02_20x20.01.txt";
//    std::string ppmstring2 = "#02_20x20.01.ppm";
//
//    generate(row2, column2, v2,r2,g2,b2);
//    hDOnePro(row2, column2, v2, tolerence2, fileString2);
//    calculateRGB(v2, r2,g2,b2 );
//    writePPM(row2, column2,v2, r2,g2,b2, ppmstring2);
}
