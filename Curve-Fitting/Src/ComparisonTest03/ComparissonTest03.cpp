#include"ComparissonTest03.h"
#define accuracy 2048

const char* PathToWrite = "D:\\YandexDisk\\Ëèçà\\Octonus\\";
int ID = 2;

namespace fs = std::experimental::filesystem;

struct OrderedSide
{
	int id;
	double area;
};

void PrintCurrentSide(POLYHEDRON* Poly, int SideId, string Prefix);
void PrintCurrentSide(POLYHEDRON* Poly, int SideId, string Prefix)
{
	int n = length(Poly->side[SideId].vlist) - 1;
	int id;
	const string fullDirname__string = string(PathToWrite) + Prefix + string(".txt");
	const char* FullDirName = fullDirname__string.c_str();

	FILE* f = fopen(FullDirName, "w");

	for (int i = 0; i < n; i++)
	{
		id = Poly->side[SideId].vlist[i];

		fprintf(f, "%lf, %lf, %lf\n", Poly->vertex[id].x, Poly->vertex[id].y, Poly->vertex[id].z);
	}

	fclose(f);
}


void PrintProjection(int ConvHullCount, int* PatternHullInds, VECTOR2* Projection, string id1, int i);
void PrintProjection(int ConvHullCount, int* PatternHullInds, VECTOR2* Projection, string id1, int i)
{
	const string fullDirname__string = string(PathToWrite) + id1 + string(to_string(i)) + string(".txt");
	const char* FullDirName = fullDirname__string.c_str();

	FILE* f = fopen(FullDirName, "w");
	int id_pl, id;
	
	for (int i = 0; i < ConvHullCount - 1; i++)
	{
		id = PatternHullInds[i];
		id_pl = PatternHullInds[i + 1];
	
		fprintf(f, "%lf, %lf\n", Projection[id].x, Projection[id].y);
		fprintf(f, "%lf, %lf\n\n", Projection[id_pl].x, Projection[id_pl].y);
	}
	
	id = PatternHullInds[ConvHullCount - 1];
	id_pl = PatternHullInds[0];
	
	fprintf(f, "%lf, %lf\n", Projection[id].x, Projection[id].y);
	fprintf(f, "%lf, %lf\n\n", Projection[id_pl].x, Projection[id_pl].y);
	
	fclose(f);
}

//double RetMatchingError(POLYHEDRON* Pattern, POLYHEDRON* Real, VECTOR Norm);
//double RetMatchingError(POLYHEDRON* Pattern, POLYHEDRON* Real, VECTOR Norm)
//{
//	VECTOR2* RealProjection = new VECTOR2[Real->nverts];
//	VECTOR2* PatternProjection = new VECTOR2[Pattern->nverts];
//
//	int RealConvHullCount, PatternConvHullCount, id;
//	int* RealHullInds = new int[Real->nverts];
//	int* PatternHullInds = new int[Pattern->nverts];
//
//	double error, err;
//
//	net_func RealF(accuracy), PatternF(accuracy), Error(accuracy);
//
//	RealConvHullCount = ConvexHullOfProjection(Real, Norm, RealHullInds, RealProjection);
//	PatternConvHullCount = ConvexHullOfProjection(Pattern, Norm, PatternHullInds, PatternProjection);
//
//	PrintProjection(RealConvHullCount, RealHullInds, RealProjection, "RealProj", ID);
//	PrintProjection(PatternConvHullCount, PatternHullInds, PatternProjection, "PatternProj", ID);
//
//	VECTOR2* RealPts = new VECTOR2[RealConvHullCount];
//	VECTOR2* PatternPts = new VECTOR2[PatternConvHullCount];
//
//	for (int i = 0; i < RealConvHullCount; i++)
//	{
//		id = RealHullInds[i];
//
//		RealPts[i] = RealProjection[id];
//	}
//
//	for (int i = 0; i < PatternConvHullCount; i++)
//	{
//		id = PatternHullInds[i];
//
//		PatternPts[i] = PatternProjection[id];
//	}
//
//	Set(RealF, RealConvHullCount, RealPts);
//	Set(PatternF, PatternConvHullCount, PatternPts);
//
//	RealF.fprint("D:\\YandexDisk\\Ëèçà\\Octonus\\RealF" + to_string(ID) + ".txt");
//	PatternF.fprint("D:\\YandexDisk\\Ëèçà\\Octonus\\PatternF" + to_string(ID) + ".txt");
//	ID++;
//	L2_error(Error.f, PatternF.f, RealF.f, accuracy);
//
//	err = Error.f[0];
//
//	Polar Real_p_s(100, Real->nverts, RealProjection, RealConvHullCount, RealHullInds);
//	Polar Pattern_p_s(100, Pattern->nverts, PatternProjection, PatternConvHullCount, PatternHullInds);
//
//	error = Pattern_p_s.L2Diff(Real_p_s);
//
//	delete[] RealProjection;
//	delete[] PatternProjection;
//	delete[] RealHullInds;
//	delete[] PatternHullInds;
//
//	/*printf("In function error via L2diff is %lf\n", error);
//	printf("In function error via L2error is %lf\n", err);*/
//
//	return err;
//}

