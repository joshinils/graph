#ifndef SORTIEREN_H
#define SORTIEREN_H

#include <cstdlib>	// für qsort()
#include <vector>

using namespace std ;


// aufsteigende Vergleichsfunktion für zwei Arrayeinträge
template < typename T >
int incComp ( T const* obj1, T const* obj2 )
{
	if ( *obj1 == *obj2 )
		return 0 ;
	else
		return ( *obj1 < *obj2  ?  -1  :  1 ) ;
}


// absteigende Vergleichsfunktion für zwei Arrayeinträge
template < typename T >
int decComp ( T const* obj1, T const* obj2 )
{
	if ( *obj1 == *obj2 )
		return 0 ;
	else
		return ( *obj1 < *obj2  ?  1  :  -1 ) ;
}


// sortiere array aufsteigend gemäß T::operator <
template < typename T >
void sortArray ( vector<T>& array, int (*vergleich)( T const*, T const* ) = incComp<T> )
{
	qsort( (void*)array.data(), array.size(), sizeof( T ),
		   (int (*) (void const*, void const*))vergleich ) ;
}


#endif  // SORTIEREN_H
