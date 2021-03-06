#include "Aether.h"
#include <iostream>

#include "Graph.h"

#define _USE_MATH_DEFINES
#include <math.h>

Aether::Aether(std::unique_ptr<Graph> g)
    : _graph(std::move(g))
{ }

bool Aether::OnUserCreate()
{
    olc::PixelGameEngine::SetPixelMode(olc::Pixel::ALPHA);

    Plane::OnUserCreate();
    std::cout << __FUNCTION__ << std::endl;

    return true;
}

#include <sstream>
bool Aether::OnUserUpdate(float fElapsedTime)
{
    Plane::OnUserUpdate(fElapsedTime);

    auto mouseX{ stocx(GetMouseX()) };
    auto mouseY{ stocy(GetMouseY()) };
    olc::PixelGameEngine::DrawString(
    0, 0, "Mouse: " + std::to_string(mouseX) + ", " + std::to_string(mouseY), olc::RED);
    olc::PixelGameEngine::DrawString(0, 9, "Scale: " + std::to_string(getScale()), olc::RED);


    auto r = dijkstra(*_graph, 0);
    FUER_ALLE_KNOTEN(uID, *_graph)
    {
        std::stringstream ss;
        ss << r.first.at(uID);
        _graph->knoten(uID).setName(ss.str());
    }
    FUER_ALLE_KNOTEN(uID, *_graph)
    {
        std::stringstream ss;
        ss << r.first.at(uID);
        std::cout << ss.str() << '\n';
    }

    this->_graph->draw(*this);

    /// FillCircle(mouseX, mouseY, 10, olc::RED);
    /// size_t segments = 100;
    /// auto f          = [&](size_t arg) {
    ///     double t   = arg * 2 * M_PI / segments;
    ///     double r_x = ScreenWidth() * 0.5;
    ///     double r_y = ScreenHeight() * 0.5;
    ///     double m   = std::min(r_x, r_y) * 0.1;
    ///     r_x -= m;
    ///     r_y -= m;
    ///     return olc::vf2d(r_x * sin(t), r_y * cos(t));
    /// };

    /// for(size_t i = 0; i < segments; i++)
    /// {
    ///     auto a = f(i);
    ///     auto b = f((i + 1) % segments);
    ///     this->FillLine(a, b, 10);
    /// }

    return true;
}

olc::vf2d turnVec(olc::vf2d const& in, float angle)
{
    auto cs = cos(angle);
    auto sn = sin(angle);
    return { in.x * cs - in.y * sn, in.x * sn + in.y * cs };
}

void Aether::DrawArrow(olc::vf2d start, olc::vf2d end, double width, olc::Pixel colorTo, olc::Pixel colorFrom)
{
    end -= width / 2 * (end - start).norm();
    olc::vf2d lineDirection = (end - start);
    double screenlength     = ctos(lineDirection).mag();
    int segments            = std::min(100., floor(screenlength));

    float coordLength   = std::min(100.f, lineDirection.mag());
    double segmentDelta = coordLength / segments;

    lineDirection = lineDirection.norm();
    for(int i = 0; i < segments; ++i)
    {
        //std::cout << "coordLength " << coordLength << " i " << i << " i/t " << i / coordLength << std::endl;
        FillLine(start + lineDirection * i * segmentDelta,
                 start + lineDirection * (i + 1.) * segmentDelta,
                 width,
                 average(colorTo, colorFrom, ((float)i / segments), (1. - (float)i / segments)));
    }


    auto turned0 = turnVec((start - end).norm(), float(M_PI / 11));
    auto turned1 = turnVec((start - end).norm(), float(-M_PI / 11));

    FillCircle(end, width / 2, colorTo);

    FillLine(end, end + turned0 * (float)width * 5, width * 8 / 10, colorTo);
    FillLine(end, end + turned1 * (float)width * 5, width * 8 / 10, colorTo);

    //FillCircle(start, 10, colorFrom);
    //FillCircle(end,   10, colorTo);
}

void Aether::FillLine(olc::vf2d start, olc::vf2d end, double width, olc::Pixel color)
{
    olc::vf2d lineDirection = (start - end).norm();
    olc::vf2d orthog{ -lineDirection.y, lineDirection.x };
    orthog *= (float)width / 2;
    auto corner0 = start + orthog;
    auto corner1 = start - orthog;
    auto corner2 = end + orthog;
    auto corner3 = end - orthog;

    DrawWarpedDecal(_gradientDecal, { ctos(corner0), ctos(corner1), ctos(corner3), ctos(corner2) }, color);
    //Plane::FillTriangle(corner0, corner1, corner2, color);
    //Plane::FillTriangle(corner3, corner1, corner2, color);
}

olc::Pixel operator*(olc::Pixel const& p, float fac)
{
    return olc::Pixel(uint8_t(p.r * fac), uint8_t(p.g * fac), uint8_t(p.b * fac), uint8_t(p.a * fac));
}

olc::Pixel operator+(olc::Pixel const& a, olc::Pixel const& b)
{
    return olc::Pixel(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);
}

/// correctly blends color; see: https://www.youtube.com/watch?v=LKnqECcg6Gw
olc::Pixel average(olc::Pixel const& left, olc::Pixel const& right, double weightA, double weightB)
{
    uint8_t r = (uint8_t)sqrt((uint64_t)left.r * left.r * weightA + (uint64_t)right.r * right.r * weightB);
    uint8_t g = (uint8_t)sqrt((uint64_t)left.g * left.g * weightA + (uint64_t)right.g * right.g * weightB);
    uint8_t b = (uint8_t)sqrt((uint64_t)left.b * left.b * weightA + (uint64_t)right.b * right.b * weightB);
    uint8_t a = (uint8_t)sqrt((uint64_t)left.a * left.a * weightA + (uint64_t)right.a * right.a * weightB);
    return olc::Pixel(r, g, b, a);
}
