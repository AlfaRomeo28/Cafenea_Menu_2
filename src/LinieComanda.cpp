#include "LinieComanda.hpp"
#include "Exceptii.hpp"

LinieComanda::LinieComanda(const std::string& denumireProdus, int cant, double pret)
    : denumire(denumireProdus), cantitate(cant), pretUnitar(pret)
{
    if (denumire.empty())
        throw EroareDateInvalide("Denumirea produsului nu poate fi goala.");
    if (cantitate <= 0)
        throw EroareDateInvalide("Cantitatea trebuie sa fie pozitiva.");
    if (pretUnitar < 0.0)
        throw EroareDateInvalide("Pretul nu poate fi negativ.");
}

const std::string& LinieComanda::getDenumire() const { return denumire; }
int LinieComanda::getCantitate() const { return cantitate; }
double LinieComanda::getPretUnitar() const { return pretUnitar; }
double LinieComanda::getTotal() const { return cantitate * pretUnitar; }

void LinieComanda::adaugaCantitate(int cant)
{
    if (cant <= 0)
        throw EroareDateInvalide("Cantitatea adaugata trebuie sa fie pozitiva.");
    cantitate += cant;
}

std::ostream& operator<<(std::ostream& os, const LinieComanda& l)
{
    os << l.denumire << " x" << l.cantitate << " @ " << l.pretUnitar
       << " = " << l.getTotal() << " Lei leuti";
    return os;
}
