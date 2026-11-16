#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "Cafenea.hpp"
#include "Exceptii.hpp"
#include "IncarcatorDate.hpp"
#include "PromotieNPlusUnu.hpp"
#include "PromotiePragCantitate.hpp"
#include "PromotieProcent.hpp"

namespace
{
    const char* const FISIER_INGREDIENTE = "assets/ingrediente.txt";
    const char* const FISIER_RETETE = "assets/retete.json";
    const char* const FISIER_PROMOTII = "assets/promotii.json";

    void curataLinie()
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    bool continua()
    {
        char op;
        std::cout << "Doriti sa continuati? (Y/N): ";
        std::cin >> op;
        if (op == 'Y' || op == 'y')
            return true;
        if (op != 'N' && op != 'n')
            std::cout << "Eroare!\n";
        return false;
    }

    // citeste un numar; la input gresit arunca exceptie (si goleste intrarea), nu strica cin
    template <typename T>
    T citesteNumar(const std::string& mesaj)
    {
        std::cout << mesaj;
        T valoare{};
        if (!(std::cin >> valoare))
        {
            std::cin.clear();
            curataLinie();
            throw EroareDateInvalide("se astepta un numar.");
        }
        curataLinie();
        return valoare;
    }

    std::string taie(const std::string& text)
    {
        std::size_t inc = text.find_first_not_of(" \t\r");
        if (inc == std::string::npos)
            return "";
        std::size_t sf = text.find_last_not_of(" \t\r");
        return text.substr(inc, sf - inc + 1);
    }

    std::string citesteText(const std::string& mesaj)
    {
        std::cout << mesaj;
        std::string text;
        std::getline(std::cin, text);
        return taie(text);
    }

    // "Espresso; Cappuccino" -> {"Espresso", "Cappuccino"}
    std::vector<std::string> imparteProduse(const std::string& linie)
    {
        std::vector<std::string> produse;
        std::stringstream ss(linie);
        std::string bucata;
        while (std::getline(ss, bucata, ';'))
        {
            bucata = taie(bucata);
            if (!bucata.empty())
                produse.push_back(bucata);
        }
        return produse;
    }

    // cauta clientul dupa nume; daca nu exista, il creeaza
    Client& gasesteSauCreeazaClient(std::vector<Client>& clienti, const std::string& numeClient)
    {
        for (Client& c : clienti)
            if (c.getNume() == numeClient)
                return c;
        clienti.emplace_back(numeClient);
        return clienti.back();
    }

    // datele vin din fisiere; daca lipsesc, aplicatia nu porneste (EroareFisier in main)
    void incarcaDate(Cafenea& cafenea)
    {
        int ingrediente = cafenea.incarcaIngredienteDinFisier(FISIER_INGREDIENTE);
        std::cout << "[INFO] Am incarcat " << ingrediente << " ingrediente din " << FISIER_INGREDIENTE << "\n";

        for (const Reteta& r : IncarcatorDate::incarcaRetete(FISIER_RETETE))
            cafenea.adaugaReteta(r);

        for (std::unique_ptr<Promotie>& p : IncarcatorDate::incarcaPromotii(FISIER_PROMOTII))
            cafenea.adaugaPromotie(std::move(p));
    }

    void comandaDinCatalog(Cafenea& cafenea)
    {
        std::vector<std::string> bauturi = cafenea.numeBauturi();
        for (std::size_t i = 0; i < bauturi.size(); ++i)
            std::cout << "  " << (i + 1) << ". " << bauturi[i] << "\n";
        int alegere = citesteNumar<int>("Alege bautura: ");
        if (alegere < 1 || static_cast<std::size_t>(alegere) > bauturi.size())
            throw EroareDateInvalide("bautura inexistenta in catalog.");
        cafenea.proceseazaComanda(bauturi[static_cast<std::size_t>(alegere - 1)]);
    }

