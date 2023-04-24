#ifndef GNUPLOT_H_
#define GNUPLOT_H_

#include <cstdio>
#include <string>
#include <iostream>

#define GNUPLOT_NAME "gnuplot -persist"

using std::string;
using std::cerr;

using namespace std;

class Gnuplot
{
public:
    Gnuplot() ;
    ~Gnuplot();
    void operator ()(const string & command);

protected:
    FILE *gnuplotpipe;
};

Gnuplot::Gnuplot()
{
    gnuplotpipe  = popen(GNUPLOT_NAME, "w");

    if (!gnuplotpipe)
        cerr << ("Gnuplot not found !");
}
Gnuplot::~Gnuplot()
{
    fprintf(gnuplotpipe,"exit\n");
    pclose(gnuplotpipe);
}
void Gnuplot::operator()(const string & command)
{
    fprintf(gnuplotpipe,"%s\n",command.c_str());
    fflush(gnuplotpipe);
};

#endif // GNUPLOT_H_