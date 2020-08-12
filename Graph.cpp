#include "Graph.h"
#include <algorithm> // sort
#include <fstream>
#include <iomanip> // für setw()
#include <sstream> // für istringstream

#define TALK 0 // Debugging-Output?


// Initialisierungskonstruktor von Datei
Graph::Graph(std::string const& dateiName, bool gerichtet)
{
    std::ifstream fin(dateiName.c_str());

    if(!fin) throw "Graph::Graph(): Datei kann nicht geoeffnet werden!";

#if TALK
    cout << "Lese Input-Graph " << dateiName << '\n';
#endif
    /***  lese Graphparameter aus  ***/

    size_t nKnoten, nKanten;
    std::string dummy;

    fin >> nKnoten;
    getline(fin, dummy); // ignoriere Rest der Zeile
    fin >> nKanten;
    getline(fin, dummy); // ignoriere Rest der Zeile

    // setze Objektattribute
    _knoten.resize(nKnoten);
    _kanten.resize(nKanten);
    _nachbarn.resize(nKnoten);
    if(gerichtet) _rnachbarn.resize(nKnoten);
    _gerichtet = gerichtet;

#if TALK
    cout << nKnoten << " Knoten" << '\n';
    cout << nKanten << " Kanten" << '\n';
    cout << '\n';
#endif
    /***  lese Knoten aus  ***/

    for(size_t i = 0; i < _knoten.size(); ++i)
    {
        Knoten& knoten = _knoten[i];

        fin >> knoten.name >> knoten.xKoo >> knoten.yKoo;
        getline(fin, dummy); // ignoriere Rest der Zeile
    }

#if TALK
    cout << "Knoten gelesen" << '\n';
#endif

    // Sortiere Knotennamen für binäre Suche in index( string const& ).
    // Eine Hash-Tabelle wäre effizienter für die Knotenzuordnung beim
    // Einlesen der Kanten, dann gäbe es aber die o.a. Methode index()
    // nicht für das weitere Leben des Graph-Objekts. Der Zeit-Tradeoff
    // "bin. Suche vs. Hashing" ist auch bei großen Input-Graphen kaum
    // zu merken.
    std::sort(_knoten.begin(), _knoten.end());

    /***  lese Kanten aus  ***/

    std::string zeile;

    // ignoriere Leerzeilen zwischen Knoten- und Kantenblock
    do
    {
        getline(fin, zeile);
        if(fin.eof()) { break; }
    } while(zeile.empty());

    // lese alle Kanten aus
    for(knotenIndex i = 0; i < _kanten.size(); ++i)
    {
        Kante& kante = _kanten[i];

        // lese Kantenzeile indirekt, damit gewicht ggf. Defaultwert
        // bekommt, wenn keine Gewichte gegeben sind
        std::string fuss, kopf;
        std::istringstream(zeile) >> kante.name >> fuss >> kopf >> kante.gewicht;

        // suche Knotenindizes zu den Namen
        kante.iFuss = this->index(fuss);
        kante.iKopf = this->index(kopf);

        // setze Nachbarnmengen
        IndexPaar foo = IndexPaar(kante.iKopf, i);
        if(_nachbarn.size() < kante.iFuss) { _nachbarn[kante.iFuss].push_back(foo); }
        if(_gerichtet) { _rnachbarn[kante.iKopf].push_back(IndexPaar(kante.iFuss, i)); }
        else
        {
            _nachbarn[kante.iKopf].push_back(IndexPaar(kante.iFuss, i));
        }

        // lese nächste Zeile
        bool breakFor = false;
        do
        {
            getline(fin, zeile);
            if((breakFor = fin.eof())) { break; }
        } while(zeile.empty());

        if(breakFor) { break; }

    } // for ( i )

#if TALK
    cout << "Kanten gelesen" << '\n';
    cout << *this << '\n';
#endif
    /***  räume auf  ***/

    fin.close();

} // Graph::Graph()


/***  get-Methoden  ***/

// gib Index der Kante (u,v), falls sie existiert
// KEIN_INDEX, falls nicht
knotenIndex Graph::index(knotenIndex u, knotenIndex v) const
{
    if(u < this->anzKnoten() && v < this->anzKnoten())
    {
        FUER_ALLE_NACHBARN(w, *this, u)
        if(w->iKnoten == v) { return w->iKante; }
    }

    return KEIN_INDEX;
}


// gib Index des Knotens zum Namen aus;
// KEIN_INDEX, falls kein solcher Knoten existiert
knotenIndex Graph::index(std::string const& knotenName) const
{
    // name nicht enthalten
    if(knotenName < _knoten[0].name || knotenName > _knoten.back().name) { return KEIN_INDEX; }

    /*** binäre Suche  ***/

    size_t li = 0;
    size_t re = _knoten.size() - 1;

    while(li < re)
    {
        size_t mitte = (li + re) >> 1;

        if(knotenName <= _knoten[mitte].name) { re = mitte; }
        else
        {
            li = mitte + 1;
        }
    }

    if(_knoten[li].name == knotenName) { return li; }
    else
    {
        return KEIN_INDEX;
    }

} // Graph::index()


// füge neue Kante (u,v) bzw. {u,v} mit name zum Objekt
// hinzu, falls sie nicht existiert; gib ihren Index
// zurück oder KEIN_INDEX, falls existent
knotenIndex Graph::setzeKante(std::string const& kantenName, knotenIndex u, knotenIndex v)
{
    // existiert Knotenpaar schon?
    if(KEIN_INDEX != this->index(u, v)) { return KEIN_INDEX; }

    // nächster freier Kantenindex
    size_t kantenIndex = _kanten.size();

    // füge Kante ein
    _kanten.push_back(Kante(kantenName, u, v));
    _nachbarn[u].push_back(IndexPaar(v, kantenIndex));
    if(_gerichtet) { _rnachbarn[v].push_back(IndexPaar(u, kantenIndex)); }
    else
    {
        _nachbarn[v].push_back(IndexPaar(u, kantenIndex));
    }

    // gib Kantenindex zurück
    return kantenIndex;

} // setzeKante()


/***  Ausgabe auf ostream  ***/

std::ostream& operator<<(std::ostream& ostr, Graph const& graph)
{
    ostr << "Graph:  " << graph.anzKnoten() << " Knoten, " << graph.anzKanten() << " Kanten" << '\n';
    ostr << '\n';

    // berechne max. Breite der Knotenname
    size_t breite = 0;
    FUER_ALLE_KNOTEN(u, graph)
    {
        if(breite < graph.knoten(u).name.size()) breite = graph.knoten(u).name.size();
    }

    // gebe die Knoten mit ihren Nachbarnmengen aus
    FUER_ALLE_KNOTEN(u, graph)
    {
        ostr << std::setw(breite) << graph.knoten(u) << " :";

        if(!graph.gerichtet())
        {
            FUER_ALLE_NACHBARN(v, graph, u)
            {
                ostr << " -" << graph.kante(v->iKante) << "->" << graph.knoten(v->iKnoten).name;
            }
        }
        else
        {
            FUER_ALLE_RNACHBARN(v, graph, u)
            {
                ostr << " <-" << graph.kante(v->iKante) << "-" << graph.knoten(v->iKnoten).name;
            }
        }
        ostr << '\n';
    } // FUER_ALLE_KNOTEN( u )

    return ostr;

} // operator <<
