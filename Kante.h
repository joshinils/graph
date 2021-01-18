#pragma once

#include "Aether.h"
#include "Graph.h"
#include "Index.h"
#include "Knoten.h"
#include <iostream>
#include <string>

/**
 * Klasse für Graphkanten
 * Kann nach Bedarf erweitert oder abgeleitet werden
 */

class Kante
{
public:
    /***  öffentliche Attribute  ***/

    /// Name der Kante
    std::string name;

    /// Indizes der Endknoten
    knotenIndex iFuss, iKopf;

    /// Gewicht der Kante
    double gewicht;

    /***  Konstruktoren  ***/

    /// Standard- und Initialisierungskonstruktor
    Kante(std::string const& argName = "",
          knotenIndex argFuss        = knotenIndex::KEIN_INDEX,
          knotenIndex argKopf        = knotenIndex::KEIN_INDEX,
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

    void draw(Aether& aether, const Graph* const graph) const;

}; /// class Kante
