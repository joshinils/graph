#include "Graph.h"
#include "Aether.h"
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
    if(gerichtet) _rNachbarn.resize(nKnoten);
    _gerichtet = gerichtet;

#if TALK
    cout << nKnoten << " Knoten" << '\n';
    cout << nKanten << " Kanten" << '\n';
    cout << '\n';
#endif
    /***  lese Knoten aus  ***/

    FUER_ALLE_KNOTEN(kId, *this)
    {
        Knoten& knoten = _knoten[kId];

        fin >> knoten.name >> knoten.xKoo >> knoten.yKoo;
        _minX = std::min(_minX, knoten.xKoo);
        _minY = std::min(_minY, knoten.yKoo);
        _maxX = std::max(_maxX, knoten.xKoo);
        _maxY = std::max(_maxY, knoten.yKoo);

        getline(fin, dummy); // ignoriere Rest der Zeile
    }
    //_drawScale = std::min(abs(_maxX - _minX), abs(_maxY - _minY));
    std::cout << "_drawScale: " << _drawScale << std::endl;

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

        _maxGewicht = std::max(_maxGewicht, kante.gewicht);

        // suche Knotenindizes zu den Namen
        kante.iFuss = this->index(fuss);
        kante.iKopf = this->index(kopf);

        // setze Nachbarnmengen
        _nachbarn[kante.iFuss].push_back(IndexPaar(kante.iKopf, i));
        if(_nachbarn.size() < kante.iFuss) { _nachbarn[kante.iFuss].push_back(IndexPaar(kante.iKopf, i)); }
        if(_gerichtet) { _rNachbarn[kante.iKopf].push_back(IndexPaar(kante.iFuss, i)); }
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

    /* initialisiere knotengrößen zum zeichnen */
    FUER_ALLE_KNOTEN(u, *this)
    {
        auto& ku = _knoten[u];
        olc::vd2d posKu(ku.xKoo, ku.yKoo);

        FUER_ALLE_KNOTEN(v, *this)
        {
            auto& kv = _knoten[v];
            olc::vd2d posKv(kv.xKoo, kv.yKoo);
            auto dist = (posKu - posKv).mag();
            //std::cout << "dist " << dist << " " << ku.name << " " << kv.name << '\n';
            if(dist > 0.01) { _minKnotenDist = std::min(_minKnotenDist, dist); }
            //std::cout << "minDist " << minDist << '\n';
        }
    }

    FUER_ALLE_KNOTEN(u, *this) { _knoten[u].nodeScale = _minKnotenDist; }
} // Graph::Graph()


/***  get-Methoden  ***/

// gib Index der Kante (u,v), falls sie existiert
// KEIN_INDEX, falls nicht
kantenIndex Graph::index(knotenIndex u, knotenIndex v) const
{
    if(u < this->anzKnoten() && v < this->anzKnoten())
    {
        FUER_ALLE_NACHBARN(w, *this, u)
        if(w->iKnoten == v) { return w->iKante; }
    }

    return kantenIndex::KEIN_INDEX;
}


// gib Index des Knotens zum Namen aus;
// KEIN_INDEX, falls kein solcher Knoten existiert
knotenIndex Graph::index(std::string const& knotenName) const
{
    // name nicht enthalten
    if(knotenName < _knoten[0].name || knotenName > _knoten.back().name) { return knotenIndex::KEIN_INDEX; }

    /*** binäre Suche  ***/

    knotenIndex li(0);
    knotenIndex re = _knoten.size() - 1;

    while(li < re)
    {
        knotenIndex mitte = (li + re) >> 1;

        if(knotenName <= _knoten[mitte].name) { re = mitte; }
        else
        {
            li = mitte + 1;
        }
    }

    if(_knoten[li].name == knotenName) { return li; }
    else
    {
        return knotenIndex::KEIN_INDEX;
    }

} // Graph::index()


// füge neue Kante (u,v) bzw. {u,v} mit name zum Objekt
// hinzu, falls sie nicht existiert; gib ihren Index
// zurück oder KEIN_INDEX, falls existent
kantenIndex Graph::setzeKante(std::string const& kantenName, knotenIndex u, knotenIndex v)
{
    // existiert Knotenpaar schon?
    if(kantenIndex::KEIN_INDEX != this->index(u, v)) { return kantenIndex::KEIN_INDEX; }

    // nächster freier Kantenindex
    kantenIndex kantenIdx(_kanten.size());

    // füge Kante ein
    _kanten.push_back(Kante(kantenName, u, v));
    _nachbarn[u].push_back(IndexPaar(v, kantenIdx));
    if(_gerichtet) { _rNachbarn[v].push_back(IndexPaar(u, kantenIdx)); }
    else
    {
        _nachbarn[v].push_back(IndexPaar(u, kantenIdx));
    }

    // gib Kantenindex zurück
    return kantenIdx;

} // setzeKante()

void Graph::draw(Aether& aether) const
{
    // draw nodes themselves
    FUER_ALLE_KNOTEN(kId, *this) { this->knoten(kId).drawCirc(aether, this->_drawScale); }

    // draw edges
    FUER_ALLE_KANTEN(eId, *this) { this->kante(eId).draw(aether, this); }

    // draw nodes names on top
    FUER_ALLE_KNOTEN(kId, *this) { this->knoten(kId).drawName(aether, this->_drawScale); }
}


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

std::pair<std::vector<double>, std::vector<knotenIndex>> dijkstra(Graph const& G, knotenIndex const& startKnoten)
{
    auto V = G.anzKnoten();

    std::vector<double> distances(V, std::numeric_limits<double>::max());
    std::vector<knotenIndex> vorgaenger(V, knotenIndex::KEIN_INDEX);
    auto cmp = [](knotenIndex const& a, knotenIndex const& b) { return false; }; //TODO
    std::priority_queue<knotenIndex, std::vector<knotenIndex>, decltype(cmp)> prioQueueW(cmp);
    prioQueueW.push(startKnoten);

    distances[startKnoten] = 0;

    while(!prioQueueW.empty())
    {
        auto u = prioQueueW.top(); // u = \argmin_{w \in prioQueueW} distances[w]
        prioQueueW.pop();
        for(auto vNM : G.rNachbarn(u)) // verlängere akt. s-u-Weg um (u, v)
        {
            auto v = vNM.iKnoten;
            // c(u, v) == kantenlänge(u, v)
            auto c = [&](knotenIndex u, knotenIndex v) { return G.kante(G.index(v, u)).gewicht; };
            if(distances[v] > distances[u] + c(u, v))
            {
                distances[v]  = distances[u] + c(u, v); // Optimalitätskriterium
                vorgaenger[v] = u; // merke Kante(u, v)
                prioQueueW.push(v); // aktualisiere v in prioQueueW
            }
        }
    }
    return std::make_pair(distances, vorgaenger);
}
