#include "Cafenea.hpp"
#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>
#include <utility>
#include <spdlog/spdlog.h>
#include "Exceptii.hpp"
#include "PromotieProcent.hpp"

Cafenea::Cafenea(const std::string& numeCafenea, double fondInitial)
    : nume(numeCafenea), balansf(fondInitial), istoric(numeCafenea), urmatorulIdComanda(1),
      cheltuieliReaprovizionare(0.0)
{
    if (nume.empty())
        throw EroareDateInvalide("Cafeneaua trebuie sa aiba un nume.");
    if (fondInitial < 0.0)
        throw EroareDateInvalide("Fondul initial nu poate fi negativ.");
}

Cafenea::Cafenea(const Cafenea& alta)
    : nume(alta.nume), balansf(alta.balansf), stocIngrediente(alta.stocIngrediente),
      catalogR(alta.catalogR), istoric(alta.istoric), urmatorulIdComanda(alta.urmatorulIdComanda),
      cheltuieliReaprovizionare(alta.cheltuieliReaprovizionare)
{
    promotii.reserve(alta.promotii.size());
    for (const auto& p : alta.promotii)
        promotii.push_back(p->clone());
}

void swap(Cafenea& a, Cafenea& b) noexcept
{
    using std::swap;
    swap(a.nume, b.nume);
    swap(a.balansf, b.balansf);
    swap(a.stocIngrediente, b.stocIngrediente);
    swap(a.catalogR, b.catalogR);
    swap(a.istoric, b.istoric);
    swap(a.urmatorulIdComanda, b.urmatorulIdComanda);
    swap(a.promotii, b.promotii);
    swap(a.cheltuieliReaprovizionare, b.cheltuieliReaprovizionare);
}

Cafenea& Cafenea::operator=(Cafenea alta)
{
    swap(*this, alta);
    return *this;
}

int Cafenea::gasesteIndexIngredient(const std::string& denumire) const
{
    auto it = std::find_if(stocIngrediente.begin(), stocIngrediente.end(),
        [&denumire](const Ingrediente& ing) { return ing.getdenumire() == denumire; });
    return it == stocIngrediente.end() ? -1 : static_cast<int>(it - stocIngrediente.begin());
}

int Cafenea::gasesteIndexReteta(const std::string& denumire) const
{
    auto it = std::find_if(catalogR.begin(), catalogR.end(),
        [&denumire](const Reteta& r) { return r.getDenumire() == denumire; });
    return it == catalogR.end() ? -1 : static_cast<int>(it - catalogR.begin());
}

int Cafenea::gasesteIndexPromotieNume(const std::string& numePromotie) const
{
    auto it = std::find_if(promotii.begin(), promotii.end(),
        [&numePromotie](const std::unique_ptr<Promotie>& p) { return p->getNume() == numePromotie; });
    return it == promotii.end() ? -1 : static_cast<int>(it - promotii.begin());
}

// apelul virtual: fiecare promotie isi calculeaza singura totalul
int Cafenea::gasesteIndexPromotie(const std::string& produs, double pretUnitar, int cantitate) const
{
    int best = -1;
    double totalBest = 0.0;
    for (std::size_t i = 0; i < promotii.size(); ++i)
    {
        if (!promotii[i]->aplicabila(produs))
            continue;
        double total = promotii[i]->totalDupaPromotie(pretUnitar, cantitate);
        if (best == -1 || total < totalBest)
        {
            best = static_cast<int>(i);
            totalBest = total;
        }
    }
    return best;
}

bool Cafenea::stocAcopera(const std::vector<CReteta>& necesar) const
{
    for (const CReteta& comp : necesar)
    {
        int idx = gasesteIndexIngredient(comp.getnume());
        if (idx == -1 || stocIngrediente[static_cast<std::size_t>(idx)].getcantitateDisponibila() < comp.getcantitate())
            return false;
    }
    return true;
}

void Cafenea::adaugaIngredient(const Ingrediente& ing)
{
    stocIngrediente.push_back(ing);
}

void Cafenea::adaugaReteta(const Reteta& ret)
{
    if (gasesteIndexReteta(ret.getDenumire()) != -1)
        throw EroareDateInvalide("Exista deja o reteta cu numele: " + ret.getDenumire());
    catalogR.push_back(ret);
}

void Cafenea::adaugaPromotie(std::unique_ptr<Promotie> promo)
{
    if (!promo)
        throw EroareDateInvalide("Promotie lipsa.");
    if (gasesteIndexReteta(promo->getProdusVizat()) == -1)
        throw EroareDateInvalide("Promotia vizeaza un produs inexistent: " + promo->getProdusVizat());
    if (gasesteIndexPromotieNume(promo->getNume()) != -1)
        throw EroareDateInvalide("Exista deja o promotie cu numele: " + promo->getNume());
    promotii.push_back(std::move(promo));
}

