#include "PromotiePragCantitate.hpp"
#include "Exceptii.hpp"

PromotiePragCantitate::PromotiePragCantitate(const std::string& numePromo, const std::string& produs,
                                             int pragBucati, double procentReducere, bool esteActiva)
    : Promotie(numePromo, produs, esteActiva), prag(pragBucati), procent(procentReducere)
{
    if (prag < 2)
        throw EroareDateInvalide("Pragul de cantitate trebuie sa fie cel putin 2.");
    if (procent <= 0.0 || procent > 90.0)
        throw EroareDateInvalide("Reducerea trebuie sa fie intre 0 (exclusiv) si 90%.");
}

double PromotiePragCantitate::totalImpl(double pretUnitar, int cantitate) const
{
    double total = pretUnitar * cantitate;
    if (cantitate >= prag)
        total *= (1.0 - procent / 100.0);
    return total;
}

void PromotiePragCantitate::afiseazaImpl(std::ostream& os) const
{
    os << "-" << procent << "% la minim " << prag << " bucati";
}

std::unique_ptr<Promotie> PromotiePragCantitate::clone() const
{
    return std::make_unique<PromotiePragCantitate>(*this);
}
