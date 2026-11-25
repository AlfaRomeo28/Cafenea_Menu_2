#ifndef LIVRARE_HPP
#define LIVRARE_HPP

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Livrator.hpp"

// ce trebuie sa ia livratorul, de la ce firma si cat se plateste
class Livrare
{
private:
    std::vector<std::string> produse;
    std::unique_ptr<Livrator> livrator;
    double valoareComanda;
    double taxa;

public:
    Livrare(std::vector<std::string> produseLivrate, const Livrator& firma, double valoare);
    Livrare(const Livrare& alta);
    Livrare& operator=(Livrare alta);
    friend void swap(Livrare& a, Livrare& b) noexcept;

    const std::string& getFirma() const;
    double getTaxa() const;

    friend std::ostream& operator<<(std::ostream& os, const Livrare& l);
};

#endif
