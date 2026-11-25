#ifndef LIVRATOR_HPP
#define LIVRATOR_HPP

#include <iostream>
#include <memory>
#include <string>

// Baza abstracta: fiecare firma de livrare isi calculeaza altfel taxa (NVI).
class Livrator
{
private:
    std::string firma;

protected:
    Livrator(const Livrator& alta);
    Livrator& operator=(const Livrator&) = delete;

    virtual double taxaImpl(double valoareComanda) const = 0;
    virtual void afiseazaImpl(std::ostream& os) const = 0;

public:
    explicit Livrator(std::string numeFirma);
    virtual ~Livrator();

    virtual std::unique_ptr<Livrator> clone() const = 0;

    const std::string& getFirma() const;
    double taxaLivrare(double valoareComanda) const;

    friend std::ostream& operator<<(std::ostream& os, const Livrator& l);
};

#endif
