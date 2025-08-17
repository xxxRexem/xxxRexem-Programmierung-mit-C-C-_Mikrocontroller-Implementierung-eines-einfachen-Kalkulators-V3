#include <iostream>
#include <string>
#include <limits>

// Fragt den Benutzer in einer Schleife nach einer g�ltigen Gleitkommazahl.
// Die Schleife wird erst verlassen, wenn die Eingabe korrekt ist.
// Lagert die Fehlerbehandlung einer fehlerhaften Eingabe in eine eigene Funktion aus, um den main-Code lesbarer zu machen.
float getFloatFromUser(const std::string& prompt) {
    float number;
    while (true) // Schleife f�r eine robuste Eingabe
    {
        std::cout << prompt; // Zeigt den �bergebenen Aufforderungstext an
        std::cin >> number;

        if (std::cin.good()) {
            // Die Eingabe war erfolgreich, wenn die Eingabe eine Zahl ist.
            return number; // Die g�ltige Zahl zur�ckgeben und die Funktion beenden.
        }
        else {
            // Die Eingabe war fehlerhaft, weil die Eingabe ein Text war.
            std::cout << "FEHLER: Bitte geben Sie eine gueltige Zahl ein.\n";
            std::cin.clear(); // Fehlerzustand von cin zur�cksetzen
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Den gesamten Puffer leeren.
            // Die Schleife beginnt von vorne.
        }
    }
}

// Lagert die Anzeige des Men�s in eine eigene Funktion aus, um den main-Code lesbarer zu machen.
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

// Fordert den Benutzer auf, die Operanden einzugeben und bereitet die Daten f�r den Versand vor.
// Lagert die Auswahl der verschieden Operationen in eine eigene Funktion aus, um den main-code lesbarer zu machen.
void handleOperation(char op) {
    float operand1, operand2; // Variablen f�r die beiden Gleitkommazahlen, die der Benutzer eingibt.
    std::string operand1_name, operand2_name; // Variablen f�r die Text Bezeichnungen z.B. 'Summand'.

    // Legt die passenden Namen f�r die Operanden fest, je nach gew�hlter Operation.
    switch (op) {
    case '+': operand1_name = "Summand 1"; operand2_name = "Summand 2"; break;
    case '-': operand1_name = "Minuend"; operand2_name = "Subtrahend"; break;
    case '*': operand1_name = "Faktor 1"; operand2_name = "Faktor 2"; break;
    case '/': operand1_name = "Dividend"; operand2_name = "Divisor"; break;
    }

    // Eingabe der Zahlen mit der Funktion, welche falsch Eingaben korrekt behandelt.
    operand1 = getFloatFromUser("Bitte geben Sie den Wert fuer " + operand1_name + " ein: ");
    operand2 = getFloatFromUser("Bitte geben Sie den Wert fuer " + operand2_name + " ein: ");


    // Baut den finalen String zusammen, der im korrekten Format an den Arduino gesendet wird.
    std::string expression = std::to_string(operand1) + " " + op + " " + std::to_string(operand2);

    // Simulierte Kommunikation
    std::cout << "\nAn Arduino gesendet (simuliert): " << expression << std::endl;
    std::string mockResult = "Ergebnis: 25.0 (simuliert)";

    // Gibt das (aktuell simulierte) Ergebnis vom Arduino auf der Konsole aus.
    std::cout << mockResult << std::endl;
}

// Die main-Funktion ist der Haupteinstiegspunkt des Programms. Die Ausf�hrung beginnt hier.
int main()
{
    std::cout << "======================================\n";
    std::cout << "   Mikrocontroller-Kalkulator-Projekt   \n";
    std::cout << "======================================\n";

    // Startet die Hauptschleife des Programms, die sich wiederholt, bis der Benutzer das Beenden w�hlt.
    while (true)
    {
        displayMenu(); // Greift auf die oben definierte displayMenu Funktion zur�ck und ruft diese auf

        int choice;
        std::cin >> choice; // Liest die numerische Auswahl des Benutzers ein.

        // Pr�ft ob die Eingabe fehlerhaft war 
        if (std::cin.fail()) {
            std::cout << "FEHLER: Bitte geben Sie nur eine Zahl zwischen 1 und 5 ein.\n";
            std::cin.clear(); // Setzt die internen Fehler von cin zur�ck, damit es wieder nutzbar ist.
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Leert den restlichen Eingabepuffer.
            continue; // Springt zum Anfang der while-Schleife, um das Men� erneut anzuzeigen.
        }

        // Leert den Eingabepuffer auch bei g�ltiger Eingabe, um zu verhindern,
        // dass das verbleibende Enter-Zeichen ('\n') zuk�nftige Eingaben st�rt.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


        // Die switch-Anweisung �berprueft den Wert der Variable 'choice'.
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