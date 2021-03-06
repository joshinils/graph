#pragma once

#include "Aether.h"
#include "Index.h"
#include <algorithm>
#include <iostream>
#include <string>

/**
 * Klasse für Graphknoten
 * Kann nach Bedarf erweitert oder abgeleitet werden
 */


class Knoten
{
public:
    /***  öffentliche Attribute  ***/

    /// Name des Knotens
    std::string name;

    /// Koordinaten des Knotens
    double xKoo, yKoo;

    double nodeScale = 20;

    /***  Konstruktoren  ***/

    /// Standard- und Initialisierungskonstruktor
    Knoten(std::string const& argName = "", double argX = 0, double argY = 0)
        : name(argName)
        , xKoo(argX)
        , yKoo(argY)
    {
        /// for validity upon saving, replace all spaces (remove them)
        std::replace(name.begin(), name.end(), ' ', '_');
    }

    /***  Vergleichsoperatoren  ***/

    /// lexikografischer Vergleich
    bool operator<(Knoten const& knoten) const { return name < knoten.name; }

    /// lexikografischer Vergleich
    bool operator==(Knoten const& knoten) const { return name == knoten.name; }

    /***  Ausgabe auf ostream  ***/

    friend std::ostream& operator<<(std::ostream& ostr, Knoten const& knoten)
    {
        return ostr << knoten.name << "(" << knoten.xKoo << "," << knoten.yKoo << ")";
    }

    void drawCirc(Aether& aether, double drawScale) const;

    void drawName(Aether& aether, double drawScale) const;

    void setName(std::string const& str)
    {
        return;
        name = str;
    }

}; /// class Knoten
