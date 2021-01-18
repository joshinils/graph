#include "Aether.h"
#include "Graph.h"
#include <iostream>
#include <memory>
#include <stdexcept>


int main(int narg, char* argv[])
try
{
    /// gebe Usage aus
    if(narg < 2)
    {
        std::cout << "usage: " << argv[0] << "  dateiname [ungerichtet]" << '\n';
        std::cout << "       dateiname  : Name einer Graph-Datei" << '\n';
        std::cout << "       ungerichtet: irgendein Zeichen" << '\n';
        std::cout << "Liest den Graphen aus der Datei aus und zeigt ihn an." << '\n';
        std::cout << "Interpretiert ihn ungerichtet, falls ein zusaetzliches" << '\n';
        std::cout << "Zeichen gegeben ist." << '\n';

        return 0;
    }

    /***  lese Graph ein  ***/

    std::string const& dateiName = argv[1];
    bool richtung                = (narg > 2 ? Graph::UNGERICHTET : Graph::GERICHTET);
    Graph graph(dateiName, richtung);

    std::cout << graph << '\n';


    double scale = 100;
    Aether world(std::make_unique<Graph>(graph));
    if(world.Construct((uint32_t)(16.0 * scale),
                       (uint32_t)(9.0 * scale),
                       (uint32_t)std::max(1.0, 120 / scale),
                       (uint32_t)std::max(1.0, 120 / scale)))
    { world.Start(); }

    return 0;

} /// main()

catch(char const text[])
{
    std::cerr << text << '\n';
} catch(std::exception const& exc)
{
    std::cerr << exc.what() << '\n';
}