void MatchingSides(POLYHEDRON* Pattern, POLYHEDRON* Real, int PatternSideId, int RealSideId, PlaneInfo* PatternInfo, PlaneInfo* RealInfo, SIMILARITY Back[]);
//void MatchingSides(POLYHEDRON* Pattern, POLYHEDRON* Real, int PatternSideId, int RealSideId, PlaneInfo* PatternInfo, PlaneInfo* RealInfo)
//{
//	VECTOR y(0, 1, 0);
//	VECTOR v_null(0, 0, 0);
//	SPIN sp(y, 0);
//	VECTOR v_move(PatternInfo[PatternSideId].x_area - RealInfo[RealSideId].x_area, PatternInfo[PatternSideId].y_area - RealInfo[RealSideId].y_area, PatternInfo[PatternSideId].z_area - RealInfo[RealSideId].z_area);
//	/*VECTOR v_move(PatternInfo[PatternSideId].x - RealInfo[RealSideId].x, PatternInfo[PatternSideId].y - RealInfo[RealSideId].y, PatternInfo[PatternSideId].z - RealInfo[RealSideId].z);*/
//	VECTOR v_move1(-PatternInfo[PatternSideId].x_area, -PatternInfo[PatternSideId].y_area, -PatternInfo[PatternSideId].z_area);
//	CONFORM con(sp, v_move, 1);
//	SIMILARITY sim(con);
//	TransformPolyhedron(Real, sim);
//
//	CONFORM con1(sp, v_move1, 1);
//	SIMILARITY sim1(con1);
//
//	TransformPolyhedron(Real, sim1);
//
//	RealInfo[RealSideId].x_area += v_move.x;
//	RealInfo[RealSideId].y_area += v_move.y;
//	RealInfo[RealSideId].z_area += v_move.z;
//
//	SPIN SpinForward;
//	VECTOR Pattern_e1, Pattern_e2;
//	VECTOR Real_e1, Real_e2;
//	VECTOR RealNorm = Real->side[RealSideId].plane.norm;
//	VECTOR PatternNorm = Pattern->side[PatternSideId].plane.norm;
//
//	VECTOR3 ax[3];
//	VECTOR Z(RealInfo[RealSideId].x_area, RealInfo[RealSideId].y_area, RealInfo[RealSideId].z_area);
//
//	AddOnReper(Real_e1, Real_e2, RealNorm);
//	AddOnReper(Pattern_e1, Pattern_e2, PatternNorm);
//
//	ax[0] = Real_e1;
//	ax[1] = Real_e2;
//	ax[2] = RealNorm;
//
//	EulerTransform(Real_e1, Real_e2, RealNorm,
//		Pattern_e1, Pattern_e2, PatternNorm, SpinForward);
//
//	CONFORM ConForwardPlus(SpinForward, v_null, 1);
//	SIMILARITY SimForwardPlus(ConForwardPlus);
//
//	/*DrawAxes(ax, "D:\\YandexDisk\\Ëèçà\\Octonus\\B", Z);*/
//
//	TransformPolyhedron(Real, SimForwardPlus);
//	CONFORM con2(sp, -v_move1, 1);
//	SIMILARITY sim2(con2);
//
//	TransformPolyhedron(Real, sim2);
//	//TransformPolyhedron(Real, sim);
//
//	RealNorm = SimForwardPlus(RealNorm);
//	//RealNorm = sim2(RealNorm);
//	ax[2] = RealNorm;
//	DrawAxes(ax, "D:\\YandexDisk\\Ëèçà\\Octonus\\Real", Z);
//
//	ax[0] = Pattern_e1;
//	ax[1] = Pattern_e2;
//	ax[2] = PatternNorm;
//	DrawAxes(ax, "D:\\YandexDisk\\Ëèçà\\Octonus\\Pattern", Z);
//
//	int id;
//	int* RealHullInds = new int[Real->nverts];
//	int* PatternHullInds = new int[Pattern->nverts];
//
//	VECTOR2* RealProjection = new VECTOR2[Real->nverts];
//	VECTOR2* PatternProjection = new VECTOR2[Pattern->nverts];
//
//	int RealConvHullCount = ConvexHullOfProjection(Real, PatternNorm, RealHullInds, RealProjection);
//	int PatternConvHullCount = ConvexHullOfProjection(Pattern, PatternNorm, PatternHullInds, PatternProjection);
//
//	FILE* f = fopen("D:\\YandexDisk\\Ëèçà\\Octonus\\PatternProj.txt", "w");
//	int id_pl;
//
//	for (int i = 0; i < PatternConvHullCount - 1; i++)
//	{
//		id = PatternHullInds[i];
//		id_pl = PatternHullInds[i + 1];
//
//		fprintf(f, "%lf, %lf\n", PatternProjection[id].x, PatternProjection[id].y);
//		fprintf(f, "%lf, %lf\n\n", PatternProjection[id_pl].x, PatternProjection[id_pl].y);
//	}
//
//	id = PatternHullInds[PatternConvHullCount - 1];
//	id_pl = PatternHullInds[0];
//
//	fprintf(f, "%lf, %lf\n", PatternProjection[id].x, PatternProjection[id].y);
//	fprintf(f, "%lf, %lf\n\n", PatternProjection[id_pl].x, PatternProjection[id_pl].y);
//
//	fclose(f);
//
//	f = fopen("D:\\YandexDisk\\Ëèçà\\Octonus\\RealProj.txt", "w");
//	//cout << "RealConvHullCount = " << RealConvHullCount << endl;
//	for (int i = 0; i < RealConvHullCount - 1; i++)
//	{
//		id = RealHullInds[i];
//		id_pl = RealHullInds[i + 1];
//
//		fprintf(f, "%lf, %lf\n", RealProjection[id].x, RealProjection[id].y);
//		fprintf(f, "%lf, %lf\n\n", RealProjection[id_pl].x, RealProjection[id_pl].y);
//	}
//
//	id = RealHullInds[RealConvHullCount - 1];
//	id_pl = RealHullInds[0];
//
//	fprintf(f, "%lf, %lf\n", RealProjection[id].x, RealProjection[id].y);
//	fprintf(f, "%lf, %lf\n\n", RealProjection[id_pl].x, RealProjection[id_pl].y);
//
//	fclose(f);
//
//	//double* error = new double[accuracy];
//
//	net_func RealFPlus(accuracy), PatternFPlus(accuracy), errorPlus(accuracy);
//
//	VECTOR2* RealPtsPlus = new VECTOR2[RealConvHullCount];
//	VECTOR2* PatternPtsPlus = new VECTOR2[PatternConvHullCount];
//
//	for (int i = 0; i < RealConvHullCount; i++)
//	{
//		id = RealHullInds[i];
//
//		RealPtsPlus[i] = RealProjection[id];
//	}
//
//	for (int i = 0; i < PatternConvHullCount; i++)
//	{
//		id = PatternHullInds[i];
//
//		PatternPtsPlus[i] = PatternProjection[id];
//	}
//
//	Set(RealFPlus, RealConvHullCount, RealPtsPlus);
//	Set(PatternFPlus, PatternConvHullCount, PatternPtsPlus);
//
//	L2_error(errorPlus.f, PatternFPlus.f, RealFPlus.f, accuracy);
//
//	double min_value = errorPlus.f[0];
//	double s, value;
//	int min_id = 0;
//
//	for (int m = 1; m < accuracy; m++)
//	{
//		value = errorPlus.f[m];
//
//		//cout << m << "\t" << value << endl;
//
//		if (value < min_value)
//		{
//			s = m;
//			min_value = value;
//			min_id = m;
//		}
//	}
//
//	double angle = (2 * M_PI * (min_id + 0)) / accuracy;
//	cout << "min_id = " << min_id << endl;
//	cout << "PatternNorm = " << PatternNorm.x << ", " << PatternNorm.y << ", " << PatternNorm.z << endl;
//
//	SPIN SpinBestTurn(PatternNorm, angle);
//	CONFORM ConBestTurn(SpinBestTurn, v_null, 1);
//	SIMILARITY SimBestTurn(ConBestTurn);
//
//	VECTOR x(1, 0, 0);
//	SPIN Spinx(x, M_PI);
//	CONFORM Conx(Spinx, v_null, 1);
//	SIMILARITY Simx(Conx);
//
//	SPIN Spiny(y, M_PI);
//	CONFORM Cony(Spiny, v_null, 1);
//	SIMILARITY Simy(Cony);
//
//	SPIN SpinN(PatternNorm, M_PI);
//	CONFORM ConN(SpinN, v_null, 1);
//	SIMILARITY SimN(ConN);
//
//	DrawAxPolyTogether(Real, PathToWrite, "Real2", "No");
//
//	TransformPolyhedron(Real, SimBestTurn);
//	//TransformPolyhedron(Real, SimN);
//	/*TransformPolyhedron(Real, Simx);
//	TransformPolyhedron(Real, Simy);*/
//
//	RealConvHullCount = ConvexHullOfProjection(Real, PatternNorm, RealHullInds, RealProjection);
//	PatternConvHullCount = ConvexHullOfProjection(Pattern, PatternNorm, PatternHullInds, PatternProjection);
//
//	f = fopen("D:\\YandexDisk\\Ëèçà\\Octonus\\ResRealProj.txt", "w");
//	//cout << "RealConvHullCount = " << RealConvHullCount << endl;
//	for (int i = 0; i < RealConvHullCount - 1; i++)
//	{
//		id = RealHullInds[i];
//		id_pl = RealHullInds[i + 1];
//
//		fprintf(f, "%lf, %lf\n", RealProjection[id].x, RealProjection[id].y);
//		fprintf(f, "%lf, %lf\n\n", RealProjection[id_pl].x, RealProjection[id_pl].y);
//	}
//
//	id = RealHullInds[RealConvHullCount - 1];
//	id_pl = RealHullInds[0];
//
//	fprintf(f, "%lf, %lf\n", RealProjection[id].x, RealProjection[id].y);
//	fprintf(f, "%lf, %lf\n\n", RealProjection[id_pl].x, RealProjection[id_pl].y);
//
//	fclose(f);
//
//	delete[] RealProjection;
//	delete[] PatternProjection;
//	delete[] RealPtsPlus;
//	delete[] PatternPtsPlus;
//	delete[] RealHullInds;
//	delete[] PatternHullInds;
//}

