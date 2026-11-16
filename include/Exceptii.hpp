#ifndef EXCEPTII_HPP
#define EXCEPTII_HPP

#include <exception>
#include <string>

// ierarhie proprie de exceptii, independenta de ierarhia Promotie
class EroareCafenea : public std::exception
{
private:
    std::string mesaj;
public:
    explicit EroareCafenea(std::string text);
    const char* what() const noexcept override;
};

// fisier care nu se poate deschide sau are continut/format gresit
class EroareFisier : public EroareCafenea
{
public:
    EroareFisier(const std::string& cale, const std::string& motiv);
};

// date invalide la construirea sau modificarea obiectelor
class EroareDateInvalide : public EroareCafenea
{
public:
    explicit EroareDateInvalide(const std::string& detalii);
};

// ingredient lipsa sau insuficient pentru o comanda
class EroareStocInsuficient : public EroareCafenea
{
public:
    explicit EroareStocInsuficient(const std::string& detalii);
};

// fonduri insuficiente la reaprovizionare
class EroareFonduriInsuficiente : public EroareCafenea
{
public:
    EroareFonduriInsuficiente(double necesar, double disponibil);
};

#endif
