#include "Knoten.h"
#include "Aether.h"

void Knoten::drawCirc(Aether& aether, double drawScale) const
{
    aether.FillCircle(this->xKoo * drawScale, this->yKoo * drawScale, nodeScale * 8);
}

void Knoten::drawName(Aether& aether, double drawScale) const
{
    aether.DrawString(this->xKoo * drawScale - 3.5 * nodeScale,
                      this->yKoo * drawScale - 3.5 * nodeScale,
                      this->name,
                      olc::DARK_GREY,
                      nodeScale);
}
