#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip>
#include <vector>
#include <numeric>
#include <cstdlib>
#include <cmath>
#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include "heatDistribution.h"
//#include <usleep>

//Number of threads:
#define NUM_THREADS 4

using namespace std;


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
  }

  //------------------------------------------
  /*
  *Fill temp boundaries:
  TODO: refactor to 'Initialise';
  */
  void fill(std::vector<std::vector<double> > &vec, bool &parallel)
  {
    int r, c;
    double average = 0.0;
    # pragma omp parallel shared ( vec, average ) private (r, c) if(parallel)
    { //West Plate
      # pragma omp for
      for (r = 1; r < vec.size(); r++)
      {
        vec[r][0] = 100.0;
      }
      //East Plate
      # pragma omp for
      for ( r = 1; r < vec.size(); r++ )
      {
        vec[r][vec[0].size()-1] = 100.0;
      }
      //South Plate
      # pragma omp for
      for ( c = 0; c < vec[0].size(); c++ )
      {
        vec[vec.size()-1][c] = 100.0;
      }
      //North Plate
      # pragma omp for
      for ( c = 0; c < vec[0].size(); c++ )
      {
        vec[0][c] = 0.0;
      }
      //Calulcate Estimate values based on boundary values:
      # pragma omp for reduction (+:average)
      for (r = 1; r < vec.size()-1; r++)
      {
        average = average + vec[r][0] + vec[r][vec[0].size()-1];
      }

      # pragma omp for reduction (+:average)
      for (c = 1; c < vec[0].size(); c++)
      {
        average = average + vec[vec.size()-1][c] + vec[0][c];
      }

      # pragma omp for
      for(r=1; r < vec.size()-1; r++)
      {
        for(c=1; c<vec[0].size()-1; c++)
        {
          vec[r][c] = average;
        }
      }
    }

    average = average/(double)(2 * vec.size() + 2 * vec[0].size() - 4);

    # pragma omp parallel shared (average,vec) private (r,c)
    # pragma omp for
    for(r=1; r < vec.size()-1; r++)
    {
      for(c=1; c<vec[0].size()-1; c++)
      {
        vec[r][c] = average;
      }
    }

    for (int r = 0; r < vec.size(); r++)
    {
      for (int c = 0; c < vec[0].size(); c++)
      {
        // cout << "[" << setw(8) << vec[r][c] << "]";
      }
      // cout<<endl;
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

      //Calculation parameters
      double previousTotal = 0;
      double currentTotal =0;
      double diff = 0;

      //Performance Declarations:
      double hdOnePro_TICKS_AND_SECONDS_START = clock();
      double hdOnePro_omp_wtime_START = omp_get_wtime();
      double hdOnePro_omp_ticks_START = omp_get_wtick();

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

            //Store the temperature in an accumulative total of temp values.
            currentTotal+=vec[r][c];

          }
        }
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
        // cout<<"Difference between grids: "<<diff<<endl;
      }
      while (diff>tolerence);
      //---------------------------------------------------

      //Calculate the wall clock time spent on do{}while();
      double hdOnePro_TICKS_AND_SECONDS_END = clock();
      double hdOnePro_omp_wtime_END = omp_get_wtime();
      double hdOnePro_omp_ticks_END = omp_get_wtick();

      // cout << "\n*************************\nhdOnePro(): One Processor:"
      // <<"\nGrid_SPEC: (WxH) " << col << "x" << row
      // <<"\nTolerance Level: " << tolerence
      // <<"\nFinal accumulative difference between grids: "<<diff
      // << "\nIteration Count: " << ic
      // << "\nCPU Time(Ticks): " << hdOnePro_TICKS_AND_SECONDS_END - hdOnePro_TICKS_AND_SECONDS_START
      // << "\nCPU Time(Seconds): " << ((float) hdOnePro_TICKS_AND_SECONDS_END - hdOnePro_TICKS_AND_SECONDS_START) / CLOCKS_PER_SEC << "s"
      // << "\nOMP Wall Time(Seconds): "<< hdOnePro_omp_wtime_END - hdOnePro_omp_wtime_START <<"s"
      // << "\nOMP Timer Precision(Seconds): "<< hdOnePro_omp_ticks_END - hdOnePro_omp_ticks_START
      // << std::endl;

      txtFile << "\n*************************\nhdOnePro(): One Processor:"
      <<"\nGrid_SPEC: (WxH) " << col << "x" << row
      <<"\nTolerance Level: " << tolerence
      <<"\nFinal accumulative difference between grids: "<<diff
      << "\nIteration Count: " << ic
      << "\nCPU Time(Ticks): " << hdOnePro_TICKS_AND_SECONDS_END - hdOnePro_TICKS_AND_SECONDS_START
      << "\nCPU Time(Seconds): " << ((float) hdOnePro_TICKS_AND_SECONDS_END - hdOnePro_TICKS_AND_SECONDS_START) / CLOCKS_PER_SEC << "s"
      << "\nOMP Wall Time(Seconds): "<< hdOnePro_omp_wtime_END - hdOnePro_omp_wtime_START <<"s"
      << "\nOMP Timer Precision(Seconds): "<< hdOnePro_omp_ticks_END - hdOnePro_omp_ticks_START
      << "\n********************************\nFINAL GRID: \n***********************************"
      << std::endl;

      //Print Final Table Results to .txt File.
      // print(vec, txtFile);
      txtFile.close();
    }

    void hDFourPro(const unsigned int &row,
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

        //Calculation parameters
        double previousTotal = 0;
        double currentTotal =0;
        double diff = tolerence;

        //Performance Declarations:
        double hdFourPro_TICKS_AND_SECONDS_START = clock();
        double hdFourPro_omp_wtime_START = omp_get_wtime();
        double hdFourPro_omp_ticks_START = omp_get_wtick();

        //SPMD Algorithm Strategy for Parallel Processing
        //*************************//
        double threadDiff;
        int r,c;

        while(diff>=tolerence)
        {
          #pragma omp parallel shared(vec) private(r,c)
          {
            #pragma omp for
            for (r = 1; r < row - 1; r++)
            {
              for (c = 1; c < col - 1; c++)
              {
                vec[r][c] = ((0.25)
                * ( (vec[r - 1][c]) + (vec[r][c + 1]) + (vec[r + 1][c]) + (vec[r][c - 1]) ) );
              }
            }
          }//Finish Parallel BLOCK

          // for (int r = 0; r < vec.size(); r++)
          // {
          //   for (int c = 0; c < vec[0].size(); c++)
          //   {
          //          cout << "[" << setw(8) << vec[r][c] << "]";
          //   }
          //   cout<<endl;
          // }

          currentTotal = 0.0;
          #pragma omp parallel shared(diff, currentTotal,previousTotal) private (r,c,threadDiff)
          {
            threadDiff = 0.0;
            #pragma omp for reduction (+:currentTotal)
            for (r = 1; r < row - 1; r++)
            {
              for (c = 1; c < col - 1; c++)
              {
                currentTotal+=vec[r][c];
              }
            }
          }

          // cout<<"Current Total = "<<currentTotal<<endl;

          #pragma omp critical
          diff = (previousTotal - currentTotal);
          //Invert any negative values i.e. find the absolute value:
          if (diff<0.0)
          {
            diff=diff*-1;
            // cout<<"DIfference Inverted"<<endl;
          }
          if (diff>tolerence)
          {
            previousTotal = currentTotal;
          }

          // cout<<"Previous Total = "<<previousTotal<<endl;
          // cout<<"Difference = "<<diff<<endl;

          ic++;
          // cout<<"Difference between grids: "<<diff<<endl;

          // }//Finish Parallel BLOCK
        }

        //Calculate the wall clock time spent on do{}while();
        double hdFourPro_TICKS_AND_SECONDS_END = clock();
        double hdFourPro_omp_wtime_END = omp_get_wtime();
        double hdFourPro_omp_ticks_END = omp_get_wtick();

        // cout << "\n*************************\nhdFourPro(): Four Processors:"
        // <<"\nGrid_SPEC: (WxH) " << col << "x" << row
        // <<"\nTolerance Level: " << tolerence
        // <<"\nFinal accumulative difference between grids: "<<diff
        // << "\nIteration Count: " << ic
        // << "\nCPU Time(Ticks): " << hdFourPro_TICKS_AND_SECONDS_END - hdFourPro_TICKS_AND_SECONDS_START
        // << "\nCPU Time(Seconds): " << ((float) hdFourPro_TICKS_AND_SECONDS_END - hdFourPro_TICKS_AND_SECONDS_START) / CLOCKS_PER_SEC << "s"
        // << "\nOMP Wall Time(Seconds): "<< hdFourPro_omp_wtime_END - hdFourPro_omp_wtime_START <<"s"
        // << "\nOMP Timer Precision(Seconds): "<< hdFourPro_omp_ticks_END - hdFourPro_omp_ticks_START
        // << std::endl;

        txtFile << "\n*************************\nhdFourPro(): Four Processors:"
        <<"\nGrid_SPEC: (WxH) " << col << "x" << row
        <<"\nTolerance Level: " << tolerence
        <<"\nFinal accumulative difference between grids: "<<diff
        << "\nIteration Count: " << ic
        << "\nCPU Time(Ticks): " << hdFourPro_TICKS_AND_SECONDS_END - hdFourPro_TICKS_AND_SECONDS_START
        << "\nCPU Time(Seconds): " << ((float) hdFourPro_TICKS_AND_SECONDS_END - hdFourPro_TICKS_AND_SECONDS_START) / CLOCKS_PER_SEC << "s"
        << "\nOMP Wall Time(Seconds): "<< hdFourPro_omp_wtime_END - hdFourPro_omp_wtime_START <<"s"
        << "\nOMP Timer Precision(Seconds): "<< hdFourPro_omp_ticks_END - hdFourPro_omp_ticks_START
        << "\n********************************\nFINAL GRID: \n***********************************"
        << std::endl;

        //Print Final Table Results to .txt File.
        // print(vec, txtFile);
        txtFile.close();
      }

      /*
      Calculating the standard deviation:  if you have an array T representing the temperature of each point on the rectangle, your solution will calculate values for every element T[i][j]. The question asks you to do this using one processor, and using four processors.

      So, you will have two solutions for T, giving two values for each point T[i][j]. For each point, subtract one from the other to find the difference, then square it, then add all the squared differences together and divide by the number of things you added together. This is the variance of the differences, and then take the square root to get the standard deviation.
      */

      double deviation(std::vector<std::vector<double> > &vec,
        std::vector<std::vector<double> > &vec1)
        {
          double difference;
          double addedDifferences=0;
          for (int j = 1; j < vec.size()-1; j++)
          {
            for (int i = 1; i < vec[j].size()-1; i++)
            {
              if(vec[j][i] > vec1[j][i])
              {
                difference = std::pow(2.0,(vec[j][i] - vec1[j][i]));
              }
              else
              {
                difference = std::pow(2.0,(vec1[j][i]-vec[j][i]));
              }

              addedDifferences+=difference;
            }
          }
          double varianceDifferences = addedDifferences/(vec.size()-2*vec[0].size()-2);
          return sqrt(varianceDifferences);
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
              const double tolerence = 0.01;

              std::vector<std::vector<double> > v;
              std::vector<std::vector<double> > v1;

              std::vector<std::vector<int> > r;
              std::vector<std::vector<int> > g;
              std::vector<std::vector<int> > b;

              std::string ONE_fileString = "ONE_500x500_0.01.txt";
              std::string ONE_ppmstring = "ONE_500x500_0.01.ppm";
              std::string FOUR_fileString = "FOUR_500x500_0.01.txt";
              std::string FOUR_ppmstring = "FOUR_500x500_0.01.ppm";

              bool parallel = true;
              bool single = false;

              generate(row, column, v,r,g,b);
              fill(v, single);
              hDFourPro(row, column, v, tolerence, ONE_fileString);
              calculateRGB(v, r,g,b );
              writePPM(row, column,v,r,g,b, ONE_ppmstring);

              generate(row, column, v1,r,g,b);
              fill(v1, parallel);
              hDFourPro(row, column, v1, tolerence, FOUR_fileString);
              calculateRGB(v1, r,g,b );
              writePPM(row, column,v1,r,g,b, FOUR_ppmstring);

              ofstream txtFile("Standard Deviation",std::ofstream::out);
              txtFile<<"\nStandard Deviation Between Grids: "<<deviation(v,v1)<<endl;
              txtFile.close();

            }
