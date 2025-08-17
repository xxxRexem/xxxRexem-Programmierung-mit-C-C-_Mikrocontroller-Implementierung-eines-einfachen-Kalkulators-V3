#include <iostream>
#include <string>
#include <limits>

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

// Fordert den Benutzer auf, die Operanden einzugeben und bereitet die Daten für den Versand vor (simuliert).
// Lagert die Auswahl der verschieden Operationen in eine eigene Funktion aus, um den main-code lesbarer zu machen.
void handleOperation(char op) {
    float operand1, operand2; // Variablen für die beiden Gleitkommazahlen, die der Benutzer eingibt.
    std::string operand1_name, operand2_name; // Variablen für die textuellen Bezeichnungen z.B. 'Summand'.

    // Legt die passenden Namen für die Operanden fest, je nach gewählter Operation.
    switch (op) {
    case '+': operand1_name = "Summand 1"; operand2_name = "Summand 2"; break;
    case '-': operand1_name = "Minuend"; operand2_name = "Subtrahend"; break;
    case '*': operand1_name = "Faktor 1"; operand2_name = "Faktor 2"; break;
    case '/': operand1_name = "Dividend"; operand2_name = "Divisor"; break;
    }

    std::cout << "Bitte geben Sie den Wert fuer " << operand1_name << " ein: "; // Fordert den Benutzer zur Eingabe des ersten Wertes auf.
    // Liest die Benutzereingabe von der Konsole und versucht, sie in die float-Variable 'operand1' zu speichern.
    std::cin >> operand1;

    // Fordert den Benutzer zur Eingabe des zweiten Wertes auf.
    std::cout << "Bitte geben Sie den Wert fuer " << operand2_name << " ein: ";
    // Liest die Benutzereingabe von der Konsole und versucht, sie in die float-Variable 'operand2' zu speichern.
    std::cin >> operand2;

    // Baut den finalen String zusammen, der an den Arduino gesendet wird.
    std::string expression = std::to_string(operand1) + " " + op + " " + std::to_string(operand2);

    // Simulierte Kommunikation 
    std::cout << "\nAn Arduino gesendet (simuliert): " << expression << std::endl;
    std::string mockResult = "Ergebnis: 25.0 (simuliert)";

    // Gibt das (aktuell simulierte) Ergebnis vom Arduino auf der Konsole aus.
    std::cout << mockResult << std::endl;
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

        // Prüft ob die Eingabe fehlerhaft war 
        if (std::cin.fail()) {
            std::cout << "FEHLER: Bitte geben Sie nur eine Zahl zwischen 1 und 5 ein.\n";
            std::cin.clear(); // Setzt die internen Fehler von cin zurück, damit es wieder nutzbar ist.
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Leert den restlichen Eingabepuffer.
            continue; // Springt zum Anfang der while-Schleife, um das Menü erneut anzuzeigen.
        }

        // Leert den Eingabepuffer auch bei gültiger Eingabe, um zu verhindern,
        // dass das verbleibende Enter-Zeichen ('\n') zukünftige Eingaben stört.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


        // Die switch-Anweisung überprueft den Wert der Variable 'choice'.
        switch (choice)
        {
        case 1: handleOperation('+'); break;
        case 2: handleOperation('-'); break;
        case 3: handleOperation('*'); break;
        case 4: handleOperation('/'); break;
        case 5:
            std::cout << "Programm wird beendet.\n";
            return 0;
        default:
            std::cout << "FEHLER: Ungueltige Auswahl. Bitte versuchen Sie es erneut.\n";
            break;
        }
    }
}