void MatchingSides(POLYHEDRON* Pattern, POLYHEDRON* Real, int PatternSideId, int RealSideId, PlaneInfo* PatternInfo, PlaneInfo* RealInfo, SIMILARITY Back[])
{
	net_func RealF[2];
	net_func PatternF[2];
	net_func Error[2];
	net_func RealFPlus(accuracy), PatternFPlus(accuracy), errorPlus(accuracy);

	int* RealHullInds = new int[Real->nverts];
	int* PatternHullInds = new int[Pattern->nverts];
	int BestMinId[2];
	int sign[2];
	sign[0] = 1;
	sign[1] = -1;
	double ValErr[2];
	double Angle[2];
	VECTOR Norm[2];

	VECTOR2* RealProjection = new VECTOR2[Real->nverts];
	VECTOR2* PatternProjection = new VECTOR2[Pattern->nverts];

	int RealConvHullCount, PatternConvHullCount, id, MinId, BestIdMin;
	REAL angle;

	VECTOR BestNorm;
	const VECTOR VNull(0, 0, 0);
	VECTOR v(1, 0, 0);
	SPIN Sp(v, 0);
	VECTOR VMove(PatternInfo[PatternSideId].x_area - RealInfo[RealSideId].x_area, PatternInfo[PatternSideId].y_area - RealInfo[RealSideId].y_area, PatternInfo[PatternSideId].z_area - RealInfo[RealSideId].z_area);
	VECTOR VMoveSideCenter(-PatternInfo[PatternSideId].x_area, -PatternInfo[PatternSideId].y_area, -PatternInfo[PatternSideId].z_area);
	CONFORM ConMove(Sp, VMove, 1);
	CONFORM ConMoveBack(Sp, -VMove, 1);
	SIMILARITY SimMove(ConMove);
	SIMILARITY SimMoveBack(ConMoveBack);
	CONFORM ConMoveSideCenter(Sp, VMoveSideCenter, 1);
	SIMILARITY SimMoveSideCenter(ConMoveSideCenter);
	CONFORM ConMoveSideCenterBack(Sp, -VMoveSideCenter, 1);
	SIMILARITY SimMoveSideCenterBack(ConMoveSideCenterBack);

	Back[4] = SimMoveSideCenterBack;
	Back[0] = SimMoveBack;

	TransformPolyhedron(Real, SimMove);
	TransformPolyhedron(Real, SimMoveSideCenter);

	/*RealInfo[RealSideId].x_area += VMove.x;
	RealInfo[RealSideId].y_area += VMove.y;
	RealInfo[RealSideId].z_area += VMove.z;*/

	SPIN SpinForward, SpinBackward;
	VECTOR Z(RealInfo[RealSideId].x_area, RealInfo[RealSideId].y_area, RealInfo[RealSideId].z_area);
	VECTOR Pattern_e1, Pattern_e2;
	VECTOR Real_e1, Real_e2;
	VECTOR PatternNorm = Pattern->side[PatternSideId].plane.norm;
	VECTOR RealNorm[2];
	RealNorm[0] = Real->side[RealSideId].plane.norm;
	RealNorm[1] = -Real->side[RealSideId].plane.norm;

	/*printf("Real->side[RealSideId].plane.norm = (%lf, %lf, %lf)\n", Real->side[RealSideId].plane.norm.x, Real->side[RealSideId].plane.norm.y, Real->side[RealSideId].plane.norm.z);*/

	AddOnReper(Real_e1, Real_e2, RealNorm[0]);
	AddOnReper(Pattern_e1, Pattern_e2, PatternNorm);

	for (int i = 0; i < 2; i++)
	{
		EulerTransform(Real_e1, Real_e2, RealNorm[i],
			Pattern_e1, Pattern_e2, PatternNorm, SpinForward);
		SpinBackward = ~SpinForward;

		CONFORM ConForward(SpinForward, VNull, 1);
		SIMILARITY SimForward(ConForward);
		CONFORM ConBackward(SpinBackward, VNull, 1);
		SIMILARITY SimBackward(ConBackward);

		//DrawAxPolyTogether(Real, PathToWrite, "A1", "No");
		TransformPolyhedron(Real, SimForward);
		//DrawAxPolyTogether(Real, PathToWrite, "A2", "No");
		TransformPolyhedron(Real, SimMoveSideCenterBack);
		//DrawAxPolyTogether(Real, PathToWrite, "A3", "No");

		RealConvHullCount = ConvexHullOfProjection(Real, Pattern_e1, RealHullInds, RealProjection);
		PatternConvHullCount = ConvexHullOfProjection(Pattern, Pattern_e1, PatternHullInds, PatternProjection);

		VECTOR2* RealPts = new VECTOR2[RealConvHullCount];
		VECTOR2* PatternPts = new VECTOR2[PatternConvHullCount];

		for (int i = 0; i < RealConvHullCount; i++)
		{
			id = RealHullInds[i];

			RealPts[i] = RealProjection[id];
		}

		for (int i = 0; i < PatternConvHullCount; i++)
		{
			id = PatternHullInds[i];

			PatternPts[i] = PatternProjection[id];
		}

		RealF[i].N = accuracy;
		RealF[i].f = new double[accuracy];

		PatternF[i].N = accuracy;
		PatternF[i].f = new double[accuracy];

		Error[i].N = accuracy;
		Error[i].f = new double[accuracy];

		Set(RealF[i], RealConvHullCount, RealPts);
		Set(PatternF[i], PatternConvHullCount, PatternPts);

		L2_error(Error[i].f, PatternF[i].f, RealF[i].f, accuracy);

		MinId = find_min(Error[i].f, accuracy);
		BestMinId[i] = MinId;
		ValErr[i] = Error[i].f[MinId];
		Norm[i] = RealNorm[i];

		TransformPolyhedron(Real, SimMoveSideCenter);
		TransformPolyhedron(Real, SimBackward);
		//DrawAxPolyTogether(Real, PathToWrite, "B0", "No");

		//Angle = (2 * M_PI * MinId) / accuracy;
		
		delete[] RealPts;
		delete[] PatternPts;
	}

	if (ValErr[0] < ValErr[1])
		BestNorm = Norm[0];

	else
		BestNorm = Norm[1];

	EulerTransform(Real_e1, Real_e2, BestNorm,
		Pattern_e1, Pattern_e2, PatternNorm, SpinForward);

	SpinBackward = ~SpinForward;
	CONFORM ConBck(SpinBackward, VNull, 1);
	SIMILARITY SimBck(ConBck);

	Back[1] = SimBck;

	CONFORM ConForward(SpinForward, VNull, 1);
	SIMILARITY SimForward(ConForward);

	TransformPolyhedron(Real, SimForward);
	//TransformPolyhedron(Real, SimBck);

	//DrawAxPolyTogether(Real, PathToWrite, "B1", "No");

	//TransformPolyhedron(Real, SimForward);
	//DrawAxPolyTogether(Real, PathToWrite, "B", "No");

	TransformPolyhedron(Real, SimMoveSideCenterBack);

	RealConvHullCount = ConvexHullOfProjection(Real, PatternNorm, RealHullInds, RealProjection);
	PatternConvHullCount = ConvexHullOfProjection(Pattern, PatternNorm, PatternHullInds, PatternProjection);

	VECTOR2* RealPts = new VECTOR2[RealConvHullCount];
	VECTOR2* PatternPts = new VECTOR2[PatternConvHullCount];

	for (int i = 0; i < RealConvHullCount; i++)
	{
		id = RealHullInds[i];

		RealPts[i] = RealProjection[id];
	}

	for (int i = 0; i < PatternConvHullCount; i++)
	{
		id = PatternHullInds[i];

		PatternPts[i] = PatternProjection[id];
	}

	int j = 0;
	Set(RealF[j], RealConvHullCount, RealPts);
	Set(PatternF[j], PatternConvHullCount, PatternPts);

	L2_error(Error[j].f, PatternF[j].f, RealF[j].f, accuracy);

	MinId = find_min(Error[j].f, accuracy);
	angle = (2 * M_PI * MinId) / accuracy;

	/*cout << angle << " = angle\n";
	cout << MinId << " = MinId\n";
	cout << Error[j].f[MinId] << " = find_min\n";
	cout << "PatternNorm = " << PatternNorm.x << ", " << PatternNorm.y << ", " << PatternNorm.z << endl;*/
	TransformPolyhedron(Real, SimMoveSideCenter);

	for (int i = 0; i < 2; i++)
	{
		Angle[i] = angle * sign[i];
		double A = Angle[i];
		printf("Angle[i] = %lf\n", A);
		SPIN SpinTurn(PatternNorm, A);
		CONFORM ConTurn(SpinTurn, VNull, 1);
		SIMILARITY SimTurn(ConTurn);
		SpinBackward = ~SpinTurn;

		CONFORM ConBackward(SpinBackward, VNull, 1);
		SIMILARITY SimBackward(ConBackward);

		/*RealConvHullCount = ConvexHullOfProjection(Real, PatternNorm, RealHullInds, RealProjection);
		PatternConvHullCount = ConvexHullOfProjection(Pattern, PatternNorm, PatternHullInds, PatternProjection);
		PrintProjection(RealConvHullCount, RealHullInds, RealProjection, "ARealProj", i);
		PrintProjection(PatternConvHullCount, PatternHullInds, PatternProjection, "APatternProj", i);*/
		TransformPolyhedron(Real, SimTurn);
		/*RealConvHullCount = ConvexHullOfProjection(Real, PatternNorm, RealHullInds, RealProjection);
		PatternConvHullCount = ConvexHullOfProjection(Pattern, PatternNorm, PatternHullInds, PatternProjection);
		PrintProjection(RealConvHullCount, RealHullInds, RealProjection, "BRealProj", i);
		PrintProjection(PatternConvHullCount, PatternHullInds, PatternProjection, "BPatternProj", i);*/
		TransformPolyhedron(Real, SimMoveSideCenterBack);
		/*RealConvHullCount = ConvexHullOfProjection(Real, PatternNorm, RealHullInds, RealProjection);
		PatternConvHullCount = ConvexHullOfProjection(Pattern, PatternNorm, PatternHullInds, PatternProjection);
		PrintProjection(RealConvHullCount, RealHullInds, RealProjection, "CRealProj", i);
		PrintProjection(PatternConvHullCount, PatternHullInds, PatternProjection, "CPatternProj", i);*/

		RealConvHullCount = ConvexHullOfProjection(Real, PatternNorm, RealHullInds, RealProjection);
		PatternConvHullCount = ConvexHullOfProjection(Pattern, PatternNorm, PatternHullInds, PatternProjection);

		RetMatchingError(Pattern, Real, PatternNorm);

		/*Polar Real_p_s(accuracy, Real->nverts, RealProjection, RealConvHullCount, RealHullInds);
		Polar Pattern_p_s(accuracy, Pattern->nverts, PatternProjection, PatternConvHullCount, PatternHullInds);*/

		VECTOR2* RealPts1 = new VECTOR2[RealConvHullCount];
		VECTOR2* PatternPts1 = new VECTOR2[PatternConvHullCount];

		for (int k = 0; k < RealConvHullCount; k++)
		{
			id = RealHullInds[k];

			RealPts1[k] = RealProjection[id];
		}

		for (int k = 0; k < PatternConvHullCount; k++)
		{
			id = PatternHullInds[k];

			PatternPts1[k] = PatternProjection[id];
		}

		Set(RealF[i], RealConvHullCount, RealPts1);
		Set(PatternF[i], PatternConvHullCount, PatternPts1);

		L2_error(Error[i].f, PatternF[i].f, RealF[i].f, accuracy);

		//MinId = find_min(Error[i].f, accuracy);
		//BestMinId[i] = MinId;
		ValErr[i] = Error[i].f[0];

		TransformPolyhedron(Real, SimMoveSideCenter);
		TransformPolyhedron(Real, SimBackward);

		delete[] RealPts1;
		delete[] PatternPts1;
	}

	if (ValErr[0] < ValErr[1])
		angle = Angle[0];

	else
		angle = Angle[1];

	printf("ValErr[0] = %lf \t ValErr[1] = %lf\n", ValErr[0], ValErr[1]);
	printf("Angle[0] = %lf \t Angle[1] = %lf\n", Angle[0], Angle[1]);

	SPIN SpinBestTurn(PatternNorm, angle);
	CONFORM ConBestTurn(SpinBestTurn, VNull, 1);
	SIMILARITY SimBestTurn(ConBestTurn);
	SpinBackward = ~SpinBestTurn;

	CONFORM ConBackward(SpinBackward, VNull, 1);
	SIMILARITY SimBackward(ConBackward);

	TransformPolyhedron(Real, SimBestTurn);
	TransformPolyhedron(Real, SimMoveSideCenterBack);

	delete[] RealPts;
	delete[] PatternPts;
	delete[] RealProjection;
	delete[] PatternProjection;
	delete[] RealHullInds;
	delete[] PatternHullInds;

	Back[2] = SimBackward;
	Back[3] = SimMoveSideCenter;
}

