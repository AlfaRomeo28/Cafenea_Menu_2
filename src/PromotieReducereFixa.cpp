#include "PromotieReducereFixa.hpp"
#include <algorithm>
#include "Exceptii.hpp"

PromotieReducereFixa::PromotieReducereFixa(const std::string& numePromo, const std::string& produs,
                                           double suma, bool esteActiva)
    : Promotie(numePromo, produs, esteActiva), sumaPeBucata(suma)
{
    if (sumaPeBucata <= 0.0)
        throw EroareDateInvalide("Reducerea fixa trebuie sa fie pozitiva.");
}

// pretul unei bucati nu coboara sub 0
double PromotieReducereFixa::totalImpl(double pretUnitar, int cantitate) const
{
    return std::max(0.0, pretUnitar - sumaPeBucata) * cantitate;
}

void PromotieReducereFixa::afiseazaImpl(std::ostream& os) const
{
    os << "-" << sumaPeBucata << " Lei leuti pe bucata";
}

std::unique_ptr<Promotie> PromotieReducereFixa::clone() const
{
    return std::make_unique<PromotieReducereFixa>(*this);
}
