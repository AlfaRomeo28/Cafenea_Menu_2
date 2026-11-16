#ifndef INCARCATORDATE_HPP
#define INCARCATORDATE_HPP

#include <memory>
#include <string>
#include <vector>
#include "Promotie.hpp"
#include "Reteta.hpp"

// citeste retete si promotii din fisiere JSON (biblioteca nlohmann/json este folosita doar in .cpp)
class IncarcatorDate
{
public:
    IncarcatorDate() = delete;

    // arunca EroareFisier (lipsa/JSON gresit) sau EroareDateInvalide (valori gresite)
    static std::vector<Reteta> incarcaRetete(const std::string& cale);
    static std::vector<std::unique_ptr<Promotie> > incarcaPromotii(const std::string& cale);
};

#endif
