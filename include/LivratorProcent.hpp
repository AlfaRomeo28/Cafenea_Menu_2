#ifndef LIVRATORPROCENT_HPP
#define LIVRATORPROCENT_HPP

#include "Livrator.hpp"

// firma ia un comision procentual din valoarea comenzii
class LivratorProcent : public Livrator
{
private:
    double procent;

protected:
    double taxaImpl(double valoareComanda) const override;
    void afiseazaImpl(std::ostream& os) const override;

public:
    LivratorProcent(const std::string& numeFirma, double procentComision);
    std::unique_ptr<Livrator> clone() const override;
};

#endif
