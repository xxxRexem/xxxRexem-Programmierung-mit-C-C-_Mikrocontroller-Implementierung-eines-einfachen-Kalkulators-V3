#include <iostream>
#include <string>

// Lagert die Anzeige des Menüs in eine eigene Funktion aus, um den main-Code lesbarer zu machen.
void displayMenu() {
    std::cout << "\n--- Hauptmenue ---\n";
    std::cout << "Bitte waehlen Sie die gewuenschte Rechenoperation aus:\n";
    std::cout << "1. Addition\n";
    std::cout << "2. Subtraktion\n";
    std::cout << "3. Multiplikation\n";
    std::cout << "4. Division\n";
    std::cout << "5. Programm beenden\n";
    std::cout << "Ihre Wahl: ";
}

// Die main-Funktion ist der Haupteinstiegspunkt des Programms. Die Ausführung beginnt hier.
int main()
{
    std::cout << "======================================\n";
    std::cout << "   Mikrocontroller-Kalkulator-Projekt   \n";
    std::cout << "======================================\n";

    // Startet die Hauptschleife des Programms, die sich wiederholt, bis der Benutzer das Beenden wählt.
    while (true)
    {
        displayMenu(); // Greift auf die oben definierte displayMenu Funktion zurück und ruft diese auf

        int choice;
        std::cin >> choice; // Liest die numerische Auswahl des Benutzers ein.

        // Die switch-Anweisung überprueft den Wert der Variable 'choice'.
        switch (choice)
        {
        case 1:
            std::cout << "-> Sie haben Addition gewaehlt.\n";
            break;
        case 2:
            std::cout << "-> Sie haben Subtraktion gewaehlt.\n";
            break;
        case 3:
            std::cout << "-> Sie haben Multiplikation gewaehlt.\n";
            break;
        case 4:
            std::cout << "-> Sie haben Division gewaehlt.\n";
            break;
        case 5:
            std::cout << "Programm wird beendet.\n";
            return 0;
        default:
            std::cout << "FEHLER: Ungueltige Auswahl. Bitte versuchen Sie es erneut.\n";
            break;
        }
    }
}