# Sistem de Gestiune a unei Cafenele

Program C++ care simulează gestiunea unei cafenele: stocuri, rețete, comenzi, clienți cu fidelitate, promoții, livrări și rapoarte financiare.

## Componentele sistemului
- **Stoc de ingrediente:** Urmărește resursele disponibile (cafea, lapte, sirop etc.), cantitățile curente, unitățile de măsură și costurile unitare de achiziție.
- **Catalog de rețete:** Definește băuturile disponibile, ingredientele necesare preparării și prețul de vânzare.
- **Balanță financiară:** Actualizată automat (crește la vânzarea produselor și scade la reaprovizionare și la plata taxelor de livrare).
- **Clienți și fidelitate:** Evidență a comenzilor per client, calcul al sumelor cheltuite, puncte de fidelitate și reducere în funcție de nivel.
- **Promoții:** Ierarhie polimorfică (`Promotie` abstractă) cu mai multe tipuri: reducere procentuală, N+1 gratis, reducere la prag de cantitate și reducere fixă. La fiecare comandă se aplică cea mai avantajoasă promoție activă.
- **Livrări:** Barista înregistrează ce trebuie să preia livratorul, firma de livrare și taxa ei. Ierarhie polimorfică (`Livrator` abstractă): comision procentual (`LivratorProcent`) sau taxă fixă (`LivratorFix`).
- **Istoric și rapoarte:** Păstrarea comenzilor finalizate (inclusiv a celor de livrare), top produse, raport de rentabilitate, raport financiar și salvarea datelor în fișiere.
- **Excepții proprii:** Ierarhie derivată din `EroareCafenea` (date invalide, fișier, stoc insuficient, fonduri insuficiente), tratată în meniu fără ca aplicația să se oprească.

---

## Meniu și funcționalități
1. **Stare cafenea:** Afișează stocurile curente, soldul financiar și catalogul de rețete.
2. **Comandă din catalog:** Alegi o băutură din lista rețetelor; se actualizează stocurile și balanța.
3. **Verificare disponibilitate:** Verifică dacă există suficiente ingrediente pentru o băutură.
4. **Raport de rentabilitate:** Cost de producție, preț de vânzare, profit net și marjă pentru fiecare rețetă, plus taxele de livrare plătite.
5. **Reaprovizionare ingredient:** Adaugă stoc, scăzând costul din fondurile cafenelei.
6. **Comandă client (multi-produs):** Comandă cu mai multe produse, asociată unui client; se aplică promoțiile și reducerea de fidelitate.
7. **Clienți și niveluri de fidelitate:** Lista clienților, sumele cheltuite și punctele acumulate.
8. **Istoric vânzări:** Comenzile procesate, totaluri, reduceri acordate, valoare medie și top produse.
9. **Salvare raport de vânzări:** Exportă istoricul într-un fișier text.
10. **Gestionare promoții:** Afișare, adăugare, activare și dezactivare.
11. **Raport financiar:** Încasări, reduceri, cost materie primă, cheltuieli de reaprovizionare, taxe de livrare, profit brut, flux de numerar și marjă.
12. **Salvare raport financiar:** Exportă raportul financiar într-un fișier.
13. **Salvare stoc curent:** Scrie stocul în același format în care este citit la pornire.
14. **Simulare comandă:** Rulează o comandă pe o copie a cafenelei, fără efect asupra datelor reale.
15. **Înregistrare livrare:** Produsele de preluat, firma de livrare și tipul taxei (comision procentual sau taxă fixă). Comanda apare în istoric, iar taxa se scade din balanță și intră în raportul financiar.
16. **Afișare livrări:** Lista livrărilor înregistrate, cu firma și suma de plată.
0. **Ieșire:** Încheie execuția programului.


## Resurse

- [spdlog v1.14.1](https://github.com/gabime/spdlog/releases/tag/v1.14.1) - logging
- [nlohmann/json v3.11.3](https://github.com/nlohmann/json/releases/tag/v3.11.3) - citire JSON