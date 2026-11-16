#include "Reteta.hpp"
#include <algorithm>
#include <utility>
#include "Exceptii.hpp"

Reteta::Reteta(std::string denumire, double pret, std::vector<CReteta> comp)
    : denBaut(std::move(denumire)), pretV(pret), componente(std::move(comp))
{
    if (denBaut.empty())
        throw EroareDateInvalide("Reteta trebuie sa aiba o denumire.");
    if (pretV < 0.0)
        throw EroareDateInvalide("Pretul retetei '" + denBaut + "' nu poate fi negativ.");
    if (componente.empty())
        throw EroareDateInvalide("Reteta '" + denBaut + "' nu are ingrediente.");
}

const std::string& Reteta::getDenumire() const { return denBaut; }
double Reteta::getPretVanzare() const { return pretV; }

void Reteta::adaugaLaNecesar(std::vector<CReteta>& necesar) const
{
    for (const CReteta& comp : componente)
    {
        auto it = std::find_if(necesar.begin(), necesar.end(),
            [&comp](const CReteta& n) { return n.getnume() == comp.getnume(); });
        if (it == necesar.end())
            necesar.push_back(comp);
        else
            *it = CReteta(comp.getnume(), it->getcantitate() + comp.getcantitate());
    }
}

double Reteta::calculeazaCostProductie(const std::vector<Ingrediente>& stoc) const
{
    double costTotal = 0.0;
    for (const CReteta& comp : componente)
    {
        auto it = std::find_if(stoc.begin(), stoc.end(),
            [&comp](const Ingrediente& ing) { return ing.getdenumire() == comp.getnume(); });
        if (it != stoc.end())
            costTotal += comp.getcantitate() * it->getpretUnitate();
    }
    return costTotal;
}

std::ostream& operator<<(std::ostream& os, const Reteta& ret)
{
    os << "Produs: " << ret.denBaut << " | Pret: " << ret.pretV << " Lei leuti | Ingrediente: [";
    for (std::size_t i = 0; i < ret.componente.size(); ++i)
    {
        os << ret.componente[i];
        if (i + 1 < ret.componente.size())
            os << ", ";
    }
    os << "]";
    return os;
}
