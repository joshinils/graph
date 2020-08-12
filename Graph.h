#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <string>
#include "Knoten.h"
#include "Kante.h"

using namespace std ;


// ungültiger Sonderwert für Indizes wird in Knoten.h definiert
// size_t const KEIN_INDEX = UINT_MAX ;


// Durchlaufschleife über alle Knotenindizes
#define  FUER_ALLE_KNOTEN( u, graph )													\
		 for ( size_t u = 0 ;  u < (graph).anzKnoten() ;  ++u )

// Durchlaufschleife über alle Kantenindizes
#define  FUER_ALLE_KANTEN( e, graph )													\
		 for ( size_t e = 0 ;  e < (graph).anzKanten() ;  ++e )

// Durchlaufschleife über die Vorwärts-Nachbarn u des Knotens v.
// Der aktuelle Knoten wird mit *u oder u->... angesprochen.
#define  FUER_ALLE_NACHBARN( u, graph, v )												\
		 for ( nachbarnMenge::const_iterator u  = (graph).nachbarn( v ).begin() ;		\
											 u != (graph).nachbarn( v ).end() ;  ++u )

// Durchlaufschleife über die Rückwärts-Nachbarn u des Knotens v
// Der aktuelle Knoten wird mit *u oder u->... angesprochen.
#define  FUER_ALLE_RNACHBARN( u, graph, v )												\
		 for ( nachbarnMenge::const_iterator u  = (graph).rnachbarn( v ).begin() ;		\
											 u != (graph).rnachbarn( v ).end() ;  ++u )


/***  Hilfsklasse zur Verbindung von Knoten- und Kantenindizes  ***/

class IndexPaar
{
public:
	// Knotenindex
	size_t iKnoten ;

	// Kantenindex
	size_t iKante ;

	// Standard- und Initialisierungskonstruktor
	IndexPaar ( size_t argKno = KEIN_INDEX,
			    size_t argKan = KEIN_INDEX )
		: iKnoten( argKno ),
		  iKante ( argKan )
	{ }

	// Vergleich miteinander
	bool operator == ( IndexPaar const& paar ) const
	{
		return	   iKnoten == paar.iKnoten
			   &&  iKante  == paar.iKante ;
	}

	// Vergleich miteinander
	bool operator != ( IndexPaar const& paar ) const
	{
		return ! ( *this == paar ) ;
	}

} ;  // class IndexPaar


// Definition des Typs der Nachbarnmengen.
// Im Prinzip beliebiger Container, seine Verwendung
// im Text muss bei Wechsel ggf. angepasst werden
typedef  vector<IndexPaar>  nachbarnMenge ;



/**
 * Klasse für gerichtete oder ungerichtete Graphen
 */

class Graph
{
/***  private Attribute  ***/

	// ist Objekt gerichtet?
	bool					_gerichtet ;

	// Array der Knoten
	vector< Knoten >        _knoten ;

	// Array der Kanten
	vector< Kante >         _kanten ;

	// Vorwärts-Nachbarnmengen aller Knoten
	vector< nachbarnMenge > _nachbarn ;

	// Rückwärts-Nachbarnmengen aller Knoten
	vector< nachbarnMenge > _rnachbarn ;

public:
/***  öffentliche Konstanten  ***/

	// Gerichtetheit des Objekts
	static bool const GERICHTET   = true ;
	static bool const UNGERICHTET = false ;

/***  Konstruktoren  ***/

	// Initialisierungskonstruktor mit Anzahl Knoten
	Graph ( size_t anzKnoten, bool gerichtet )
		: _knoten   ( anzKnoten ),
		  _nachbarn ( anzKnoten ),
		  _rnachbarn( gerichtet ? anzKnoten : 0 ),
		  _gerichtet( gerichtet )
	{ }


	// Initialisierungskonstruktor von Datei
	Graph ( string const& dateiName, bool gerichtet ) ;

/***  get-Methoden  ***/

	// ist das Objekt gerichtet?
	bool gerichtet () const
	{
		return _gerichtet ;
	}


	// gib Anzahl Knoten aus
	size_t anzKnoten () const
	{
		return _knoten.size() ;
	}


	// gib Anzahl Kanten aus
	size_t anzKanten () const
	{
		return _kanten.size() ;
	}


	// gib Objekt des Knotens mit Index v aus
	// wirf Exception, falls v zu groß
	Knoten const& knoten ( size_t v ) const
	{
		if ( v >= anzKnoten() )
			throw "Graph::knoten(): Index zu gross!" ;
		return _knoten[ v ] ;
	}


	// gib Objekt der Kante mit Index e aus
	// wirf Exception, falls e zu groß
	Kante const& kante ( size_t e ) const
	{
		if ( e >= anzKanten() )
			throw "Graph::kante(): Index zu gross!" ;
		return _kanten[ e ] ;
	}

	
	// gib Vorwärts-Nachbarnmenge des Knotens v aus
	// wirf Exception, falls v ungültig
	nachbarnMenge const& nachbarn ( size_t v ) const
	{
		if ( v >= anzKnoten() )
			throw "Graph::nachbarn(): Index zu gross!" ;
		return _nachbarn[ v ] ;
	}


	// gib Rückwärts-Nachbarnmenge des Knotens v aus
	// wirf Exception, falls v ungültig
	nachbarnMenge const& rnachbarn ( size_t v ) const
	{
		if ( v >= anzKnoten() )
			throw "Graph::rnachbarn(): Index zu gross!" ;

		// Objekt ungerichtet -> rückwärts = vorwärts
		return _gerichtet  ?  _rnachbarn[ v ]  :  _nachbarn[ v ] ;
	}


	// gib Vorwärts-Grad des Knotens v aus
	// wirf Exception, falls v ungültig
	size_t grad ( size_t v ) const
	{
		return nachbarn( v ).size() ;
	}


	// gib Rückwärts-Grad des Knotens v aus
	// wirf Exception, falls v ungültig
	size_t rgrad ( size_t v ) const
	{
		return rnachbarn( v ).size() ;
	}


	// gib Index der Kante (u,v), falls sie existiert
	// KEIN_INDEX, falls nicht
	size_t index ( size_t u, size_t v ) const ;


	// gib Index des Knotens zum Namen aus;
	// KEIN_INDEX, falls kein solcher Knoten existiert
	size_t index ( string const& name ) const ;

/***  set-Methoden  ***/

	// füge neue Kante (u,v) bzw. {u,v} mit name zum Objekt
	// hinzu, falls sie nicht existiert; gib ihren Index
	// zurück oder KEIN_INDEX, falls existent
	size_t setzeKante ( string const& name, size_t u, size_t v ) ;

} ;  // class Graph


/***  Ausgabe auf ostream  ***/

ostream& operator << ( ostream& ostr, Graph const& graph ) ;


#endif  // GRAPH_H
