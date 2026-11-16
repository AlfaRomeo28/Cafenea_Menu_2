#include <cassert>
#include <cmath>
#include <memory>
#include <vector>
#include "CReteta.hpp"
#include "Exceptii.hpp"
#include "PromotieNPlusUnu.hpp"
#include "PromotieProcent.hpp"
#include "Reteta.hpp"

namespace
{
    bool egal(double a, double b) { return std::abs(a - b) < 1e-9; }
}

int main()
{
    // promotie procentuala activa: 2 bucati la 10 lei, -20%
    PromotieProcent procent("Happy Hour", "Espresso", 20.0, true);
    assert(egal(procent.totalDupaPromotie(10.0, 2), 16.0));

    // promotie oprita: pret intreg
    procent.dezactiveaza();
    assert(egal(procent.totalDupaPromotie(10.0, 2), 20.0));


    PromotieNPlusUnu nPlusUnu("2+1", "Cappuccino", 2, true);
    assert(egal(nPlusUnu.totalDupaPromotie(10.0, 3), 20.0));

    
    std::unique_ptr<Promotie> copie = nPlusUnu.clone();
    copie->dezactiveaza();
    assert(egal(copie->totalDupaPromotie(10.0, 3), 30.0));
    assert(egal(nPlusUnu.totalDupaPromotie(10.0, 3), 20.0));

    // reteta fara ingrediente arunca EroareDateInvalide
    bool aruncat = false;
    try
    {
        Reteta invalida("Gol", 5.0, std::vector<CReteta>());
    }
    catch (const EroareDateInvalide&)
    {
        aruncat = true;
    }
    assert(aruncat);

    return 0;
}