#ifndef _WLOGGER_H
#define _WLOGGER_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>



enum LOG_MODES {
	LM_NOLOG = 0,
    LM_BASE = 1,    // самая базовая информация
    LM_VERB = 2,    // более подробно (вызовы функций и методов)
    LM_DETAIL = 3,  // детальная информация (процесс вычислений и сообщения о принятых решениях)
    LM_DEBUG = 4,   // отладочные данные 
    LM_MAX = 5     // 
};



class WLogger
{
protected:
    FILE *flog;
    char fname[512];

    int  maxLevel;
    bool local;    
    bool disableLocal;
    
    void SmartOpen() {
        fopen_s(&flog, fname, "w");
        if ( flog ) {
            fputc('\n',flog);
            fclose(flog);
        }
        flog = 0;
    }


public:

	WLogger() {
		flog = nullptr;
		fname[0] = 0;
	}
    
	void Set(const char *f, LOG_MODES mode = LM_MAX, bool dis_loc = false) {
		maxLevel = LM_NOLOG;
		SetMaxLevel(mode);
		disableLocal = dis_loc;
		if (mode == LM_NOLOG) {
			flog = 0;
			return;
		}
		local = false;
		strcpy_s(fname, f); fname[511] = 0;
		SmartOpen();
	}


    WLogger( const char *f, LOG_MODES mode = LM_MAX, bool dis_loc = false ) {
		Set(f, mode, dis_loc);
    } 

	WLogger(const char *mode, const char *f) {
		maxLevel = LM_NOLOG;
		SetMaxLevel(LM_MAX);
		strcpy_s(fname, f); fname[511] = 0;
		fopen_s(&flog, fname, mode);
	}



    ~WLogger() { Close(); }

	int SetMaxLevel(int mlevel) { int m = maxLevel;  maxLevel = mlevel; return m; }

    FILE * GetLog ( int level = LM_MAX + 1 ) {
        if ( maxLevel == LM_NOLOG ) return 0;
		if ( level == LM_NOLOG) return 0;
		if ( level > maxLevel ) return 0;
        if ( flog == 0 ) fopen_s(&flog,fname,"a");
        return flog;
    }

    void Close () { if (flog) fclose(flog); flog = 0;  }
    void Flush () { if (flog) fflush(flog); }

    void SetLocal( bool v ) { local = v; }
    bool Local () { return local; }
    FILE * GetLocal() { return (local && !disableLocal )? GetLog(LM_BASE) : 0; }

    void Print ( int level, const char *format, ... )
    {
        FILE *f = GetLog( level );
        if ( f ) {
            va_list list;
            va_start( list, format );
            vfprintf( f, format, list );
            va_end ( list );
        }
    }
   
    void PrintLocal ( const char *format, ... )
    {
        if ( !local || disableLocal ) return;
        FILE *f = GetLog( LM_BASE );
        if ( f ) {
            va_list list;
            va_start( list, format );
            vfprintf( f, format, list );
            va_end ( list );
        }
    }

	void PrintConditional (bool condition, const char *format, ...)
	{
		if (!condition) return;
		FILE *f = GetLog(LM_BASE);
		if (f) {
			va_list list;
			va_start(list, format);
			vfprintf(f, format, list);
			va_end(list);
		}
	}


};

//#ifndef  PrintLog 
//#define  PrintLog
//#define  BeginLog(x) if ( x.GetLog() ) fprintf( x.GetLog(),
//#define  EndLog   );  
//#endif



inline bool OpenWrite ( FILE **f, const char *fname ) {
    return fopen_s( f, fname, "w" ) == 0; 
}





class MemLogger : public WLogger
{
    int cnt;

public:
    MemLogger ( const char *f ) : WLogger(f,LM_DEBUG ) { cnt = 0; }
    void Set ( int n, void *p, const char *s ) {
        Print ( maxLevel, " %p  %05d allocate %d %s\n",p, ++cnt, n, s );
    }
    void Del ( void *p ) {
        Print ( maxLevel, " %p  %05d delete\n",p, ++cnt );
    } 
    void DelA( void *p ) {
        Print ( maxLevel, " %p  %05d delete []\n",p, ++cnt );
    } 
};








#endif // _WLOGGER_H