    std::unique_ptr<Promotie> citestePromotieNoua()
    {
        std::cout << "  Tip: 1. Procent  2. N+1 gratis  3. Reducere la prag de cantitate\n";
        int tip = citesteNumar<int>("  Alege tipul: ");
        std::string numePromo = citesteText("Numele promotiei: ");
        std::string produs = citesteText("Produsul vizat: ");

        if (tip == 1)
            return std::make_unique<PromotieProcent>(numePromo, produs, citesteNumar<double>("Reducere (%): "));
        if (tip == 2)
            return std::make_unique<PromotieNPlusUnu>(numePromo, produs, citesteNumar<int>("Cumperi cate bucati: "));
        if (tip == 3)
        {
            int prag = citesteNumar<int>("Prag (bucati): ");
            return std::make_unique<PromotiePragCantitate>(numePromo, produs, prag,
                                                           citesteNumar<double>("Reducere (%): "));
        }
        throw EroareDateInvalide("tip de promotie necunoscut.");
    }

    // submeniu pentru promotii: afisare, adaugare, activare, dezactivare
    void meniuPromotii(Cafenea& cafenea)
    {
        std::cout << "  1. Afiseaza promotiile\n"
                  << "  2. Adauga o promotie\n"
                  << "  3. Activeaza o promotie\n"
                  << "  4. Dezactiveaza o promotie\n";
        int sub = citesteNumar<int>("  Alege: ");

        if (sub == 1)
        {
            cafenea.afiseazaPromotii();
        }
        else if (sub == 2)
        {
            cafenea.adaugaPromotie(citestePromotieNoua());
            std::cout << "[INFO] Promotie adaugata. Starea ei implicita este oprita; porneste-o din optiunea 3.\n";
        }
        else if (sub == 3 || sub == 4)
        {
            std::string numePromo = citesteText("Numele promotiei: ");
            if (cafenea.seteazaStarePromotie(numePromo, sub == 3))
                std::cout << "[INFO] Promotia a fost " << (sub == 3 ? "activata" : "dezactivata") << ".\n";
            else
                std::cout << "[EROARE] Nu exista nicio promotie cu numele '" << numePromo << "'.\n";
        }
        else
        {
            std::cout << "Optiune invalida pentru promotii.\n";
        }
    }

    void salveazaCuMesaj(bool reusit, const std::string& ce, const std::string& cale)
    {
        if (reusit)
            std::cout << "[INFO] " << ce << " salvat in '" << cale << "'.\n";
        else
            std::cout << "[EROARE] Nu am putut scrie fisierul '" << cale << "'.\n";
    }

    void afiseazaMeniu()
    {
        std::cout << "\n========================================\n"
                  << "        MENIU GESTIUNE CAFENEA         \n"
                  << "========================================\n"
                  << " 1. Afiseaza starea cafenelei (stocuri, sold, catalog)\n"
                  << " 2. Comanda o bautura din catalog\n"
                  << " 3. Verifica disponibilitate bautura\n"
                  << " 4. Afiseaza raportul de rentabilitate si profit\n"
                  << " 5. Reaproviziseaza un ingredient\n"
                  << " 6. Comanda pentru un client (mai multe produse)\n"
                  << " 7. Afiseaza clientii si nivelurile de fidelitate\n"
                  << " 8. Afiseaza istoricul vanzarilor\n"
                  << " 9. Salveaza raportul de vanzari intr-un fisier\n"
                  << "10. Gestioneaza promotiile\n"
                  << "11. Afiseaza raportul financiar\n"
                  << "12. Salveaza raportul financiar intr-un fisier\n"
                  << "13. Salveaza stocul curent intr-un fisier\n"
                  << "14. Simuleaza o comanda pe o copie a cafenelei (fara efect real)\n"
                  << " 0. Iesire\n"
                  << "Alege o optiune: ";
    }

