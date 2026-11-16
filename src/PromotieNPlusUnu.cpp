#include "PromotieNPlusUnu.hpp"
#include "Exceptii.hpp"

PromotieNPlusUnu::PromotieNPlusUnu(const std::string& numePromo, const std::string& produs,
                                   int cumperi, bool esteActiva)
    : Promotie(numePromo, produs, esteActiva), bucatiPlatite(cumperi)
{
    if (bucatiPlatite < 1)
        throw EroareDateInvalide("Promotia N+1 cere cel putin o bucata platita.");
}

double PromotieNPlusUnu::totalImpl(double pretUnitar, int cantitate) const
{
    int gratuite = cantitate / (bucatiPlatite + 1);
    return pretUnitar * (cantitate - gratuite);
}

void PromotieNPlusUnu::afiseazaImpl(std::ostream& os) const
{
    os << "Cumperi " << bucatiPlatite << ", primesti 1 gratis";
}

std::unique_ptr<Promotie> PromotieNPlusUnu::clone() const
{
    return std::make_unique<PromotieNPlusUnu>(*this);
}
