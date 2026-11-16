#include "Exceptii.hpp"
#include <utility>

EroareCafenea::EroareCafenea(std::string text) : mesaj(std::move(text)) {}

const char* EroareCafenea::what() const noexcept { return mesaj.c_str(); }

EroareFisier::EroareFisier(const std::string& cale, const std::string& motiv)
    : EroareCafenea("Fisier '" + cale + "': " + motiv) {}

EroareDateInvalide::EroareDateInvalide(const std::string& detalii)
    : EroareCafenea("Date invalide: " + detalii) {}

EroareStocInsuficient::EroareStocInsuficient(const std::string& detalii)
    : EroareCafenea("Stoc insuficient: " + detalii) {}

EroareFonduriInsuficiente::EroareFonduriInsuficiente(double necesar, double disponibil)
    : EroareCafenea("Fonduri insuficiente (necesar: " + std::to_string(necesar) +
                    " Lei leuti, disponibil: " + std::to_string(disponibil) + " Lei leuti)") {}
