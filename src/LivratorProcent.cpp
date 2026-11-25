#include "LivratorProcent.hpp"
#include "Exceptii.hpp"

LivratorProcent::LivratorProcent(const std::string& numeFirma, double procentComision)
    : Livrator(numeFirma), procent(procentComision)
{
    if (procent < 0.0 || procent > 100.0)
        throw EroareDateInvalide("Comisionul trebuie sa fie intre 0 si 100.");
}

double LivratorProcent::taxaImpl(double valoareComanda) const
{
    return valoareComanda * procent / 100.0;
}

void LivratorProcent::afiseazaImpl(std::ostream& os) const
{
    os << "comision " << procent << "%";
}

std::unique_ptr<Livrator> LivratorProcent::clone() const
{
    return std::make_unique<LivratorProcent>(*this);
}