void Sort(OrderedSide* S, int n);
void Sort(OrderedSide* S, int n)
{
	int id;
	double area;

	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (S[i].area < S[j].area)
			{
				area = S[i].area;
				id = S[i].id;

				S[i].area = S[j].area;
				S[i].id = S[j].id;

				S[j].area = area;
				S[j].id = id;
			}

		}
	}
}

void GetErrorNorm(POLYHEDRON* PatternPoly, POLYHEDRON* RealPoly, PlaneInfo* PatternInfo, PlaneInfo* RealInfo, double Err[], int n, int m, int PatternSideId, int RealSideId);
void GetErrorNorm(POLYHEDRON* PatternPoly, POLYHEDRON* RealPoly, PlaneInfo* PatternInfo, PlaneInfo* RealInfo, double Err[], int n, int m, int PatternSideId, int RealSideId)
{
	int RealConvHullCount, PatternConvHullCount;
	int* RealHullInds = new int[RealPoly->nverts];
	int* PatternHullInds = new int[PatternPoly->nverts];
	VECTOR2* RealProjection = new VECTOR2[RealPoly->nverts];
	VECTOR2* PatternProjection = new VECTOR2[PatternPoly->nverts];

	double ErrSumm = 0.0, Error = 0.0;
	SIMILARITY Back[5];
	VECTOR NormVector;

	MatchingSides(PatternPoly, RealPoly, PatternSideId, RealSideId, PatternInfo, RealInfo, Back);

	for (int i = 0; i < 6; i++)
	{
		NormVector = PatternPoly->side[i].plane.norm;

		Error = RetMatchingError(PatternPoly, RealPoly, NormVector);
		ErrSumm += Error * Error;

		DrawAxPolyTogether(RealPoly, PathToWrite, (string(to_string(n)) + string(to_string(m))).c_str(), "No");
	}

	Err[n * 6 + m] = sqrt(ErrSumm);

	//cout << sqrt(ErrSumm) << endl;

	TransformPolyhedron(RealPoly, Back[3]);

	/*if (n == 2 && m == 1)
		DrawAxPolyTogether(RealPoly, PathToWrite, "Real0", "No");*/

	TransformPolyhedron(RealPoly, Back[2]);

	/*if (n == 2 && m == 1)
		DrawAxPolyTogether(RealPoly, PathToWrite, "Real1", "No");*/

	TransformPolyhedron(RealPoly, Back[1]);
	TransformPolyhedron(RealPoly, Back[4]);
	TransformPolyhedron(RealPoly, Back[0]);

	/*if(n == 2 && m == 1)
		DrawAxPolyTogether(RealPoly, PathToWrite, "Real2", "No");*/

}