bool Cafenea::seteazaStarePromotie(const std::string& numePromotie, bool activa)
{
    int idx = gasesteIndexPromotieNume(numePromotie);
    if (idx == -1)
        return false;
    Promotie& promo = *promotii[static_cast<std::size_t>(idx)];
    if (activa)
        promo.activeaza();
    else
        promo.dezactiveaza();
    return true;
}

int Cafenea::incarcaIngredienteDinFisier(const std::string& cale)
{
    std::ifstream fisier(cale);
    if (!fisier.is_open())
        throw EroareFisier(cale, "nu pot deschide fisierul");

    int incarcate = 0;
    std::string linie;
    while (std::getline(fisier, linie))
    {
        if (!linie.empty() && linie.back() == '\r')
            linie.pop_back();
        if (linie.empty() || linie[0] == '#')
            continue;

        std::stringstream ss(linie);
        std::string den, stocText, unitate, pretText;
        if (!(std::getline(ss, den, ';') && std::getline(ss, stocText, ';') &&
              std::getline(ss, unitate, ';') && std::getline(ss, pretText, ';')))
        {
            spdlog::warn("Linie invalida ignorata: {}", linie);
            continue;
        }

        try
        {
            double stoc = std::stod(stocText);
            double pret = std::stod(pretText);
            if (den.empty() || stoc < 0.0 || pret < 0.0)
                throw EroareDateInvalide("valori negative sau denumire goala");
            adaugaIngredient(Ingrediente(den, stoc, unitate, pret));
            ++incarcate;
        }
        catch (const std::exception&)
        {
            spdlog::warn("Valori invalide, linie ignorata: {}", linie);
        }
    }
    if (incarcate == 0)
        throw EroareFisier(cale, "nu contine niciun ingredient valid");
    return incarcate;
}

std::vector<std::string> Cafenea::numeBauturi() const
{
    std::vector<std::string> nume_bauturi;
    for (const Reteta& r : catalogR)
        nume_bauturi.push_back(r.getDenumire());
    return nume_bauturi;
}

bool Cafenea::poatePrepara(const std::string& numeBautura) const
{
    int ir = gasesteIndexReteta(numeBautura);
    if (ir == -1)
        return false;
    std::vector<CReteta> necesar;
    catalogR[static_cast<std::size_t>(ir)].adaugaLaNecesar(necesar);
    return stocAcopera(necesar);
}

// comanda simpla: un singur produs, client anonim
void Cafenea::proceseazaComanda(const std::string& numeBautura)
{
    Client anonim("Client anonim");
    plaseazaComanda(anonim, std::vector<std::string>(1, numeBautura));
}

// 1) aduna necesarul total, 2) verifica stocul pentru tot, 3) scade ingredientele,
// 4) pretul fiecarui produs, dupa cea mai buna promotie pentru cantitatea comandata,
// 5) reducerea de fidelitate, 6) incaseaza si inregistreaza
void Cafenea::plaseazaComanda(Client& client, const std::vector<std::string>& produse)
{
    if (produse.empty())
        throw EroareDateInvalide("Comanda nu contine niciun produs.");

    std::vector<CReteta> necesar;
    std::map<std::string, int> cantitati;
    for (const std::string& produs : produse)
    {
        int ir = gasesteIndexReteta(produs);
        if (ir == -1)
            throw EroareDateInvalide("Produs necunoscut: '" + produs + "'.");
        catalogR[static_cast<std::size_t>(ir)].adaugaLaNecesar(necesar);
        ++cantitati[produs];
    }

    if (!stocAcopera(necesar))
        throw EroareStocInsuficient("nu ajunge stocul pentru intreaga comanda.");

    for (const CReteta& comp : necesar)
        stocIngrediente[static_cast<std::size_t>(gasesteIndexIngredient(comp.getnume()))].consuma(comp.getcantitate());

    Comanda cmd(urmatorulIdComanda, client.getNume());
    for (const auto& linie : cantitati)
    {
        const std::string& produs = linie.first;
        int cantitate = linie.second;
        double pret = catalogR[static_cast<std::size_t>(gasesteIndexReteta(produs))].getPretVanzare();
        double total = pret * cantitate;

        int ip = gasesteIndexPromotie(produs, pret, cantitate);
        if (ip != -1)
        {
            const Promotie& promo = *promotii[static_cast<std::size_t>(ip)];
            double totalPromo = promo.totalDupaPromotie(pret, cantitate);
            if (totalPromo < total)
            {
                spdlog::info("[PROMO] {}: {} x{} {:.2f} -> {:.2f} Lei leuti", promo.getNume(), produs,
                             cantitate, total, totalPromo);
                total = totalPromo;
            }
        }
        cmd.adaugaLinie(produs, total / cantitate, cantitate);
    }

    cmd.aplicaReducere(client.getReducere());   // reducerea nivelului de fidelitate
    balansf += cmd.getTotal();
    client.inregistreazaComanda(cmd.getTotal());
    istoric.adaugaComanda(cmd);
    ++urmatorulIdComanda;

    std::cout << "[COMANDA FINALIZATA]\n" << cmd << "\n";
}