    // executa o optiune din meniu; erorile se propaga catre main
    void executaOptiune(int optiune, Cafenea& cafenea, std::vector<Client>& clienti)
    {
        switch (optiune)
        {
        case 1:
            std::cout << "\n" << cafenea << "\n";
            break;
        case 2:
            comandaDinCatalog(cafenea);
            break;
        case 3:
        {
            std::string numeProdus = citesteText("Introdu numele exact al bauturii de verificat: ");
            if (cafenea.poatePrepara(numeProdus))
                std::cout << "[INFO] Exista stoc suficient pentru '" << numeProdus << "'.\n";
            else
                std::cout << "[ATENTIE] Nu se poate prepara '" << numeProdus
                          << "' (stoc insuficient sau produs inexistent).\n";
            break;
        }
        case 4:
            cafenea.afiseazaRaportProfitabilitate();
            break;
        case 5:
        {
            std::string ing = citesteText("Ingredient: ");
            cafenea.reaprovizioneaza(ing, citesteNumar<double>("Cantitate: "));
            break;
        }
        case 6:
        {
            std::string numeClient = citesteText("Numele clientului: ");
            std::string linieProduse = citesteText("Produse (separate prin ';'): ");
            Client& client = gasesteSauCreeazaClient(clienti, numeClient);
            cafenea.plaseazaComanda(client, imparteProduse(linieProduse));
            break;
        }
        case 7:
            if (clienti.empty())
                std::cout << "Nu exista clienti inregistrati.\n";
            for (const Client& c : clienti)
                std::cout << " * " << c << "\n";
            break;
        case 8:
            cafenea.afiseazaIstoric();
            break;
        case 9:
        {
            std::string cale = citesteText("Numele fisierului de iesire: ");
            salveazaCuMesaj(cafenea.salveazaRaportVanzari(cale), "Raport", cale);
            break;
        }
        case 10:
            meniuPromotii(cafenea);
            break;
        case 11:
            cafenea.afiseazaRaportFinanciar();
            break;
        case 12:
        {
            std::string cale = citesteText("Numele fisierului de iesire: ");
            salveazaCuMesaj(cafenea.raportFinanciar().salveazaInFisier(cale), "Raport financiar", cale);
            break;
        }
        case 13:
        {
            std::string cale = citesteText("Numele fisierului pentru stoc: ");
            salveazaCuMesaj(cafenea.salveazaStocInFisier(cale), "Stoc", cale);
            break;
        }
        case 14:
        {
            // lucreaza pe o copie (copy ctor + clone pentru promotii); cafeneaua reala ramane neschimbata
            Cafenea simulare(cafenea);
            std::string linieProduse = citesteText("Produse de simulat (separate prin ';'): ");
            Client simulat("Client simulat");
            simulare.plaseazaComanda(simulat, imparteProduse(linieProduse));
            std::cout << "[INFO] Simulare incheiata; stocul si sold-ul real nu s-au modificat.\n";
            break;
        }
        default:
            std::cout << "\nOptiune invalida! Incearca din nou.\n";
            break;
        }
    }
}

int main()
{
    try
    {
        Cafenea cafenea("Bobis_Bob_Caffe", 1000.0);
        incarcaDate(cafenea);

        std::vector<Client> clienti;
        int optiune = -1;
        while (optiune != 0)
        {
            afiseazaMeniu();
            if (!(std::cin >> optiune))
                break;
            curataLinie();
            if (optiune == 0)
            {
                std::cout << "\nInchidere aplicatie. La revedere!\n";
                break;
            }

            try
            {
                executaOptiune(optiune, cafenea, clienti);
            }
            catch (const EroareStocInsuficient& e)
            {
                std::cout << "[STOC] " << e.what() << "\n";
            }
            catch (const EroareFonduriInsuficiente& e)
            {
                std::cout << "[FONDURI] " << e.what() << "\n";
            }
            catch (const EroareFisier& e)
            {
                std::cout << "[FISIER] " << e.what() << "\n";
            }
            catch (const EroareCafenea& e)
            {
                std::cout << "[EROARE] " << e.what() << "\n";
            }

            if (!continua())
                break;
        }
    }
    catch (const EroareFisier& e)
    {
        std::cout << "[FISIER] " << e.what() << "\nAplicatia nu poate porni fara fisierele de date.\n";
        return 1;
    }
    catch (const std::exception& e)
    {
        std::cout << "[EXCEPTIE] " << e.what() << "\n";
        return 1;
    }
    return 0;
}
