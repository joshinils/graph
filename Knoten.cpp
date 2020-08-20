#include "Knoten.h"
#include "Aether.h"

void Knoten::drawCirc(Aether& aether, double drawScale) const
{
    aether.FillCircle(this->xKoo * drawScale, this->yKoo * drawScale, nodeScale * drawScale / 4 / 3);
}

void Knoten::drawName(Aether& aether, double drawScale) const
{
    aether.DrawStringDecalMinScale(
    { float((this->xKoo /*- 1 * nodeScale*/) * drawScale), float((this->yKoo /*- 1 * nodeScale*/) * drawScale) },
    this->name,
    olc::DARK_GREY,
    { nodeScale * drawScale / 100, nodeScale * drawScale / 100 });
}
