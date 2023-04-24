
#include <iostream>
#include <string>

#include "../../Src/Draw/Gnuplot.h"

int main(int argc, char const *argv[])
{
	Gnuplot plot;

	string commandEllipse, commandEllipseLSoM;
    string commandDistrib, commandDistribLSoM;
    string commandBins, commandBinsLSoM;
    string Bins, BinsPoints, BinsCumul;
    string commandMulti, FILENAME;

    // FILENAME = "../Data/SegmPoints0cut.txt";

    // FILENAME = "../Data/Contour/DropBoxRaw/SegmPoints0.txt";
    // FILENAME = "../Data/Contour/DropBoxRaw/SegmPoints1.txt";
    // FILENAME = "../Data/Contour/DropBoxRaw/SegmPoints2.txt";
    // FILENAME = "../Data/Contour/DropBoxRaw/SegmPoints3.txt";
    // FILENAME = "../Data/Contour/DropBoxRaw/SegmPoints4.txt";
    // FILENAME = "../Data/Contour/DropBoxRaw/SegmPoints5.txt";
    // FILENAME = "../Data/Contour/DropBoxRaw/SegmPoints6.txt";

    // FILENAME = "../Data/Contour/PolyRaw/SegmPoints0.txt";
    // FILENAME = "../Data/Contour/PolyRaw/SegmPoints1.txt";
    // FILENAME = "../Data/Contour/PolyRaw/SegmPoints2.txt";
    // FILENAME = "../Data/Contour/PolyRaw/SegmPoints3.txt";
    // FILENAME = "../Data/Contour/PolyRaw/SegmPoints4.txt";

    commandEllipse = "plot '" + FILENAME + "' lt rgb 'blue' ti 'Points', 'EllipseLSO.txt' w l lw 3 lt rgb 'red' ti 'EllipseLSO'";

    commandDistrib = "plot 'EllipseLSODistrib.txt' lw 3 lt rgb 'red' ti 'EllipseLSO'";

    Bins = "u (bin($2,width)):(1.0) s f w boxes fs solid 0.5";

    commandBins = "width=0.002\nbin(x, s) = s*int(x/s) + width/2\nset boxwidth width\nplot 'EllipseLSODistrib.txt' " + Bins + " lt rgb 'red' ti 'EllipseLSO'";

    commandMulti = "reset\nset terminal qt 1 size 1400,800 font 'Geneva,12'\nset size 2,1\nset multiplot\nset size 0.55, 0.5\nset origin 0.45, 0\nset title 'DISTANCE VISUALIZATION'\nset xlabel 'Number of point'\nset ylabel 'Distance from curve'\nset key spacing 1.3\nset label 'residualL2 = 4.837123e-02' at 2.77, 1.75\nset label '5159 points' at 2.84, 1.63\n" + commandDistrib + "\nset size 0.55, 0.5\nset origin 0.45, 0.5\nset title 'DISTANCE BAR CHART'\nset xlabel 'Distance from curve'\nset ylabel 'Count of points'\nset key spacing 1.3\n" + commandBins + "\nset size 1, 1\nset origin -0.6, 0\nset title 'ELLIPSE FITTING'\nset xlabel 'X-axis'\nset ylabel 'Y-axis'\nset key spacing 1.3\n" + commandEllipse + "\nunset multiplot";

    // commandMulti = "plot '../Data/Contour/DropBoxRaw/DROPBOXWholeContour.txt', '../Data/Contour/DropBoxRaw/Segm0Data/EllipseLSO.txt' w l lt rgb 'red', '../Data/Contour/DropBoxRaw/Segm1Data/EllipseLSO.txt' w l lt rgb 'red', '../Data/Contour/DropBoxRaw/Segm2Data/EllipseLSO.txt' w l lt rgb 'red', '../Data/Contour/DropBoxRaw/Segm3Data/EllipseLSO.txt' w l lt rgb 'red', '../Data/Contour/DropBoxRaw/Segm4Data/EllipseLSO.txt' w l lt rgb 'red', '../Data/Contour/DropBoxRaw/Segm5Data/EllipseLSO.txt' w l lt rgb 'red', '../Data/Contour/DropBoxRaw/Segm6Data/EllipseLSO.txt' w l lt rgb 'red'";

    plot(commandMulti);

	return 0;
}