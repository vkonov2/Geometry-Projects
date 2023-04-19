#ifndef _ARRAYLIST_H_
#define _ARRAYLIST_H_


#include <stdarg.h>

#ifndef max
#define max(a,b)   (((a)>(b)) ? (a) : (b))
#endif


#include "WException.h"
#include "WLogger.h"
//extern WLogger logger;
//extern MemLogger mlog;

#include <stdio.h>
#include <stdlib.h>

#ifndef ZZ
extern int LINENUM;
extern const char * FILENAME;
#define ZZ  {LINENUM=__LINE__; FILENAME = __FILE__; } 
#endif



//---------------------------------------------------------------------------------------
//   универсальный массив-контейнер для собственных нужд
//---------------------------------------------------------------------------------------

//#define DEBUG_IND_CHECK


template <class Type>
class WArray 
{
protected:
    Type *v;
    int  size;
    int  maxsize;
	void Allocate(int n) { n = max(n,8);  v = new Type[n];  maxsize = n; }
public:
    // абсолютно пустой массив
    WArray () : v(0), size(0), maxsize(0) {}
    
    // пустой массив заданного размера с заданным запасом
    WArray ( int n, int m=0 )  { Allocate( n + m ); size = n; }
    
    // конструктор копии 
    WArray ( const WArray<Type> &b ) { Allocate( b.MaxSize() );  
	size = FillFrom( b.Data(), b.Size() ); 
	} 


    void Swap( WArray<Type> &wa ) {
        Type * vv = v; v = wa.v; wa.v = vv;
        int k = size; size = wa.size; wa.size = k;
        k = maxsize; maxsize = wa.maxsize; wa.maxsize = k;
    }
 
    // изменение размера (при увеличении пустое место дополнительно не резервируется)
	void Resize(int n) {
		if (n <= maxsize) { size = n; return; }
		Type *p = v;
		Allocate(n);  size = n;
		if (p) { /*mlog.DelA(p);*/  delete[] p; }
	}

    void Resize ( int n, bool with_copy ) {
        if ( n <= maxsize ) { size = n; return; }
        Type *p = v;
        Allocate ( n );  size = n;
        if ( with_copy && p ) { for (int i=0; i<size; i++ ) v[i] = p[i]; }
        
        if ( p ) { /*mlog.DelA(p);*/  delete [] p; }
    }

    // установка заданных размеров без копирования
    void SetSize ( int n, int m ) {
        if ( v ) delete [] v;
        Allocate( n + m ); size = n;
    }

	// установка заданных размеров без копирования и ненужного выделения памяти
	void EnsureSize(int n, int m) {
		if (n + m <= maxsize) { size = n; return; }
		SetSize(n, m);
	}


	void RemoveFirstValue(const Type &val) {
		for (int i = 0; i < Size(); i++) {
			if (v[i] == val) {
				if (i<size-1) v[i] = v[size-1];
				size--;
				return;
			}
		}
	}

    void RemoveAllValues ( const Type &val ) {
        int cnt = 0;
        for (int i=0; i < Size(); i++  ) { 
            if ( v[i] != val ) {
                v[cnt++] = v[i];
            }
        }
        Resize( cnt, true );
    }

    void RemoveAllValues ( const Type &val, int (*cmp)(const Type &, const Type &) ) {
        int cnt = 0;
        for (int i=0; i < Size(); i++  ) { 
            if ( cmp(v[i],val)!=0 ) {
                v[cnt++] = v[i];
            }
        }
        Resize( cnt, true );
    }


    // изменение размера всего буфера (всегда с копией), размер массива сохраняется
    void Remaxsize ( int n ) {
        if ( n <= size ) n = size;
        if ( n == maxsize ) return;
        Type *p = v;
        Allocate ( n );  
        for (int i=0; i<size; i++ ) v[i] = p[i]; 
        if ( p ) { delete [] p; }
    }

    // освобождение незанятой области
    void Truncate () { Remaxsize ( size ); }

    // заполнение из другого источника 
    // если n>0, то размер устанавливается в n, 
    // иначе копируется по текущему размеру
    int FillFrom ( const Type * data, int n=-1 ) {
        if ( n <= 0 ) n = size;
        Resize ( n );    
        for (int i=0; i<size; i++)  v[i] = data[i];
		return size;
    }
    
