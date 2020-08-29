#include "Kante.h"
#include "Aether.h"

void Kante::draw(Aether& aether, const Graph* const graph) const
{
    Knoten const& kFuss = graph->knoten(iFuss);
    olc::vf2d start     = olc::vf2d(kFuss.xKoo, kFuss.yKoo) * graph->getDrawScale();

    Knoten const& kKopf  = graph->knoten(iKopf);
    olc::vf2d const& end = olc::vf2d(kKopf.xKoo, kKopf.yKoo) * graph->getDrawScale();

    olc::vf2d dir = (start - end).norm();

    aether.DrawArrow(start /*- kFuss.nodeScale * dir * graph->getDrawScale() / 100*/,
                     end /*+ kKopf.nodeScale * dir * graph->getDrawScale() / 100*/,
                     (gewicht / graph->getMaxGewicht()) * graph->getDrawScale() * graph->getMinKnotenDist(),
                     olc::GREEN,
                     olc::RED);
}
