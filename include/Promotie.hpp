#ifndef PROMOTIE_HPP
#define PROMOTIE_HPP

#include <iostream>
#include <memory>
#include <string>

// Baza abstracta a ierarhiei de promotii: fiecare tip isi calculeaza altfel totalul.
// Interfata publica este non-virtuala (NVI); partea virtuala este protejata.
class Promotie
{
private:
    std::string nume;
    std::string produsVizat;
    bool activa;
    static int numarInstante;   // cate promotii exista acum in memorie

protected:
    Promotie(const Promotie& alta);
    Promotie& operator=(const Promotie&) = delete;

    // totalul pentru `cantitate` bucati, cand promotia e activa (specifica fiecarui tip)
    virtual double totalImpl(double pretUnitar, int cantitate) const = 0;
    virtual void afiseazaImpl(std::ostream& os) const = 0;

public:
    Promotie(std::string numePromo, std::string produs, bool esteActiva);
    virtual ~Promotie();

    virtual std::unique_ptr<Promotie> clone() const = 0;

    const std::string& getNume() const;
    const std::string& getProdusVizat() const;

    bool aplicabila(const std::string& produs) const;

    // total de plata pentru `cantitate` bucati (pret intreg daca promotia e oprita)
    double totalDupaPromotie(double pretUnitar, int cantitate) const;


    void activeaza();
    void dezactiveaza();

    static int getNumarInstante();

    friend std::ostream& operator<<(std::ostream& os, const Promotie& p);
};

#endif
