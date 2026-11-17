#ifndef PROMOTIEREDUCEREFIXA_HPP
#define PROMOTIEREDUCEREFIXA_HPP

#include "Promotie.hpp"

// reducere in lei, fixa, pe fiecare bucata (ex: -2 Lei la fiecare Espresso)
class PromotieReducereFixa : public Promotie
{
private:
    double sumaPeBucata;
protected:
    double totalImpl(double pretUnitar, int cantitate) const override;
    void afiseazaImpl(std::ostream& os) const override;
public:
    PromotieReducereFixa(const std::string& numePromo, const std::string& produs, double suma,
                         bool esteActiva = false);
    std::unique_ptr<Promotie> clone() const override;
};

#endif
