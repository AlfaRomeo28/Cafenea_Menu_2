#include "Promotie.hpp"
#include <utility>
#include "Exceptii.hpp"

int Promotie::numarInstante = 0;

Promotie::Promotie(std::string numePromo, std::string produs, bool esteActiva)
    : nume(std::move(numePromo)), produsVizat(std::move(produs)), activa(esteActiva)
{
    if (nume.empty())
        throw EroareDateInvalide("Numele promotiei nu poate fi gol.");
    if (produsVizat.empty())
        throw EroareDateInvalide("Promotia trebuie sa vizeze un produs.");
    ++numarInstante;
}

Promotie::Promotie(const Promotie& alta)
    : nume(alta.nume), produsVizat(alta.produsVizat), activa(alta.activa)
{
    ++numarInstante;
}

Promotie::~Promotie()
{
    --numarInstante;
}

const std::string& Promotie::getNume() const { return nume; }
const std::string& Promotie::getProdusVizat() const { return produsVizat; }
bool Promotie::esteActiva() const { return activa; }
int Promotie::getNumarInstante() { return numarInstante; }

bool Promotie::aplicabila(const std::string& produs) const
{
    return activa && produs == produsVizat;
}

double Promotie::totalDupaPromotie(double pretUnitar, int cantitate) const
{
    if (pretUnitar < 0.0)
        throw EroareDateInvalide("Pretul unitar nu poate fi negativ.");
    if (cantitate <= 0)
        throw EroareDateInvalide("Cantitatea trebuie sa fie pozitiva.");
    if (!activa)
        return pretUnitar * cantitate;
    return totalImpl(pretUnitar, cantitate);
}

double Promotie::economie(double pretUnitar, int cantitate) const
{
    return pretUnitar * cantitate - totalDupaPromotie(pretUnitar, cantitate);
}

void Promotie::activeaza() { activa = true; }
void Promotie::dezactiveaza() { activa = false; }

std::ostream& operator<<(std::ostream& os, const Promotie& p)
{
    os << p.nume << " | Produs: " << p.produsVizat << " | ";
    p.afiseazaImpl(os);                               // partea specifica tipului
    os << " | Stare: " << (p.activa ? "ACTIVA" : "oprita");
    return os;
}
