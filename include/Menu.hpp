#ifndef MENU_HPP
#define MENU_HPP

// Główne menu programu - spina wszystkie trzy struktury w jedną całość.
// Udostępnia tryb interaktywny (dodawanie / usuwanie / szukanie / wyświetlanie)
// oraz uruchamianie pomiarów wydajności z zapisem wyników do pliku CSV.
class Menu {
public:
    // Uruchamia główną pętlę menu
    void run();

private:
    // Interaktywna obsługa wybranej struktury
    void runInteractive();

    // Pomiary wydajności wszystkich struktur -> zapis do wyniki.csv
    void runBenchmarks();
};

#endif // MENU_HPP
