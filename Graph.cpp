#include <iomanip>			// für setw()
#include <fstream>
#include <sstream>			// für istringstream
#include "sortieren.h"		// für sortArray()
#include "Graph.h"

#define  TALK  0		// Debugging-Output?


// Initialisierungskonstruktor von Datei
Graph::Graph ( string const& dateiName, bool gerichtet )
{
	ifstream fin( dateiName.c_str() ) ;

	if ( ! fin )
		throw "Graph::Graph(): Datei kann nicht geoeffnet werden!" ;

#if TALK
cout << "Lese Input-Graph " << dateiName << endl ;
#endif
/***  lese Graphparameter aus  ***/

	size_t nKnoten, nKanten ;
	string dummy ;

	fin >> nKnoten ;
	getline( fin, dummy ) ;		// ignoriere Rest der Zeile
	fin >> nKanten ;
	getline( fin, dummy ) ;		// ignoriere Rest der Zeile

	// setze Objektattribute
	_knoten  .resize( nKnoten ) ;
	_kanten  .resize( nKanten ) ;
	_nachbarn.resize( nKnoten ) ;
	if ( gerichtet )
		_rnachbarn.resize( nKnoten ) ;
	_gerichtet = gerichtet ;

#if TALK
cout << nKnoten << " Knoten" << endl ;
cout << nKanten << " Kanten" << endl ;
cout << endl ;
#endif
/***  lese Knoten aus  ***/

	for ( size_t i = 0 ;  i < _knoten.size() ;  ++i )
	{
		Knoten& knoten = _knoten[ i ] ;

		fin >> knoten.name >> knoten.xKoo >> knoten.yKoo ;
		getline( fin, dummy ) ;		// ignoriere Rest der Zeile
	}

#if TALK
cout << "Knoten gelesen" << endl ;
#endif

	// Sortiere Knotennamen für binäre Suche in index( string const& ).
	// Eine Hash-Tabelle wäre effizienter für die Knotenzuordnung beim
	// Einlesen der Kanten, dann gäbe es aber die o.a. Methode index()
	// nicht für das weitere Leben des Graph-Objekts. Der Zeit-Tradeoff
	// "bin. Suche vs. Hashing" ist auch bei großen Input-Graphen kaum
	// zu merken.
	sortArray( _knoten ) ;

/***  lese Kanten aus  ***/

	string zeile ;

	// ignoriere Leerzeilen zwischen Knoten- und Kantenblock
	do
		getline( fin, zeile ) ;
	while ( zeile.empty() ) ;

	// lese alle Kanten aus
	for ( size_t i = 0 ;  i < _kanten.size() ;  ++i )
	{
		Kante& kante = _kanten[ i ] ;

		// lese Kantenzeile indirekt, damit gewicht ggf. Defaultwert
		// bekommt, wenn keine Gewichte gegeben sind
		string fuss, kopf ;
		istringstream( zeile ) >> kante.name >> fuss >> kopf >> kante.gewicht ;

		// suche Knotenindizes zu den Namen
		kante.iFuss = index( fuss ) ;
		kante.iKopf = index( kopf ) ;

		// setze Nachbarnmengen
		_nachbarn[ kante.iFuss ].push_back( IndexPaar( kante.iKopf, i ) ) ;
		if ( _gerichtet )
			_rnachbarn[ kante.iKopf ].push_back( IndexPaar( kante.iFuss, i ) ) ;
		else
			_nachbarn [ kante.iKopf ].push_back( IndexPaar( kante.iFuss, i ) ) ;

		// lese nächste Zeile
		getline( fin, zeile ) ;

	}  // for ( i )

#if TALK
cout << "Kanten gelesen" << endl ;
cout << *this << endl ;
#endif
/***  räume auf  ***/

	fin.close() ;	

}  // Graph::Graph()


/***  get-Methoden  ***/

// gib Index der Kante (u,v), falls sie existiert
// KEIN_INDEX, falls nicht
size_t Graph::index ( size_t u, size_t v ) const
{
	if (     u < anzKnoten()
			&&  v < anzKnoten() )
		FUER_ALLE_NACHBARN( w, *this, u )
			if ( w->iKnoten == v )
				return w->iKante ;

	return KEIN_INDEX ;
}


// gib Index des Knotens zum Namen aus;
// KEIN_INDEX, falls kein solcher Knoten exitiert
size_t Graph::index ( string const& name ) const
{
	// name nicht enthalten
	if (	 name < _knoten[ 0 ].name
		 ||  name > _knoten.back().name )
		return KEIN_INDEX ;

/*** binäre Suche  ***/

	size_t li = 0 ;
	size_t re = _knoten.size() - 1 ;

	while ( li < re )
	{
		size_t mitte = ( li + re ) >> 1 ;

		if ( name <= _knoten[ mitte ].name )
			re = mitte ;
		else
			li = mitte + 1 ;
	}

	if ( _knoten[ li ].name == name )
		return li ;
	else
		return KEIN_INDEX ;

}  // Graph::index()


// füge neue Kante (u,v) bzw. {u,v} mit name zum Objekt
// hinzu, falls sie nicht existiert; gib ihren Index
// zurück oder KEIN_INDEX, falls existent
size_t Graph::setzeKante ( string const& name, size_t u, size_t v )
{
	// existiert Knotenpaar schon?
	if ( KEIN_INDEX != index( u, v ) )
		return KEIN_INDEX ;

	// nächster freier Kantenindex
	size_t e = _kanten.size() ;

	// füge Kante ein
	_kanten.push_back( Kante( name, u, v ) ) ;
	_nachbarn[ u ].push_back( IndexPaar( v, e ) ) ;
	if ( _gerichtet )
		_rnachbarn[ v ].push_back( IndexPaar( u, e ) ) ;
	else
		_nachbarn [ v ].push_back( IndexPaar( u, e ) ) ;

	// gib Kantenindex zurück
	return e ;

}  // setzeKante()



/***  Ausgabe auf ostream  ***/

ostream& operator << ( ostream& ostr, Graph const& graph )
{
	ostr << "Graph:  " << graph.anzKnoten() << " Knoten, "
		 << graph.anzKanten() << " Kanten" << endl ;
	ostr << endl ;

	// berechne max. Breite der Knotenname
	size_t breite = 0 ;
	FUER_ALLE_KNOTEN( u, graph )
		if ( breite < graph.knoten( u ).name.size() )
			breite = graph.knoten( u ).name.size() ;

	// gebe die Knoten mit ihren Nachbarnmengen aus
	FUER_ALLE_KNOTEN( u, graph )
	{
		ostr << setw( breite ) << graph.knoten( u ) << " :" ;
	
		FUER_ALLE_NACHBARN( v, graph, u )
		{
			ostr << " -" << graph.kante ( v->iKante  ) << "->"
						 << graph.knoten( v->iKnoten ).name ;
		}
		ostr << endl ;

		if ( graph.gerichtet() )
		{
			ostr << setw( breite ) << graph.knoten( u ) << " :" ;
	
			FUER_ALLE_RNACHBARN( v, graph, u )
			{
				ostr << " <-" << graph.kante ( v->iKante  ) << "-"
							  << graph.knoten( v->iKnoten ).name ;
			}
			ostr << endl ;
		}
	}  // FUER_ALLE_KNOTEN( u )

	return ostr ;

}  // operator <<