    // заполнение константой 
    // если n>0, то размер устанавливается в n, 
    // иначе заполняется по текущему размеру
    void Set ( const Type & x, int n=-1 ) {
        if ( n <= 0 ) n = size;
        Resize ( n );    
        for (int i=0; i<n; i++)  v[i] = x;
    }   

    // доступ к элементам
#ifdef DEBUG_IND_CHECK 
    Type & operator[]( int i ) { 
        if ( i<0 || i>=size ) {
            char s[128];  sprintf_s(s,128,"WArray index failure: size %d index %d, file %s line %d",size,i, FILENAME, LINENUM );
            WAssert ( false, s);
        }
        return v[i]; 
    } 
#else
    Type & operator[]( int i ) { return v[i]; } 
#endif
	

#ifdef DEBUG_IND_CHECK 
    const Type & operator[]( int i ) const { 
        if ( i<0 || i>=size ) {
            char s[128];  sprintf_s(s,128,"WArray index failure: size %d index %d, file %s line %d",size,i, FILENAME, LINENUM );
            WAssert ( false, s);
        }
        return v[i]; 
    } 
#else
    const Type & operator[]( int i ) const { return v[i]; } 
#endif

       Type * Data() { return v; }
	   const Type * Data() const { return v; }

    // добавление элемента в конец, возвращает размер
    int Add ( const Type & x ) {
        if ( size >= maxsize ) Remaxsize ( size + size/2 + 1 );
        v[size++] = x; 
        return size;
    }

    // добавление элемента в конец при условии, что такого элемента не было
    int AddUnique ( const Type & x ) {
        int i;
        if ( !SearchFirst (x,i) ) Add (x); 
        return size;
    }
	int AddUnique(const Type & x, int(*cmp)(const Type &, const Type &)) {
		int i;
		if (!SearchFirst(x, i, cmp)) Add(x);
		return size;
	}


    // вставка элемента по индексу, возвращает размер
    int Insert ( const Type & x, int i ) {
        if ( size >= maxsize ) Remaxsize ( size + size/2 + 1 );
        for (int j=size; j>i; j--) v[j] = v[j-1];
        v[i] = x;
        return ++size;
    }

    // удаление элемента по индексу, возвращает размер
    int Remove ( int i ) {
        for (int j=i; j<size-1; j++) v[j] = v[j+1];
        Resize( size-1, true );
        return size;
    }
    
    int RemoveValueAt ( int i ) {
        if ( size > 0 ) v[i] = v[ size - 1 ];
        Resize( size-1, true );
        return size;
    }

	// просто поиск (проверка наличия)
	bool Search(const Type &x) {
		for (int i = 0; i<size; i++) if (x == v[i]) { return true; }
		return false;
	}

    // последовательный поиск первого
	bool SearchFirst ( const Type &x, int & ind ) {
        ind = -1;
        for (int i=0; i<size; i++) if ( x==v[i] ) { ind = i; return true; }
        return false;
    }
    bool SearchFirst ( const Type &x, int & ind, int (*cmp)(const Type &, const Type &) ) {
        ind = -1;
        for (int i=0; i<size; i++) if ( cmp(x,v[i])==0 ) { ind = i; return true; }
        return false;
    }

    // последовательный поиск последнего
    bool SearchLast ( const Type &x, int & ind ) {
        ind = -1;
        for (int i=size-1; i>=0; i--) if ( x==v[i] ) { ind = i; return true; }
        return false;
    }
    bool SearchLast ( const Type &x, int & ind, int (*cmp)(const Type &, const Type &) ) {
        ind = -1;
        for (int i=size-1; i>=0; i--) if ( cmp(x,v[i])==0 ) { ind = i; return true; }
        return false;
    }

    // бинарный поиск элемента (в предположении упорядоченности)
    // возвращает bool найден/не найден
    // ind  - позиция, где дожен находиться элемент
    bool BinSearch ( const Type &x, int & ind ) {
        if ( size == 0 ) { ind = 0; return false; }
        if ( x == v[0] )     { ind = 0; return true; }
        if ( x < v[0] )      { ind = 0; return false; }
        if ( x > v[size-1] ) { ind =size; return false; }
        // invariant: v[l] < x <= v[r]
        int l=0, r=size-1, m; 
        while ( r - l > 1 ) {
            m = (l + r)/2;
            if ( x <= v[m] ) r = m;
            else l = m; 
        }
        ind = r;
        return x == v[ind];  
    }
    bool BinSearch ( Type &x, int & ind, int (*cmp)( Type &, Type &) ) {
        if ( size == 0 ) { ind = 0; return false; }
        if ( cmp(x,v[0])==0 )     { ind = 0; return true; }
        if ( cmp(x,v[0])< 0  )      { ind = 0; return false; }
        if ( cmp(x,v[size-1])>0 ) { ind =size; return false; }
        // invariant: v[l] < x <= v[r]
        int l=0, r=size-1, m; 
        while ( r - l > 1 ) {
           m = (l + r)/2;
            if ( cmp(x,v[m])<=0 ) r = m;
            else l = m; 
        }
        ind = r;
        return cmp(x,v[ind])==0;  
    }

