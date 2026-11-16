#ifndef PROMOTIEPRAGCANTITATE_HPP
#define PROMOTIEPRAGCANTITATE_HPP

#include "Promotie.hpp"

// reducere aplicata doar daca se comanda cel putin `prag` bucati din produs
class PromotiePragCantitate : public Promotie
{
private:
    int prag;
    double procent;
protected:
    double totalImpl(double pretUnitar, int cantitate) const override;
    void afiseazaImpl(std::ostream& os) const override;
public:
    PromotiePragCantitate(const std::string& numePromo, const std::string& produs, int pragBucati,
                          double procentReducere, bool esteActiva = false);
    std::unique_ptr<Promotie> clone() const override;
};

#endif
