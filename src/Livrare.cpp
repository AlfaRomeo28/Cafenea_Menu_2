#include "Livrare.hpp"
#include <utility>
#include "Exceptii.hpp"

Livrare::Livrare(std::vector<std::string> produseLivrate, const Livrator& firma, double valoare)
    : produse(std::move(produseLivrate)), livrator(firma.clone()), valoareComanda(valoare),
      taxa(firma.taxaLivrare(valoare))
{
    if (produse.empty())
        throw EroareDateInvalide("Livrarea trebuie sa contina cel putin un produs.");
}

Livrare::Livrare(const Livrare& alta)
    : produse(alta.produse), livrator(alta.livrator->clone()), valoareComanda(alta.valoareComanda),
      taxa(alta.taxa)
{
}

void swap(Livrare& a, Livrare& b) noexcept
{
    using std::swap;
    swap(a.produse, b.produse);
    swap(a.livrator, b.livrator);
    swap(a.valoareComanda, b.valoareComanda);
    swap(a.taxa, b.taxa);
}

Livrare& Livrare::operator=(Livrare alta)
{
    swap(*this, alta);
    return *this;
}

const std::string& Livrare::getFirma() const { return livrator->getFirma(); }

double Livrare::getTaxa() const { return taxa; }

std::ostream& operator<<(std::ostream& os, const Livrare& l)
{
    os << "Livrare prin " << *l.livrator << " | Produse:";
    for (const std::string& p : l.produse)
        os << " " << p << ";";
    return os << " | Valoare: " << l.valoareComanda << " Lei leuti | De plata firmei: " << l.taxa
              << " Lei leuti";
}
