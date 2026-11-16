#include "IncarcatorDate.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include "Exceptii.hpp"
#include "PromotieNPlusUnu.hpp"
#include "PromotiePragCantitate.hpp"
#include "PromotieProcent.hpp"

using json = nlohmann::json;

namespace
{
    json citesteLista(const std::string& cale)
    {
        std::ifstream fisier(cale);
        if (!fisier.is_open())
            throw EroareFisier(cale, "nu pot deschide fisierul");

        json continut;
        try
        {
            continut = json::parse(fisier);
        }
        catch (const json::exception& e)
        {
            throw EroareFisier(cale, std::string("JSON invalid: ") + e.what());
        }
        if (!continut.is_array())
            throw EroareFisier(cale, "se astepta o lista de obiecte JSON");
        return continut;
    }

    Reteta creeazaReteta(const json& j)
    {
        std::vector<CReteta> componente;
        for (const json& c : j.at("ingrediente"))
            componente.emplace_back(c.at("nume").get<std::string>(), c.at("cantitate").get<double>());
        return Reteta(j.at("denumire").get<std::string>(), j.at("pret").get<double>(), componente);
    }

    // alege clasa derivata dupa campul "tip"
    std::unique_ptr<Promotie> creeazaPromotie(const json& j)
    {
        const std::string tip = j.at("tip").get<std::string>();
        const std::string numePromo = j.at("nume").get<std::string>();
        const std::string produs = j.at("produs").get<std::string>();
        const bool activa = j.value("activa", false);

        if (tip == "procent")
            return std::make_unique<PromotieProcent>(numePromo, produs, j.at("procent").get<double>(), activa);
        if (tip == "n_plus_unu")
            return std::make_unique<PromotieNPlusUnu>(numePromo, produs, j.at("n").get<int>(), activa);
        if (tip == "prag")
            return std::make_unique<PromotiePragCantitate>(numePromo, produs, j.at("prag").get<int>(),
                                                           j.at("procent").get<double>(), activa);
        throw EroareDateInvalide("tip de promotie necunoscut: '" + tip + "'");
    }
}

std::vector<Reteta> IncarcatorDate::incarcaRetete(const std::string& cale)
{
    std::vector<Reteta> retete;
    try
    {
        for (const json& j : citesteLista(cale))
            retete.push_back(creeazaReteta(j));
    }
    catch (const json::exception& e)
    {
        throw EroareFisier(cale, std::string("camp lipsa sau de tip gresit: ") + e.what());
    }
    return retete;
}

std::vector<std::unique_ptr<Promotie> > IncarcatorDate::incarcaPromotii(const std::string& cale)
{
    std::vector<std::unique_ptr<Promotie> > promotii;
    try
    {
        for (const json& j : citesteLista(cale))
            promotii.push_back(creeazaPromotie(j));
    }
    catch (const json::exception& e)
    {
        throw EroareFisier(cale, std::string("camp lipsa sau de tip gresit: ") + e.what());
    }
    return promotii;
}
