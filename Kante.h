#ifndef KANTE_H
#define KANTE_H

#include <climits>
#include <iostream>
#include <string>


// ungültiger Sonderwert für Indizes
size_t const KEIN_INDEX = UINT_MAX;


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
    size_t iFuss, iKopf;

    // Gewicht der Kante
    double gewicht;

    /***  Konstruktoren  ***/

    // Standard- und Initialisierungskonstruktor
    Kante(std::string const& argName = "", size_t argFuss = KEIN_INDEX, size_t argKopf = KEIN_INDEX, double argGew = 1)
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


#endif // KANTE_H
