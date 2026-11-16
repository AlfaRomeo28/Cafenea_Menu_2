#ifndef CAFENEA_HPP
#define CAFENEA_HPP

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Client.hpp"
#include "Comanda.hpp"
#include "CReteta.hpp"
#include "IstoricVanzari.hpp"
#include "Ingrediente.hpp"
#include "Promotie.hpp"
#include "RaportFinanciar.hpp"
#include "Reteta.hpp"

class Cafenea
{
private:
    std::string nume;
    double balansf;
    std::vector<Ingrediente> stocIngrediente;
    std::vector<Reteta> catalogR;
    IstoricVanzari istoric;
    int urmatorulIdComanda;
    std::vector<std::unique_ptr<Promotie> > promotii;   // pointeri la baza: apeluri virtuale
    double cheltuieliReaprovizionare;

    int gasesteIndexIngredient(const std::string& denumire) const;
    int gasesteIndexReteta(const std::string& denumire) const;
    int gasesteIndexPromotieNume(const std::string& numePromotie) const;
    // promotia activa cu cel mai mic total pentru cantitatea data, sau -1
    int gasesteIndexPromotie(const std::string& produs, double pretUnitar, int cantitate) const;
    bool stocAcopera(const std::vector<CReteta>& necesar) const;
    double costMateriePrimaVanduta() const;
public:
    Cafenea(const std::string& numeCafenea, double fondInitial);
    Cafenea(const Cafenea& alta);                 // copiaza promotiile prin clone()
    Cafenea(Cafenea&& alta) noexcept = default;
    Cafenea& operator=(Cafenea alta);             // copy and swap
    ~Cafenea() = default;

    void adaugaIngredient(const Ingrediente& ing);
    void adaugaReteta(const Reteta& ret);
    void adaugaPromotie(std::unique_ptr<Promotie> promo);
    bool seteazaStarePromotie(const std::string& numePromotie, bool activa);

    // arunca EroareFisier daca fisierul lipseste sau nu contine niciun ingredient valid
    int incarcaIngredienteDinFisier(const std::string& cale);

    std::vector<std::string> numeBauturi() const;
    bool poatePrepara(const std::string& numeBautura) const;
    void proceseazaComanda(const std::string& numeBautura);
    void plaseazaComanda(Client& client, const std::vector<std::string>& produse);
    void reaprovizioneaza(const std::string& denumireIngredient, double cantitate);

    void afiseazaRaportProfitabilitate() const;
    void afiseazaIstoric() const;
    void afiseazaPromotii() const;
    void afiseazaRaportFinanciar() const;
    RaportFinanciar raportFinanciar() const;
    bool salveazaRaportVanzari(const std::string& cale) const;
    bool salveazaStocInFisier(const std::string& cale) const;
    

    friend void swap(Cafenea& a, Cafenea& b) noexcept;
    friend std::ostream& operator<<(std::ostream& os, const Cafenea& c);
};

#endif
