#ifndef PROMOTIENPLUSUNU_HPP
#define PROMOTIENPLUSUNU_HPP

#include "Promotie.hpp"

// "cumperi N, primesti 1 gratis": la fiecare grup de N+1 bucati, una nu se plateste
class PromotieNPlusUnu : public Promotie
{
private:
    int bucatiPlatite;
protected:
    double totalImpl(double pretUnitar, int cantitate) const override;
    void afiseazaImpl(std::ostream& os) const override;
public:
    PromotieNPlusUnu(const std::string& numePromo, const std::string& produs, int cumperi,
                     bool esteActiva = false);
    std::unique_ptr<Promotie> clone() const override;
};

#endif