//void ComparissonTest03(POLYHEDRON* PatternPoly, POLYHEDRON* RealPoly)
//{
//	system("del /q D:\\YandexDisk\\Octonus\\*.*");
//
//	/*DrawAxPolyTogether(PatternPoly, PathToWrite, "PatternBfZr", "No");
//	DrawAxPolyTogether(RealPoly, PathToWrite, "RealBfZr", "No");*/
//
//	PlaneInfo* PatternInfo = new PlaneInfo[PatternPoly->nsides];
//	MassCenterZeroing(PatternPoly, PatternInfo);
//
//	PlaneInfo* RealInfo = new PlaneInfo[RealPoly->nsides];
//	MassCenterZeroing(RealPoly, RealInfo);
//
//
//	for (int i = 0; i < PatternPoly->nsides; i++)
//	{
//		FindSubPlaneInfo(PatternInfo, PatternPoly, i);
//	}
//
//	for (int i = 0; i < RealPoly->nsides; i++)
//	{
//		FindSubPlaneInfo(RealInfo, RealPoly, i);
//	}
//
//	/*DrawAxPolyTogether(PatternPoly, PathToWrite, "PatternZr", "No");
//	DrawAxPolyTogether(RealPoly, PathToWrite, "RealZr", "No");*/
//
//	OrderedSide* PatternSides = new OrderedSide[PatternPoly->nsides];
//	OrderedSide* RealSides = new OrderedSide[RealPoly->nsides];
//
//	for (int i = 0; i < PatternPoly->nsides; i++)
//	{
//		PatternSides[i].id = i;
//		PatternSides[i].area = PatternInfo[i].area;
//	}
//
//	for (int i = 0; i < RealPoly->nsides; i++)
//	{
//		RealSides[i].id = i;
//		RealSides[i].area = RealInfo[i].area;
//	}
//
//	Sort(RealSides, RealPoly->nsides);
//	Sort(PatternSides, PatternPoly->nsides);
//
//	for (int i = 0; i < 6; i++)
//		cout << RealSides[i].area << "\t" << PatternSides[i].area << endl;
//
//	int id;
//	int len;// = length(PatternPoly->side[PatternSides[0].id].vlist);
//
//	for (int i = 0; i < PatternPoly->nsides; i++)
//	{
//		len = length(PatternPoly->side[PatternSides[i].id].vlist);
//		//cout << PatternInfo[PatternSides[i].id].area << endl;
//		//printf("i = %d, id = %d\n", i, PatternSides[i].id);
//
//		const string FullNameString = string(PathToWrite) + string("P") + string(to_string(i));
//		const char* FullName = FullNameString.c_str();
//
//		FILE* g = fopen(FullName, "w");
//
//		const string FullNameString2 = string(PathToWrite) + string("PCA") + string(to_string(i));
//		const char* FullName2 = FullNameString2.c_str();
//
//		FILE* a = fopen(FullName2, "w");
//
//		fprintf(a, "%lf, %lf, %lf\n", PatternInfo[PatternSides[i].id].x_area, PatternInfo[PatternSides[i].id].y_area, PatternInfo[PatternSides[i].id].z_area);
//
//		fclose(a);
//
//		const string FullNameString3 = string(PathToWrite) + string("PC") + string(to_string(i));
//		const char* FullName3 = FullNameString3.c_str();
//
//		a = fopen(FullName3, "w");
//
//		fprintf(a, "%lf, %lf, %lf\n", PatternInfo[PatternSides[i].id].x, PatternInfo[PatternSides[i].id].y, PatternInfo[PatternSides[i].id].z);
//
//		fclose(a);
//
//		for (int j = 0; j < len; j++)
//		{
//			id = PatternPoly->side[PatternSides[i].id].vlist[j];
//
//			fprintf(g, "%lf, %lf, %lf\n", PatternPoly->vertex[id].x, PatternPoly->vertex[id].y, PatternPoly->vertex[id].z);
//		}
//
//		fclose(g);
//
//	}
//
//	DrawAxPolyTogether(RealPoly, PathToWrite, "RealBfM", "No");
//
//	const string FullNameString4 = string(PathToWrite) + string("RCABfM") + string(to_string(0));
//	const char* FullName4 = FullNameString4.c_str();
//
//	FILE* a4 = fopen(FullName4, "w");
//
//	fprintf(a4, "%lf, %lf, %lf\n", RealInfo[RealSides[0].id].x_area, RealInfo[RealSides[0].id].y_area, RealInfo[RealSides[0].id].z_area);
//
//	fclose(a4);
//
//	const string FullNameString5 = string(PathToWrite) + string("RCBfM") + string(to_string(0));
//	const char* FullName5 = FullNameString5.c_str();
//
//	a4 = fopen(FullName5, "w");
//
//	fprintf(a4, "%lf, %lf, %lf\n", RealInfo[RealSides[0].id].x, RealInfo[RealSides[0].id].y, RealInfo[RealSides[0].id].z);
//
//	fclose(a4);
//
//	MatchingSides(PatternPoly, RealPoly, PatternSides[1].id, RealSides[1].id, PatternInfo, RealInfo);
//
//	DrawAxPolyTogether(RealPoly, PathToWrite, "RealBfM", "No");
//
//	for (int i = 0; i < RealPoly->nsides; i++)
//	{
//		FindSubPlaneInfo(RealInfo, RealPoly, i);
//	}
//
//	const string FullNameString1 = string(PathToWrite) + string("SecondSide");
//	const char* FullName1 = FullNameString1.c_str();
//
//	FILE* g = fopen(FullName1, "w");
//
//	for (int i = 0; i < 10; i++)
//	{
//		id = RealPoly->side[0].vlist[i];
//
//		fprintf(g, "%lf, %lf, %lf\n", RealPoly->vertex[id].x, RealPoly->vertex[id].y, RealPoly->vertex[id].z);
//	}
//
//	fclose(g);
//
//	for (int i = 0; i < 8; i++)
//	{
//		len = length(RealPoly->side[RealSides[i].id].vlist);
//		//cout << RealInfo[RealSides[i].id].area << endl;
//		//printf("i = %d, id = %d\n", i, RealSides[i].id);
//
//		const string FullNameString = string(PathToWrite) + string("R") + string(to_string(i));
//		const char* FullName = FullNameString.c_str();
//
//		FILE* g = fopen(FullName, "w");
//
//		const string FullNameString2 = string(PathToWrite) + string("RCA") + string(to_string(i));
//		const char* FullName2 = FullNameString2.c_str();
//
//		FILE* a = fopen(FullName2, "w");
//
//		fprintf(a, "%lf, %lf, %lf\n", RealInfo[RealSides[i].id].x_area, RealInfo[RealSides[i].id].y_area, RealInfo[RealSides[i].id].z_area);
//
//		fclose(a);
//
//		const string FullNameString3 = string(PathToWrite) + string("RC") + string(to_string(i));
//		const char* FullName3 = FullNameString3.c_str();
//
//		a = fopen(FullName3, "w");
//
//		fprintf(a, "%lf, %lf, %lf\n", RealInfo[RealSides[i].id].x, RealInfo[RealSides[i].id].y, RealInfo[RealSides[i].id].z);
//
//		fclose(a);
//
//
//		for (int j = 0; j < len; j++)
//		{
//			id = RealPoly->side[RealSides[i].id].vlist[j];
//
//			fprintf(g, "%lf, %lf, %lf\n", RealPoly->vertex[id].x, RealPoly->vertex[id].y, RealPoly->vertex[id].z);
//		}
//
//		fclose(g);
//	}
//
//	FILE* f = fopen("D:\\YandexDisk\\Octonus\\PtsPattern.txt", "w");
//	len = length(PatternPoly->side[PatternSides[0].id].vlist);
//
//	for (int i = 0; i < len; i++)
//	{
//
//		id = PatternPoly->side[PatternSides[0].id].vlist[i];
//
//		fprintf(f, "%lf, %lf, %lf\n", PatternPoly->vertex[id].x, PatternPoly->vertex[id].y, PatternPoly->vertex[id].z);
//	}
//
//	fclose(f);
//
//	f = fopen("D:\\YandexDisk\\Octonus\\PtsReal.txt", "w");
//	len = length(RealPoly->side[RealSides[0].id].vlist);
//
//	for (int i = 0; i < len; i++)
//	{
//		id = RealPoly->side[RealSides[0].id].vlist[i];
//
//		fprintf(f, "%lf, %lf, %lf\n", RealPoly->vertex[id].x, RealPoly->vertex[id].y, RealPoly->vertex[id].z);
//	}
//
//	fclose(f);
//
//	DrawAxPolyTogether(PatternPoly, PathToWrite, "Pattern", "No");
//	DrawAxPolyTogether(RealPoly, PathToWrite, "Real", "No");
//
//	RealInfo->SubInfo.clear();
//	PatternInfo->SubInfo.clear();
//	delete[] PatternSides;
//	delete[] RealSides;
//	delete[] PatternInfo;
//	delete[] RealInfo;
//}

