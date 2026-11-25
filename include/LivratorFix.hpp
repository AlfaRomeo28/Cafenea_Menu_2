#ifndef LIVRATORFIX_HPP
#define LIVRATORFIX_HPP

#include "Livrator.hpp"

// firma cere o taxa fixa pe livrare
class LivratorFix : public Livrator
{
private:
    double taxaFixa;

protected:
    double taxaImpl(double valoareComanda) const override;
    void afiseazaImpl(std::ostream& os) const override;

public:
    LivratorFix(const std::string& numeFirma, double taxa);
    std::unique_ptr<Livrator> clone() const override;
};

#endif
