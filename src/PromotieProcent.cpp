#include "PromotieProcent.hpp"
#include "Exceptii.hpp"

namespace
{
    const double REDUCERE_MAXIMA = 90.0;
}

PromotieProcent::PromotieProcent(const std::string& numePromo, const std::string& produs,
                                 double procentReducere, bool esteActiva)
    : Promotie(numePromo, produs, esteActiva), procent(procentReducere)
{
    if (procent <= 0.0 || procent > REDUCERE_MAXIMA)
        throw EroareDateInvalide("Reducerea procentuala trebuie sa fie intre 0 (exclusiv) si 90%.");
}

double PromotieProcent::totalImpl(double pretUnitar, int cantitate) const
{
    return pretUnitar * cantitate * (1.0 - procent / 100.0);
}

void PromotieProcent::afiseazaImpl(std::ostream& os) const
{
    os << "Reducere: " << procent << "% pe bucata";
}

std::unique_ptr<Promotie> PromotieProcent::clone() const
{
    return std::make_unique<PromotieProcent>(*this);
}


