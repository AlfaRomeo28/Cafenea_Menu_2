#ifndef PROMOTIEPROCENT_HPP
#define PROMOTIEPROCENT_HPP

#include "Promotie.hpp"

// reducere procentuala pe fiecare bucata (ex: Happy Hour -20%)
class PromotieProcent : public Promotie
{
private:
    double procent;
protected:
    double totalImpl(double pretUnitar, int cantitate) const override;
    void afiseazaImpl(std::ostream& os) const override;
    bool esteProcentualaImpl() const override { return true; }
    double procentReducereImpl() const override { return procent; }

public:
    PromotieProcent(const std::string& numePromo, const std::string& produs, double procentReducere,
                    bool esteActiva = false);
    std::unique_ptr<Promotie> clone() const override;
    
};

#endif
