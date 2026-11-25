#include "Livrator.hpp"
#include <utility>
#include "Exceptii.hpp"

Livrator::Livrator(std::string numeFirma) : firma(std::move(numeFirma))
{
    if (firma.empty())
        throw EroareDateInvalide("Firma de livrare trebuie sa aiba un nume.");
}

Livrator::Livrator(const Livrator&) = default;

Livrator::~Livrator() = default;

const std::string& Livrator::getFirma() const { return firma; }

double Livrator::taxaLivrare(double valoareComanda) const
{
    if (valoareComanda < 0.0)
        throw EroareDateInvalide("Valoarea comenzii nu poate fi negativa.");
    return taxaImpl(valoareComanda);
}

std::ostream& operator<<(std::ostream& os, const Livrator& l)
{
    os << l.firma << " (";
    l.afiseazaImpl(os);
    return os << ")";
}
