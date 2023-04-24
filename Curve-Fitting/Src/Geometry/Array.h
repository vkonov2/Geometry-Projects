#ifndef __ARRAY_H
#define __ARRAY_H

#include "Real.h"
#include "Object.h"

#pragma pack (push, 4)

class   SPECIAL_ARRAY
{
    protected:
        REAL resize;
        int increase;
        POINTER * array;
        int n, nmax;
    private:
        void    resizearray ( );
    public:
        SPECIAL_ARRAY ( );
        SPECIAL_ARRAY ( int f, REAL r = 2, int inc = 1);
        virtual ~ SPECIAL_ARRAY ( );
        virtual void    add ( POINTER );
        virtual void    remove ( );     // remove last object
        POINTER removeAt ( int );       // remove object
        void    bubbleFor ( int );      // bubble object
        virtual int     length ( );
        POINTER object ( int n );
        POINTER replaceAt ( POINTER, int );     // replace object
        void    insertAt  ( POINTER, int );
        void    empty ( );
        void    sort ( int (*) ( const void *, const void * ) );
    friend  class OBJECT_ARRAY;
};

// ������, ��������������� ������������� ��� ����������.
template <class Type>
class SpecialArray : private SPECIAL_ARRAY
{
    public:
        SpecialArray ( ) { }

        // �������� ������� � ����� �������
        void    Add (Type object) { SPECIAL_ARRAY:: add (object); }
        // ������� ������ �� ������
        Type    RemoveAt (int nIndex) { return (Type) SPECIAL_ARRAY:: removeAt (nIndex); }
        // �������� ������ � ������ ������
        void    Bubble   (int nIndex) { SPECIAL_ARRAY:: bubbleFor (nIndex); }
        // ���������� ���������
        int     Length   ( ) { return SPECIAL_ARRAY:: length (); }
        // ������ � ��������
        Type    Object   (int nIndex) { return (Type) SPECIAL_ARRAY:: object(nIndex); }
        Type    operator [] (int nIndex) { return (Type) SPECIAL_ARRAY:: object(nIndex); }
        // �������� �������
        Type    ReplaceAt (Type object, int nIndex) { return (Type) SPECIAL_ARRAY:: replaceAt (object, nIndex); }
        // �������� ������� �����
        void    InsertAt  (Type object, int nIndex) { SPECIAL_ARRAY:: insertAt (object, nIndex); }
        // �������� ������ ���������
        void    Empty ( ) { SPECIAL_ARRAY:: empty (); }
		// ����������� ������ ���������
        void    Sort (int (*pCompare) (Type * Elem1, Type * Elem2))
		{
			SPECIAL_ARRAY:: sort ((int (*)(const void *,const void *)) pCompare);
		}
};


class   DATA_ARRAY :
public  OBJECT
{
    public:
        SPECIAL_ARRAY   data;
};

class   STRING_ARRAY :
public  SPECIAL_ARRAY
{
    public:
        STRING_ARRAY ( );
        ~ STRING_ARRAY ( );
        void    addstring ( const char * );
        void    removestring ( );
        const char *  get ( int n );
};

class   OBJECT_ARRAY :
public  OBJECT
{
        SPECIAL_ARRAY array;
    public:
        OBJECT_ARRAY ( );
        ~ OBJECT_ARRAY ( );
        void    insertObjectAt ( OBJECT *, int );
        void    addObject      ( OBJECT * );
        void    removeObject   ( OBJECT * );
        OBJECT * removeObjectAt ( int i );
        void    empty          ( );
        OBJECT * objectAt      ( int );
        int     length ( );
};

class   OBJECT_BAG :
public  OBJECT
{
        OBJECT_ARRAY  objects;
        SPECIAL_ARRAY flags;
        char *  _name;
    public:
        OBJECT_BAG ( );
        ~ OBJECT_BAG ( );
        void    insertObjectAt ( OBJECT *, int flag, int );
        void    addObject      ( OBJECT *, int flag );
        void    removeObject   ( OBJECT * );
        void    removeObjectAt ( int i );
        void    empty          ( );
        OBJECT * objectAt      ( int );
        int     dataAt         ( int );
        int     replaceDataAt  ( int flag, int );
        int     length ( );

        const char * name ( );
        void    rename ( const char * );
        int     type ( ) const;
        //...
        int     size  ( STREAM * );
        void    read  ( STREAM * );
        void    write ( STREAM * );
        void    awake ( );
        const char * classname ( );
        //...
};

#pragma pack (pop)

#endif // __ARRAY_H