void ComparissonTest03(POLYHEDRON* PatternPoly, POLYHEDRON* RealPoly)
{
	system("del /q D:\\YandexDisk\\Ëèçà\\Octonus\\*.*");

	SIMILARITY Back[5];
	double Err[36];
	double MinError = 0.0;
	int MinI, MinJ;

	PlaneInfo* PatternInfo = new PlaneInfo[PatternPoly->nsides];
	MassCenterZeroing(PatternPoly, PatternInfo);

	PlaneInfo* RealInfo = new PlaneInfo[RealPoly->nsides];
	MassCenterZeroing(RealPoly, RealInfo);

	DrawAxPolyTogether(PatternPoly, PathToWrite, "Pattern", "No");
	DrawAxPolyTogether(RealPoly, PathToWrite, "OriReal", "No");


	for (int i = 0; i < PatternPoly->nsides; i++)
	{
		FindSubPlaneInfo(PatternInfo, PatternPoly, i);
	}

	for (int i = 0; i < RealPoly->nsides; i++)
	{
		FindSubPlaneInfo(RealInfo, RealPoly, i);
	}

	OrderedSide* PatternSides = new OrderedSide[PatternPoly->nsides];
	OrderedSide* RealSides = new OrderedSide[RealPoly->nsides];

	for (int i = 0; i < PatternPoly->nsides; i++)
	{
		PatternSides[i].id = i;
		PatternSides[i].area = PatternInfo[i].area;
	}

	for (int i = 0; i < RealPoly->nsides; i++)
	{
		RealSides[i].id = i;
		RealSides[i].area = RealInfo[i].area;
	}

	Sort(RealSides, RealPoly->nsides);
	Sort(PatternSides, PatternPoly->nsides);

	for (int i = 0; i < 6; i++)
	{
		PrintCurrentSide(PatternPoly, PatternSides[i].id, "P" + to_string(i));
		PrintCurrentSide(RealPoly, RealSides[i].id, "R" + to_string(i));

		cout << PatternSides[i].area << endl;
		cout << PatternSides[i].id << endl;

		for (int j = 0; j < 6; j++)
		{
			GetErrorNorm(PatternPoly, RealPoly, PatternInfo, RealInfo, Err, i, j, PatternSides[i].id, RealSides[j].id);
			printf("%d to %d mapping error is %lf\n", i, j, Err[i * 6 + j]);

			if (i == 0 && j == 0)
			{
				MinError = Err[i * 6 + j];
				MinI = i;
				MinJ = j;
			}

			else if (MinError > Err[i * 6 + j])
			{
				MinError = Err[i * 6 + j];
				MinI = i;
				MinJ = j;
			}
		}
	}

	PrintCurrentSide(RealPoly, RealSides[6].id, "R" + to_string(6));
	PrintCurrentSide(RealPoly, RealSides[7].id, "R" + to_string(7));
	PrintCurrentSide(RealPoly, RealSides[8].id, "R" + to_string(8));

	printf("Best mapping is %d to %d mapping error is %lf\n", MinI, MinJ, MinError);
	MinI = PatternSides[MinI].id;
	MinJ = RealSides[MinJ].id;
	MatchingSides(PatternPoly, RealPoly, MinI, MinJ, PatternInfo, RealInfo, Back);
	DrawAxPolyTogether(RealPoly, PathToWrite, "ResReal", "No");

	PatternInfo->SubInfo.clear();
	RealInfo->SubInfo.clear();
	delete[] PatternInfo;
	delete[] RealInfo;
	delete[] PatternSides;
	delete[] RealSides;
}