#pragma once

#include "Knoten.h"
#include <climits>
#include <iostream>
#include <string>

// ungültiger Sonderwert für Indizes
knotenIndex const KEIN_INDEX = UINT_MAX;

/**
 * Klasse für Graphkanten
 * Kann nach Bedarf erweitert oder abgeleitet werden
 */

class Kante
{
public:
    /***  öffentliche Attribute  ***/

    // Name der Kante
    std::string name;

    // Indizes der Endknoten
    knotenIndex iFuss, iKopf;

    // Gewicht der Kante
    double gewicht;

    /***  Konstruktoren  ***/

    // Standard- und Initialisierungskonstruktor
    Kante(std::string const& argName = "",
          knotenIndex argFuss        = KEIN_INDEX,
          knotenIndex argKopf        = KEIN_INDEX,
          double argGew              = 1)
        : name(argName)
        , iFuss(argFuss)
        , iKopf(argKopf)
        , gewicht(argGew)
    { }

    /***  Ausgabe auf ostream  ***/
    friend std::ostream& operator<<(std::ostream& ostr, Kante const& kante)
    {
        return ostr << kante.name << "[" << kante.gewicht << "]";
    }

}; // class Kante
