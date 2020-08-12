#pragma once

#include <iostream>
#include <string>

/**
 * Klasse für Graphknoten
 * Kann nach Bedarf erweitert oder abgeleitet werden
 */

// size_t but named differently
struct knotenIndex
{
    size_t index;
    knotenIndex(size_t i = 0)
        : index(i)
    { }
    operator size_t() const { return index; }
    knotenIndex& operator++()
    {
        index++;
        return *this;
    }
    knotenIndex operator++(int)
    {
        knotenIndex tmp(*this);
        operator++();
        return tmp;
    }
    bool operator==(knotenIndex const& other) const { return this->index == other.index; }
    bool operator!=(knotenIndex const& other) const { return this->index != other.index; }
};

class Knoten
{
public:
    /***  öffentliche Attribute  ***/

    // Name des Knotens
    std::string name;

    // Koordinaten des Knotens
    double xKoo, yKoo;

    /***  Konstruktoren  ***/

    // Standard- und Initialisierungskonstruktor
    Knoten(std::string const& argName = "", double argX = 0, double argY = 0)
        : name(argName)
        , xKoo(argX)
        , yKoo(argY)
    { }

    /***  Vergleichsoperatoren  ***/

    // lexikografischer Vergleich
    bool operator<(Knoten const& knoten) const { return name < knoten.name; }

    // lexikografischer Vergleich
    bool operator==(Knoten const& knoten) const { return name == knoten.name; }

    /***  Ausgabe auf ostream  ***/

    friend std::ostream& operator<<(std::ostream& ostr, Knoten const& knoten)
    {
        return ostr << knoten.name << "(" << knoten.xKoo << "," << knoten.yKoo << ")";
    }

}; // class Knoten