    bool BinSearch ( Type &x, int & ind, int (*cmp)( const void*, const void*) ) {
        if ( size == 0 ) { ind = 0; return false; }
        int find = cmp( &x, v );
        if ( find == 0 ) { ind = 0; return true; }
        if ( find < 0  ) { ind = 0; return false; }
        if ( cmp( &x, v + size-1 ) > 0 ) { ind = size; return false; }
        // invariant: v[l] < x <= v[r]
        int l=0, r=size-1, m; 
        while ( r - l > 1 ) {
           m = (l + r)/2;
            if ( cmp( &x, v+m )<=0 ) r = m;
            else l = m; 
        }
        ind = r;
        return cmp( &x, v+ind ) == 0;  
    }



    // сортировка по возрастанию 
    void Sort () {
        Type c; 
        for (int i=1; i<size; i++ ) {
            for (int j=i; j>0 && v[j]<v[j-1]; j-- ) {
                c = v[j-1]; v[j-1] = v[j]; v[j] = c;
            }
        }   
    }

    // сортировка по возрастанию по данному критерию 
    void Sort ( int (*cmp)(const Type &, const Type &) ) {
        Type c; 
        for (int i=1; i<size; i++ ) {
            for (int j=i; j>0 && cmp(v[j],v[j-1])<0; j-- ) {
                c = v[j-1]; v[j-1] = v[j]; v[j] = c;
            }
        }   
    }

    void QSort ( int (*cmp)(const void*, const void*) ) {
        qsort ( v, size, sizeof(Type), cmp );
    }


    // деструктор по элементам массива (если хранятся указатели)
    void DestroyPtrContent () {
        for (int i=1; i<size; i++ ) if (v[i]) {  /*mlog.Del(v[i]);*/ delete v[i]; }
        size = 0;
    }

    // деструктор
    ~WArray() { if (v) {  /*mlog.DelA(v);*/ delete [] v; } }

    // размеры областей
    int Size() const { return size; } 
    int MaxSize() const { return maxsize; }


	WArray<Type> & operator= (const WArray<Type> &a) {
		Resize(a.Size());
		for (int i = 0; i < Size(); i++) v[i] = a.v[i];
		return *this;
	}
	

    

};



template <class T, class N> 
T * CopyNewArray ( N &n, WArray<T> &a )
{
    n = a.Size();
    T * b = new T [n];
    for (unsigned int i=0; i<n; i++ ) b[i] = a[i];
    return b;
}


//---------------------------------------------------------------------------------------
//   простое множество для собственных нужд (на базе массива)
//---------------------------------------------------------------------------------------

template <class Type>
class WSimpleSet
{
protected:
    WArray<Type> elems; 
    int itPos;

public:

    WSimpleSet () : elems(0,8) { itPos = 0; } 

    bool Contains ( const Type &x ) {
        int ind;
        return elems.SearchFirst( x, ind );
    }

    void Add ( const Type &x ) {
        if ( !Contains(x) ) elems.Add(x);
    }
    
    void Remove ( const Type &x ) {
        int ind;
        int size = elems.Size();
        if ( elems.SearchFirst( x, ind ) ) {
            elems[ind] = elems[size-1];
            elems.Remove(size-1);
        }
    }

    void Clear () {
        elems.Resize( 0 );
        itPos = 0;
    }

    int Size () const { return elems.Size(); }

	const Type* Elems() const { return elems.Data();  }

    void InitIter () {
        itPos = 0;
    }
  
    bool HasNext () const {
        return  itPos < elems.Size();
    }

    bool GetNext ( Type &x ) {
        if ( itPos < elems.Size() ) { 
            x = elems[itPos++]; 
            return true;
        } else {
            return false;
        }
    }

	const Type & operator[](int i) const { return elems[i]; }



