#include <iostream>
#include <stdexcept>
#include "Graph.h"

using namespace std ;


int main ( int narg, char* argv[] )
try
{
	// gebe Usage aus
	if ( narg < 2 )
	{
		cout << "usage: " << argv[ 0 ] << "  dateiname [ungerichtet]"	 << endl ;
		cout << "       dateiname  : Name einer Graph-Datei"			 << endl ;
		cout << "       ungerichtet: irgendein Zeichen"					 << endl ;
		cout << "Liest den Graphen aus der Datei aus und zeigt ihn an."  << endl ;
		cout << "Interpretiert ihn ungerichtet, falls ein zusaetzliches" << endl ;
		cout << "Zeichen gegeben ist."									 << endl ;

		return 0 ;
	}

/***  lese Graph ein  ***/

	string const& dateiName = argv[ 1 ] ;
	bool          richtung  = ( narg > 2  ?  Graph::UNGERICHTET  :  Graph::GERICHTET ) ;
	Graph		  graph( dateiName, richtung ) ;

	cout << graph << endl ;

	return 0 ;

}  // main()

catch ( char const text[] )
{
	cerr << text << endl ;
}
catch ( exception const& exc )
{
	cerr << exc.what() << endl ;
}
