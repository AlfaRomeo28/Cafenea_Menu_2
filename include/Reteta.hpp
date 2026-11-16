#ifndef RETETA_H
#define RETETA_H

#include <iostream>
#include <string>
#include <vector>
#include "CReteta.hpp"
#include "Ingrediente.hpp"

// reteta unei bauturi; componentele sunt tinute intr-un std::vector (regula lui 0)
class Reteta
{
private:
    std::string denBaut;
    double pretV;
    std::vector<CReteta> componente;
public:
    Reteta(std::string denumire, double pret, std::vector<CReteta> comp);

    const std::string& getDenumire() const;
    double getPretVanzare() const;

    // adauga componentele retetei la necesarul total (cumuleaza cantitatile aceluiasi ingredient)
    void adaugaLaNecesar(std::vector<CReteta>& necesar) const;
    double calculeazaCostProductie(const std::vector<Ingrediente>& stoc) const;

    friend std::ostream& operator<<(std::ostream& os, const Reteta& ret);
};

#endif