    const WSimpleSet & operator+= ( const WSimpleSet<Type> & b ) {
        Type x;
        for ( b.InitIter(); b.GetNext(x); ) {
            Add(x);
        }
        return *this;
    }

   
    

};

class WIntSet : public WSimpleSet<int> {
public:
	WIntSet() : WSimpleSet() {}
	WIntSet (const int *els, int n = -1) { 
		if (n < 0) {
			for (n=0; els[n]>=0; n++);
		}
		elems.Resize(n); 
		for (int i=0; i<n; i++) elems[i] = els[i]; 
	}

	void Print(FILE *f, const char * text = 0) {
		if (!f) return;
		if (text) fprintf(f, "%s", text);
		for (int i=0; i<elems.Size(); i++) { 
			fprintf(f," %3d",elems[i]); 
		}
		fputc('\n', f);
	}

};




//---------------------------------------------------------------------------------------
// Элемент односвязного списка
//---------------------------------------------------------------------------------------

template <class Type>
class WSListItem
{
public:
    Type v;
    WSListItem<Type> *next;
    WSListItem() { next = 0; }
    WSListItem(const Type &x) { next = 0; v = x; }
};

//---------------------------------------------------------------------------------------
//   односвязный список без текущей позиции
//---------------------------------------------------------------------------------------

template <class Type>
class WSList
{
protected:
    WSListItem<Type> *start;
    int size;

public:

    // конструкторы
    WSList () { start = 0; size = 0; }        

    // деструктор
    ~WSList () { Clear(); }
    
    void Clear () {
        WSListItem<Type> *p;
        while ( start ) { 
            p = start->next; 
            delete start; 
            start = p; 
        }
        size = 0;
    } 

    WSListItem<Type> * GetStart() { return start; }
    const WSListItem<Type> * GetStart() const { return start; }

    int Size() { return size; }

    // always adds
    void Add ( const Type &x ) {
        WSListItem<Type> *p = new WSListItem<Type>;
        p->v = x;
        p->next = start;
        start = p;
        size++;
    }

    // returns true if the the deletion actually occurs, otherwise returns false
    bool Del ( const Type &x ) {
        WSListItem<Type> *p = start;
        WSListItem<Type> *q = 0;
        while ( p ) {
            if ( p->v == x ) {
                if ( q == 0 ) start = p->next;                        
                else          q->next = p->next;
                delete p;
                size--;
                return true;
            }
            q = p;
            p = p->next; 
        }
        return false;
    }

    bool Del ( Type &x, int (*Cmp)(const Type &, const Type &) ) {
        WSListItem<Type> *p = start;
        WSListItem<Type> *q = 0;
        while ( p ) {
            if ( Cmp(p->v,x) == 0 ) {
                if ( q == 0 ) start = p->next;                        
                else          q->next = p->next;
                delete p;
                size--;
                return true;
            }
            q = p;
            p = p->next;
        }
        return false;
    }

    Type * Search ( const Type &x ) {
        WSListItem<Type> *p = start;
        while ( p ) {
            if ( p->v == x ) return &(p->v);
            p = p ->next;
        }
        return 0;
    }

    Type * Search ( const Type &x, int (*Cmp)(const Type &, const Type &) ) {
        WSListItem<Type> *p = start;
        while ( p ) {
            if ( Cmp(p->v,x) == 0 ) return &(p->v);
            p = p ->next;
        }
        return 0;
    }

    void FillFrom ( WSList &li ) {
        WSListItem<Type> *p;
        while ( start ) { p = start->next; delete start; start = p; } 
        for ( p=li.start; p; p=p->next ) {
            Add( p->v );
        }
    }


};

//---------------------------------------------------------------------------------------
//   простенький двусвязный список для собственных нужд
//---------------------------------------------------------------------------------------

//
// Элемент двусвязного списка
//
template <class Type>
class WDListItem
{
public:
    Type v;
    WDListItem<Type> *next, *prev;
    WDListItem() { next = prev = 0; }
    WDListItem(const Type &x) { next = prev = 0; v = x; }
};

//
// Кольцевой двусвязный список
//
template <class Type>
class WCList 
{
protected:
    WDListItem<Type> *cur;  // указатель текущей позиции
    int size;              // количество элементов в списке

public:
    WCList() { cur = 0; size = 0; }
   ~WCList() { Clear(); }


