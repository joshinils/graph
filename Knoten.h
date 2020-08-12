#ifndef KNOTEN_H
#define KNOTEN_H

#include <iostream>

using namespace std ;


/**
 * Klasse für Graphknoten
 * Kann nach Bedarf erweitert oder abgeleitet werden
 */

class Knoten
{
public:
/***  öffentliche Attribute  ***/

	// Name des Knotens
	string name ;

	// Koordinaten des Knotens
	double xKoo, yKoo ;

/***  Konstruktoren  ***/

	// Standard- und Initialisierungskonstruktor
	Knoten ( string const& argName = "", double argX = 0, double argY = 0 )
		: name( argName ),
		  xKoo( argX    ),
		  yKoo( argY    )
	{ }

/***  Vergleichsoperatoren  ***/

	// lexikografischer Vergleich
	bool operator < ( Knoten const& knoten ) const
	{
		return name < knoten.name ;
	}

	// lexikografischer Vergleich
	bool operator == ( Knoten const& knoten ) const
	{
		return name == knoten.name ;
	}

/***  Ausgabe auf ostream  ***/

	friend ostream& operator << ( ostream& ostr, Knoten const& knoten )
	{
		return ostr << knoten.name << "(" << knoten.xKoo << "," << knoten.yKoo << ")" ;
	}

} ;  // class Knoten


#endif   // KNOTEN_H
