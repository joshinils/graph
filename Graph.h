#pragma once

class Aether;
#include "Aether.h"
#include "Kante.h"
#include "Knoten.h"
#include <iostream>
#include <limits>
#include <queue> /// dijkstra
#include <string>
#include <vector>


/// Durchlaufschleife über alle Knotenindizes
#define FUER_ALLE_KNOTEN(u, graph) for(knotenIndex u = 0; u < (graph).anzKnoten(); ++u)

/// Durchlaufschleife über alle Kantenindizes
#define FUER_ALLE_KANTEN(e, graph) for(kantenIndex e = 0; e < (graph).anzKanten(); ++e)

/// Durchlaufschleife über die Vorwärts-Nachbarn u des Knotens v.
/// Der aktuelle Knoten wird mit *u oder u->... angesprochen.
#define FUER_ALLE_NACHBARN(u, graph, v) \
    for(nachbarnMenge::const_iterator u = (graph).nachbarn(v).begin(); u != (graph).nachbarn(v).end(); ++u)

/// Durchlaufschleife über die Rückwärts-Nachbarn u des Knotens v
/// Der aktuelle Knoten wird mit *u oder u->... angesprochen.
#define FUER_ALLE_RNACHBARN(u, graph, v) \
    for(nachbarnMenge::const_iterator u = (graph).rNachbarn(v).begin(); u != (graph).rNachbarn(v).end(); ++u)

/***  Hilfsklasse zur Verbindung von Knoten- und Kantenindizes  ***/


class IndexPaar //: public std::pair<knotenIndex, size_t>
{
public:
    /// Knotenindex
    knotenIndex iKnoten;

    /// Kantenindex
    kantenIndex iKante;

    /// Standard- und Initialisierungskonstruktor
    IndexPaar(knotenIndex argKnoten = knotenIndex::KEIN_INDEX, kantenIndex argKante = kantenIndex::KEIN_INDEX)
        : iKnoten(argKnoten)
        , iKante(argKante)
    { }

    /// /// Vergleich miteinander
    /// bool operator==(IndexPaar const& paar) const { return iKnoten == paar.iKnoten && iKante == paar.iKante; }

    /// /// Vergleich miteinander
    /// bool operator!=(IndexPaar const& paar) const { return !(*this == paar); }

}; /// class IndexPaar

/// Definition des Typs der Nachbarnmengen.
/// Im Prinzip beliebiger Container, seine Verwendung
/// im Text muss bei Wechsel ggf. angepasst werden
typedef std::vector<IndexPaar> nachbarnMenge;

/**
 * Klasse für gerichtete oder ungerichtete Graphen
 */

class Graph
{
    /***  private Attribute  ***/

    /// Array der Knoten
    std::vector<Knoten> _knoten;

    /// Vorwärts-Nachbarnmengen aller Knoten
    std::vector<nachbarnMenge> _nachbarn;

    /// Rückwärts-Nachbarnmengen aller Knoten
    std::vector<nachbarnMenge> _rNachbarn;

    /// ist Objekt gerichtet?
    bool _gerichtet;

    /// Array der Kanten
    std::vector<Kante> _kanten;

    double _minX = std::numeric_limits<double>::max(), _maxX = std::numeric_limits<double>::min(),
           _minY = std::numeric_limits<double>::max(), _maxY = std::numeric_limits<double>::min();
    double _drawScale     = 1;
    double _maxGewicht    = std::numeric_limits<double>::min();
    double _minKnotenDist = 10;

public:
    /***  öffentliche Konstanten  ***/

    /// Gerichtetheit des Objekts
    static bool const GERICHTET   = true;
    static bool const UNGERICHTET = false;

    /***  Konstruktoren  ***/

    /// Initialisierungskonstruktor mit Anzahl Knoten
    Graph(size_t anzKnoten, bool gerichtet)
        : _knoten(anzKnoten)
        , _nachbarn(anzKnoten)
        , _rNachbarn(gerichtet ? anzKnoten : 0)
        , _gerichtet(gerichtet)
    { }

