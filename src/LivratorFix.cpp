#include "LivratorFix.hpp"
#include "Exceptii.hpp"

LivratorFix::LivratorFix(const std::string& numeFirma, double taxa)
    : Livrator(numeFirma), taxaFixa(taxa)
{
    if (taxaFixa < 0.0)
        throw EroareDateInvalide("Taxa de livrare nu poate fi negativa.");
}

double LivratorFix::taxaImpl(double) const { return taxaFixa; }

void LivratorFix::afiseazaImpl(std::ostream& os) const
{
    os << "taxa fixa " << taxaFixa << " Lei leuti";
}

std::unique_ptr<Livrator> LivratorFix::clone() const
{
    return std::make_unique<LivratorFix>(*this);
}