    // установить текущую позицию (без проверки!!!)
    void SetCurItem (WDListItem<Type> *p) { cur = p; }

    // очистить список
    void Clear() { while (size) DelToNext(); } 
    
    // заполнить список из массива
    void Fill(int n, Type *val) {
        for (int i=0; i<n; i++) AddNext(val[i]);
    }

    // заполнить массив из списка
    void Export(Type *val) {
        for (int i=0; i<size; i++, GoToNext()) val[i] = GetCurRef();
    }

    // добавить после текущей позиции, новый элемент становится текущим
    void AddNext(const Type &x) {
        WDListItem<Type> *p = new WDListItem<Type>(x);
        if ( size ) {
            p->next = cur->next;
            p->prev = cur;
            p->next->prev = p;
            p->prev->next = p;
        } else {
            p->next = p->prev = p;   
        }
        cur = p;
        size++;
    }
    // добавить перед текущей позицией, новый элемент становится текущим
    void AddPrev(const Type &x) {
        WDListItem<Type> *p = new WDListItem<Type>(x);
        if ( size ) {
            p->next = cur;
            p->prev = cur->prev;
            p->next->prev = p;
            p->prev->next = p;
        } else {
            p->next = p->prev = p;   
        }
        cur = p;
        size++;
    }
    // удалить следующий элемент, текущая позиция остается на месте
    void DelNext() {
        WDListItem<Type> *p;
        if ( size ) {
            p = cur->next;
            cur->next = p->next;
            p->next->prev = cur;
            size--;
            delete p;
            if ( size == 0 ) cur = 0;
        } 
    }

    // удалить текущий элемент, текущая позиция сдвигается на следующий
    void DelToNext() {
        WDListItem<Type> *p;
        if ( size ) {
            p = cur;
            cur->next->prev = cur->prev;
            cur->prev->next = cur->next;
            cur = cur->next;
            size--;
            delete p;
            if ( size == 0 ) cur = 0;
        } 
    }
    // удалить текущий элемент, текущая позиция сдвигается на предыдущий
    void DelToPrev() {
        WDListItem<Type> *p;
        if ( size ) {
            p = cur;
            cur->next->prev = cur->prev;
            cur->prev->next = cur->next;
            cur = cur->prev;
            size--;
            delete p;
            if ( size == 0 ) cur = 0;
        } 
    }
    // искать заданное значение (установить текущую позицию на него), 
    // при отсутствии текущая позиция не изменяется
    // return = true/false успех/неудача
    bool Search (const Type &x) {
        if ( size==0 ) return false;
        WDListItem<Type> *p = cur;
        do {
            if ( p->v == x ) return true;
            p = p->next;
        } while ( p != cur );
        return false;
    }
    bool Search (const Type &x, int (*cmp)(const Type &, const Type &) ) {
        if ( size==0 ) return false;
        WDListItem<Type> *p = cur;
        do {
            if ( cmp(p->v,x)==0 ) return true;
            p = p->next;
        } while ( p != cur );
        return false;
    }

    // количество элементов в списке
    int Size() { return size; }
    // ссылка не текущее значение
    Type & GetCurRef() { return cur->v; }
    // ссылка не следующее значение
    Type & GetNextRef() { return cur->next->v; }
    // ссылка не предыдущее значение
    Type & GetPrevRef() { return cur->prev->v; }
    // указатель на текущее значение 
    Type * GetCurPtr() { return &(cur->v); }
    // указатель на текущий элемент списка
    WDListItem<Type> * GetCurItem () { return cur; }
    // передвинуть текущую позицию к следующему
    void GoToNext() { cur = cur->next; }
    // передвинуть текущую позицию к предыдущему
    void GoToPrev() { cur = cur->prev; }