    /// Initialisierungskonstruktor von Datei
    Graph(std::string const& dateiName, bool gerichtet);

    /***  get-Methoden  ***/

    /// ist das Objekt gerichtet?
    bool gerichtet() const { return _gerichtet; }

    /// gib Anzahl Knoten aus
    size_t anzKnoten() const { return _knoten.size(); }

    /// gib Anzahl Kanten aus
    size_t anzKanten() const { return _kanten.size(); }

    double getDrawScale() const { return _drawScale; }
    double getMaxGewicht() const { return _maxGewicht; }
    double getMinKnotenDist() const { return _minKnotenDist; }

    /// gib Objekt des Knotens mit Index v aus
    /// wirf Exception, falls v zu groß
    Knoten const& knoten(knotenIndex v) const
    {
        if(v >= this->anzKnoten()) { throw "Graph::knoten(): Index zu gross!"; }
        return _knoten[v];
    }

    Knoten& knoten(knotenIndex v)
    {
        if(v >= this->anzKnoten()) { throw "Graph::knoten(): Index zu gross!"; }
        return _knoten[v];
    }

    /// gib Objekt der Kante mit Index e aus
    /// wirf Exception, falls e zu groß
    Kante const& kante(kantenIndex e) const
    {
        if(e >= this->anzKanten()) { throw "Graph::kante(): Index zu gross!"; }
        return _kanten[e];
    }

    /// gib Vorwärts-Nachbarnmenge des Knotens v aus
    /// wirf Exception, falls v ungültig
    nachbarnMenge const& nachbarn(knotenIndex v) const
    {
        if(v >= this->anzKnoten()) { throw std::string(__PRETTY_FUNCTION__) + ": Index zu gross!"; }
        return _nachbarn[v];
    }

    /// gib Rückwärts-Nachbarnmenge des Knotens v aus
    /// wirf Exception, falls v ungültig
    nachbarnMenge const& rNachbarn(knotenIndex v) const
    {
        if(v >= this->anzKnoten()) { throw std::string(__PRETTY_FUNCTION__) + ": Index zu gross!"; }

        /// Objekt ungerichtet -> rückwärts = vorwärts
        return _gerichtet ? _rNachbarn[v] : _nachbarn[v];
    }

    /// gib Vorwärts-Grad des Knotens v aus
    /// wirf Exception, falls v ungültig
    size_t grad(size_t v) const { return this->nachbarn(v).size(); }

    /// gib Rückwärts-Grad des Knotens v aus
    /// wirf Exception, falls v ungültig
    size_t rGrad(size_t v) const { return this->rNachbarn(v).size(); }

    /// gib Index der Kante (u,v), falls sie existiert
    /// KEIN_INDEX, falls nicht
    kantenIndex index(knotenIndex u, knotenIndex v) const;

    /// gib Index des Knotens zum Namen aus;
    /// KEIN_INDEX, falls kein solcher Knoten existiert
    knotenIndex index(std::string const& name) const;

    /***  set-Methoden  ***/

    /// füge neue Kante (u,v) bzw. {u,v} mit name zum Objekt
    /// hinzu, falls sie nicht existiert; gib ihren Index
    /// zurück oder KEIN_INDEX, falls existent
    kantenIndex setzeKante(std::string const& name, knotenIndex u, knotenIndex v);

    knotenIndex addNode(const Knoten&);

    void draw(Aether& aether) const;

    void saveToFile(std::string fileName) const;

}; /// class Graph

/***  Ausgabe auf ostream  ***/

std::ostream& operator<<(std::ostream& ostr, Graph const& graph);

std::pair<std::vector<double>, std::vector<knotenIndex>> dijkstra(Graph const& G, knotenIndex const& startKnoten);
