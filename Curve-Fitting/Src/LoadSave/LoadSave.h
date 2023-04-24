#pragma once

#include <iostream>
#include <vector>
// #include "../ArrayList.h"
#include "../PlaneGeometry/PlaneGeometryFunctions.h"
#include "../Geometry/Polyhedr.h"

using namespace std;
/*------------------------------------------------------------------------------------------------------
IO_RET_CODE PrintTextPolyhedron(const POLYHEDRON* pPoly, const char* prefix, const char* fname);
	функция печатает многогранник в файл prefix + fname

	pPoly - многогранник

POLYHEDRON* LoadTextPolyhedron(const char* prefix, const char* fname);
	функция читает многогранник из файла prefix + fname

IO_RET_CODE write_OFF(const POLYHEDRON* pPoly, const char* prefix, const char* fname);
	функция печатает многогранник в файл формата OFF с именем prefix + fname 

	pPoly - многогранник

POLYHEDRON* read_OFF(const char* prefix, const char* fname);
	функция читает многогранник из файла формата OFF с именем prefix + fname 
------------------------------------------------------------------------------------------------------*/

enum IO_RET_CODE {
	IO_OK = 0,
	IO_OPEN_ERROR = -1,
	IO_READ_ERROR = -2,
	IO_WRITE_ERROR = -3,
	IO_ALLOCATE_MEMORY = -4,
};

class FILEPTR {
	FILE *f;
public:
	FILEPTR(FILE *_f = nullptr) { f = _f; }
	FILE* operator=(FILE *_f) { if (f) fclose(f); f = _f; return f; }
	FILE * operator *() { return f; }
	operator FILE*() { return f; }
	operator bool() { return (f != nullptr); }
	bool operator!() { return (f == nullptr); }
	~FILEPTR() { if (f) fclose(f); }
};

FILE * OpenNamedFile(const char *mode, const char *format, ...);

int length(const PHINDEX* list);

IO_RET_CODE PrintTextPolyhedron(const POLYHEDRON* pPoly, const char* prefix, const char* fname);
POLYHEDRON* LoadTextPolyhedron(const char* prefix, const char* fname);
IO_RET_CODE write_OFF(const POLYHEDRON* pPoly, const char* prefix, const char* fname);
POLYHEDRON* read_OFF(const char* prefix, const char* fname);
POLYHEDRON* SimpleLoadTextPolyhedron(const char* prefix, const char* fname);