void Cafenea::reaprovizioneaza(const std::string& denumireIngredient, double cantitate)
{
    int idx = gasesteIndexIngredient(denumireIngredient);
    if (idx == -1 || cantitate <= 0.0)
        throw EroareDateInvalide("ingredient necunoscut sau cantitate invalida.");

    Ingrediente& ing = stocIngrediente[static_cast<std::size_t>(idx)];
    double cost = cantitate * ing.getpretUnitate();
    if (cost > balansf)
        throw EroareFonduriInsuficiente(cost, balansf);

    balansf -= cost;
    cheltuieliReaprovizionare += cost;
    ing.adaugaStoc(cantitate);
    spdlog::info("[REAPROVIZIONARE] +{} {} {} | Cost: {:.2f} Lei leuti", cantitate,
                 ing.getunitateMasura(), denumireIngredient, cost);
}

void Cafenea::afiseazaRaportProfitabilitate() const
{
    std::cout << "\n---------- RAPORT RENTABILITATE CATALOG ----------\n";
    for (const Reteta& r : catalogR)
    {
        double cost = r.calculeazaCostProductie(stocIngrediente);
        double pret = r.getPretVanzare();
        double profitNet = pret - cost;
        double marja = (pret > 0.0) ? (profitNet / pret) * 100.0 : 0.0;

        std::cout << "- " << r.getDenumire()
                  << " | Cost Productie: " << cost << " Lei leuti"
                  << " | Pret Vanzare: " << pret << " Lei leuti"
                  << " | Profit Net: " << profitNet << " Lei leuti"
                  << " (" << marja << "% marja)\n";
    }
    std::cout << "-----------------------------------\n";
}

void Cafenea::afiseazaIstoric() const
{
    std::cout << "\n" << istoric << "\n";
}

bool Cafenea::salveazaRaportVanzari(const std::string& cale) const
{
    return istoric.salveazaInFisier(cale);
}

const IstoricVanzari& Cafenea::getIstoric() const
{
    return istoric;
}

std::ostream& operator<<(std::ostream& os, const Cafenea& c)
{
    os << "-----------------------------------\n"
       << "Cafenea: " << c.nume << " | Balanta: " << c.balansf << " Lei leuti"
       << " | Comenzi finalizate: " << c.istoric.getNumarComenzi() << "\n"
       << "Stocuri Curente:\n";
    for (const Ingrediente& ing : c.stocIngrediente)
        os << "  * " << ing << "\n";
    os << "Catalog:\n";
    for (const Reteta& r : c.catalogR)
        os << "  - " << r << "\n";
    os << "-----------------------------------";
    return os;
}

void Cafenea::afiseazaPromotii() const
{
    std::cout << "\n---------- PROMOTII ----------\n";
    if (promotii.empty())
        std::cout << "Nu exista promotii definite.\n";

    int procentuale = 0;
    double reducereMaxima = 0.0;
    for (const auto& p : promotii)
    {
        std::cout << " * " << *p << "\n";
        // downcast cu sens: doar PromotieProcent are un procent de comparat
        if (const PromotieProcent* pp = dynamic_cast<const PromotieProcent*>(p.get()))
        {
            ++procentuale;
            reducereMaxima = std::max(reducereMaxima, pp->getProcent());
        }
    }
    if (procentuale > 0)
        std::cout << "Promotii procentuale: " << procentuale << " (cea mai mare: " << reducereMaxima << "%)\n";
    std::cout << "Instante de promotii in memorie: " << Promotie::getNumarInstante() << "\n";
    std::cout << "------------------------------\n";
}

// cost materie prima pentru toate bauturile vandute pana acum
double Cafenea::costMateriePrimaVanduta() const
{
    double total = 0.0;
    for (const auto& vandut : istoric.topProduse(catalogR.size()))
    {
        int ir = gasesteIndexReteta(vandut.first);
        if (ir == -1)
            continue;
        total += vandut.second * catalogR[static_cast<std::size_t>(ir)].calculeazaCostProductie(stocIngrediente);
    }
    return total;
}

RaportFinanciar Cafenea::raportFinanciar() const
{
    return RaportFinanciar(istoric.totalIncasari(), istoric.totalReduceri(),
                           costMateriePrimaVanduta(), cheltuieliReaprovizionare,
                           istoric.getNumarComenzi());
}

void Cafenea::afiseazaRaportFinanciar() const
{
    std::cout << "\n" << raportFinanciar() << "\n";
}

// scrie stocul in acelasi format pe care il citeste incarcaIngredienteDinFisier
bool Cafenea::salveazaStocInFisier(const std::string& cale) const
{
    std::ofstream fisier(cale);
    if (!fisier.is_open())
        return false;

    fisier << "# denumire;stoc;unitate;pret_unitar\n";
    for (const Ingrediente& ing : stocIngrediente)
    {
        fisier << ing.getdenumire() << ";" << ing.getcantitateDisponibila() << ";"
               << ing.getunitateMasura() << ";" << ing.getpretUnitate() << "\n";
    }
    return fisier.good();
}
