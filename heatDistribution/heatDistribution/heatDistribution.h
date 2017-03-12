//
//  heatDistribution.h
//  heatDistribution
//
//  Created by William Morris on 11/03/2017.
//  Copyright © 2017 William Morris. All rights reserved.
//

#ifndef heatDistribution_h
#define heatDistribution_h

void generate(unsigned const int &rows,
              unsigned const int &columns,
              std::vector<std::vector<double> > &vec,
              std::vector<std::vector<int> > &r,
              std::vector<std::vector<int> > &g,
              std::vector<std::vector<int> > &b);

void fill(std::vector<std::vector<double> > &vec);

void hDOnePro(const unsigned int &row,
              const unsigned int &col,
              std::vector<std::vector<double> > &vec,
              const double &tolerence,
              std::string &fname);

void hDFourPro(const unsigned int &row,
              const unsigned int &col,
              std::vector<std::vector<double> > &vec,
              const double &tolerence,
              std::string &fname);

double deviation(std::vector<std::vector<double> > &vec,
                std::vector<std::vector<double> > &vec1,
                std::ofstream &file);

void writePPM(const unsigned int &row,
              const unsigned int &col,
              std::vector<std::vector<double> > &vec,
              std::vector<std::vector<int> > &r,
              std::vector<std::vector<int> > &g,
              std::vector<std::vector<int> > &b ,
              std::string &fname);

void calculateRGB(std::vector<std::vector<double> > &xyGrid,
                  std::vector<std::vector<int> > &r,
                  std::vector<std::vector<int> > &g,
                  std::vector<std::vector<int> > &b );

void print(std::vector<std::vector<double> > &xyGrid, std::ofstream &file);

#endif /* heatDistribution_h */