    // распечатать содержимое списка внешней функцией
    void Dump ( void (*PrintItem)(const Type &, FILE *), FILE *f=stdout ) {
        fprintf(f,"[%2d]:\n",size);
        for (int i=0; i<size; i++ ) {
            PrintItem(GetCurRef(),f);
            GoToNext();
        }
        fprintf(f,"\n");
    }
    void DumpV ( void (*PrintItem)(const Type &, FILE *), FILE *f=stdout ) {
        fprintf(f,"[%2d]:\n",size);
        for (int i=0; i<size; i++ ) {
            PrintItem(GetCurRef(),f);
            GoToNext();
        }
        fprintf(f,"\n");
    }
    void Dump2 ( void (*PrintItem)(const Type &, FILE *), FILE *f=stdout ) {
        WDListItem<Type> * p = cur;
        fprintf(f,"size %d\n",size);
        for (int i=0; i<size; i++ ) {
            fprintf(f,"%p <- %p -> %p ",p->prev,p,p->next);
            PrintItem(GetCurRef(),f);
            if ( p->next->prev != p ) printf(" error 1 %p -> %p => %p\n",p,p->next,p->next->prev);   
            if ( p->prev->next != p ) printf(" error 2 %p -> %p => %p\n",p,p->prev,p->prev->next);   
            fprintf(f,"\n");
            p = p->next;
        }
        fprintf(f,"\n");
    }
    // склеить с другим списком по совпадающим парам значений
    bool Join( WCList<Type> &list ) {
        // ищем совпадающую пару
        for (int i=0; i<size; i++) {
            for (int j=0; j<list.size; j++) {
                if ( cur->v == list.cur->v && cur->next->v == list.cur->prev->v ) goto s_ok;
                list.GoToNext();
            }
            GoToNext();
        }
        return false;
    s_ok:    
        // ищем первую пару
        while ( cur->v == list.cur->v ) {
            cur = cur->prev;
            list.cur = list.cur->next;
        }
        cur = cur->next;
        list.cur = list.cur->prev;

        // ищем последнюю совпадающую пару
        int cnt = 0;
        WDListItem<Type> *p = cur;
        WDListItem<Type> *q = list.cur;
        while ( p->v == q->v ) {
            p = p->next;
            q = q->prev;
            cnt++;
        }
        p = p->prev;
        q = q->next;

        //printf("(%d) %d %d %d %d\n",cnt,cur->v,list.cur->v,p->v,q->v);

        // объединяем списки
//printf("1  %p <- %p -> %p  < %p\n",cur->prev,cur,cur->next,cur->next->prev);
//printf("2  %p <- %p -> %p  < %p\n",list.cur->prev,list.cur,list.cur->next,list.cur->next->prev);

        WDListItem<Type> *pp = cur->next; 
        
        cur->next = list.cur->next;
        cur->next->prev = cur;
        list.cur->next = pp;
        list.cur->next->prev = list.cur;

//printf("1  %p <- %p -> %p  < %p\n",cur->prev,cur,cur->next,cur->next->prev);
//printf("2  %p <- %p -> %p  < %p\n",list.cur->prev,list.cur,list.cur->next,list.cur->next->prev);


        pp = q->next;
        q->next = p->next;
        q->next->prev = q;
        p->next = pp;
        p->next->prev = p;

        size += list.size - 2*cnt + 2;
        list.size = 2*cnt - 2;

        return true;
    }

    void Test(const char *text) {
        if ( size==0 && cur==0 ) return;
        int cnt=0;
        WDListItem<Type> *p = cur;
        
        for (int i=0; i<size; i++ ) {
            if ( p->next->prev != p ) printf("%s list error 1 %p -> %p => %p\n",text,p,p->next,p->next->prev);   
            if ( p->prev->next != p ) printf("%s list error 2 %p -> %p => %p\n",text,p,p->prev,p->prev->next);   
            p = p->next;       
        }
        if ( p!=cur ) printf("%s list error 3 size %p  %p\n",text,p,cur);   

       
    }


};


//---------------------------------------------------------------------------------------
//   простая двумерная матрица
//---------------------------------------------------------------------------------------

template <class T>
class SimpleMatrix 
{
protected:
    T * val;
    int numrows, numcols;
public:
    SimpleMatrix () { numrows = numcols = 0; val = 0;}
    SimpleMatrix ( int nr, int nc ) { val = 0; SetSize( nr, nc ); } 
   ~SimpleMatrix () { if (val) delete [] val; }     
    void SetSize ( int nr, int nc ) { if (val) delete [] val; val = new T[nr*nc]; numrows = nr, numcols = nc; } 
    T * operator [] (int i) { return val + numcols*i; }

};


template <class T>
class WMatrix 
{
protected: 
    WArray<WArray<T>*> row;
public:
    WMatrix() {}
    WMatrix(int nrows) {
        for (int i=0; i<nrows; i++) AddRow();
    }
    ~WMatrix () { row.DestroyPtrContent(); }

    void SetNumRows (int nrows) {
        int n = NumRows();  
        for (int i=n; i<nrows; i++) AddRow();    
    }

