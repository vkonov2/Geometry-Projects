#ifndef _WEXEPTION_H_
#define _WEXEPTION_H_

//---------------------------------------------------------------------------------------
//   ����������� ����� ���������� ��� ����������� ����
//---------------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#ifndef ZZ
extern int LINENUM;
extern const char * FILENAME;
#define ZZ  {LINENUM=__LINE__; FILENAME = __FILE__; } 
#endif


#define MSGLEN 128

class WException
{
private:
	char msg[MSGLEN];
	int  errn;

public:
	//WException(int en, const char *s) {
    //    strncpy_s(msg,MSGLEN,s,MSGLEN-1); 
	//	  errn = en;
	//}

    WException(int en, const char *format, ... ) {
        va_list list;
        va_start( list, format );
        vsprintf_s( msg, MSGLEN, format, list );
        va_end ( list );
    	errn = en;
	}


	int GetError() { return errn; } 
	const char * GetErrMsg() { return msg; }

};

enum WExceptionCodes {
    EX_OK,
    EX_MEMORY,
    EX_FILE,
    EX_DATA,
    EX_ASSERT,
    EX_UNKNOWN
};


//inline void WAssert(int v, const char *text) { if (!v) throw new WException(EX_ASSERT,text); } 


inline void WAssert ( bool v, const char *format, ... ) {
    if (v) return;
    char buf[128];
    va_list list;
    va_start( list, format );
    vsprintf_s( buf, 128, format, list );
    va_end ( list );
    if (!v) throw new WException(EX_ASSERT,buf);
}



#endif // _WEXEPTION_H_