    // возвращает индекс добавленной строки
    int AddRow () { 
        WArray<T> *a = new WArray<T>;
        row.Add(a);
        return row.Size()-1;
    }

    int AddRowElem ( const T &x, int irow ) {
        return row[irow]->Add(x);
    }
    int AddUniqueRowElem ( const T &x, int irow ) {
        return row[irow]->AddUnique(x); 
    }
        
    bool SearchFirstInRow ( const T &x, int irow, int &jelem ) {
        return row[irow]->SearchFirst( x, jelem );
    }
    
    int NumRows() { return row.Size(); }

    WArray<T> & Row( int i) { return *row[i]; }

};



//---------------------------------------------------------------------------------------
//   простое множество-счетчик
//---------------------------------------------------------------------------------------
class CntSet 
{
protected:
    int *set;
    int size;
public: 
    CntSet ( int _size ) { set = new int[_size]; size = _size; Clear(); }
   ~CntSet () { delete [] set; }
    void Clear () { for (int i=0; i<size; i++) set[i] = 0; }
    void Clear (int i) { set[i] = 0; }
    void Add (int i) { if (i<size && i>=0) set[i]++; }
    void Del (int i) { if (i<size && i>=0) set[i]--; }
    const int & operator [] (int i) { return set[i]; }

};


//---------------------------------------------------------------------------------------
//   простое битовое множество
//---------------------------------------------------------------------------------------

#define BITSETBLOCK  (8*sizeof(int))

class BitSetOld
{
protected:
    unsigned int * table;        
    int vmax, vmin;    
    int itInd, itBit;
    int nBlk;
    int numElems;
    WLogger * lll;

    void GoToNext() { 
        //lll->Print(LM_BASE,"ind %3d bit %3d  table %08X ",itInd, itBit, table[itInd] );
        for ( ++itBit; itBit < BITSETBLOCK && (table[itInd] & ( 1<<itBit )) == 0; ++itBit );
        if ( itBit == BITSETBLOCK ) {
            for ( ++itInd;  itInd < nBlk && table[itInd]==0; ++itInd );
            for ( itBit=0; itBit < BITSETBLOCK && (table[itInd] & ( 1<<itBit )) == 0; ++itBit );
        }
        //lll->Print(LM_BASE," -> ind %3d bit %3d table %08X\n",itInd, itBit, table[itInd] );
        
    }
        
public:

    BitSetOld( int maxsize ) {
        nBlk = maxsize/BITSETBLOCK + 1;
        table = new unsigned int[ nBlk ];
        ClearAll();
        vmax = maxsize;
        vmin = 0;
        lll=0;
    }

    ~BitSetOld () { delete [] table; }
    
    void SetLogger( WLogger *l ) { lll=l; }

    int NumElems () { return numElems; }

    void Set ( int k ) {
        if ( Contains(k) ) return;
        WAssert( vmin <= k && k<=vmax, "BitSetOldOld::Set: bad index" );
        unsigned int & v = table [ k/BITSETBLOCK ];
        v |= 1<<( k%BITSETBLOCK );
        ++numElems;
    } 
    
    void Clear ( int k ) {
        if ( !Contains(k) ) return;
        WAssert( vmin <= k && k<=vmax, "BitSetOldOld::Set: bad index" );
        unsigned int & v = table [ k/BITSETBLOCK ];
        v &= ~(1<<( k%BITSETBLOCK ));
        --numElems;
    }

    bool Contains( int k ) {
        if ( k < vmin || k > vmax ) return false;
        unsigned int & v = table [ k/BITSETBLOCK ];
        return  ( v & (1<<( k%BITSETBLOCK ))) != 0 ;
    }

    void ClearAll () {
        for (int i=0; i<nBlk; i++) table[i] = 0;
        numElems = 0;
    }

    void InitIter() {
        itInd = 0; itBit = -1; 
        GoToNext();
    }

    bool GetNext ( int &res ) {
        if ( itInd >= nBlk ) return false;
        res = itInd*BITSETBLOCK + itBit; 
        //lll->Print(LM_BASE,"GetNExt %d\n",res);
        GoToNext();
        return true;
    }

    void Print() {
        if (lll==0) return;
        for (int i=0; i<nBlk; i++) {
            lll->Print(LM_BASE,"%08X ",table[i]);
        }
        lll->Print(LM_BASE,"\n");
    }
    

};




#endif // _ARRAYLIST_H_
