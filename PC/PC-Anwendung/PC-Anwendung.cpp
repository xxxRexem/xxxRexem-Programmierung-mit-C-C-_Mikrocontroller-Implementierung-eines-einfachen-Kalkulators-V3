<<<<<<< HEAD
#define NOMINMAX 
#include <iostream>
#include <string>
#include <limits>
#include <windows.h>

// Stellt eine Verbindung zum seriellen Port her, sendet Daten und empf�ngt eine Antwort.
// Der 'command'-Parameter ist der String, der an den Arduino gesendet wird.
std::string communicateWithArduino(const std::string& command) {
    // Erstellt ein "Handle", eine spezielle Variable, die als Referenz auf die serielle Verbindung dient.
    HANDLE hSerial;

    // CreateFile ist die Funktion, um eine Verbindung zur Datei zu �ffnen.
    // Das 'L' vor dem String konvertiert ihn in das von Windows erwartete Unicode-Format.
    hSerial = CreateFile(
        L"\\\\.\\COM3",      // Portname
        GENERIC_READ | GENERIC_WRITE, // Sowohl Lese als auch Schreib Operationen sollen m�glich sein.
        0,                  // Exklusiver Zugriff (Port kann nich von anderen Programmen genutzt werden).
        0,                  // Standard-Sicherheitseinstellungen.
        OPEN_EXISTING,      // Den Port nur �ffnen, wenn er bereits existiert.
        FILE_ATTRIBUTE_NORMAL, // Normale Attribute.
        0                   // Kein Template-File.
    );

    // Pr�ft, ob das �ffnen des Ports fehlerhaft war.
    if (hSerial == INVALID_HANDLE_VALUE) {
        // Gibt eine Fehlermeldung aus, falls das Handle ung�ltig ist.
        std::cerr << "Fehler beim Oeffnen des seriellen Ports COM3. Ist der Arduino angeschlossen?" << std::endl;
        return ""; // Gibt einen leeren String zur�ck, um einen Fehler zu signalisieren.
    }

    // Erstellt eine DCB-Struktur (Device Control Block), um die Einstellungen des seriellen Ports zu konfigurieren.
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams); // // Die DCBlength muss vor der Verwendung immer auf die Gr��e der Struktur gesetzt werden.

    // Holt die aktuellen Einstellungen des Ports, um sie im n�chsten Schritt zu modifizieren.
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Fehler beim Abrufen der Port-Einstellungen." << std::endl;
        CloseHandle(hSerial); // Port bei Fehler wieder schlie�en
        return "";
    }

    // �ndert die Einstellungen auf die, die der Arduino standardm��ig erwartet.
    dcbSerialParams.BaudRate = CBR_9600;  // Baudrate: �bertragungsgeschwindigkeit von 9600 Bits pro Sekunde.
    dcbSerialParams.ByteSize = 8;         // Byte-Gr��e: 8 Datenbits pro Byte.
    dcbSerialParams.StopBits = ONESTOPBIT; // Stopbits: 1 Stopbit am Ende jedes Bytes.
    dcbSerialParams.Parity = NOPARITY;  // Parit�t: Keine Fehlerpr�fung (Parity Bit) wird verwendet.

    // Wendet die neuen, von uns definierten Einstellungen auf die serielle Verbindung an.
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Fehler beim Setzen der Port-Einstellungen." << std::endl;
        CloseHandle(hSerial); // Port bei Fehler wieder schlie�en
        return "";
    }

    // Konfiguriert die Timeout-Einstellungen f�r Lese- und Schreibvorg�nge.
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50; // Maximale Zeit in ms zwischen dem Eintreffen zweier Zeichen.
    timeouts.ReadTotalTimeoutConstant = 50; // Eine konstante Zeit in ms, die pro Leseoperation gewartet wird.
    timeouts.ReadTotalTimeoutMultiplier = 10; // Eine zus�tzliche Wartezeit in ms pro zu lesendem Byte.
    timeouts.WriteTotalTimeoutConstant = 50; // Konstante Wartezeit f�r Schreibvorg�nge.
    timeouts.WriteTotalTimeoutMultiplier = 10; // Zus�tzliche Wartezeit pro zu schreibendem Byte.

    // Wendet die neuen Timeout-Einstellungen auf die serielle Verbindung an.
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cerr << "Fehler beim Setzen der Timeouts." << std::endl;
        CloseHandle(hSerial);
        return "";
    }

    std::cout << "[DEBUG] Serielle Verbindung zu COM3 erfolgreich geoeffnet und konfiguriert." << std::endl;

    // Deklariert Variablen, in denen Windows die Anzahl der tats�chlich �bertragenen Bytes speichert.
    DWORD bytes_written, bytes_read;
    // F�gt dem Befehl ein Newline-Zeichen hinzu, auf das der Arduino als Endezeichen wartet.
    std::string command_with_newline = command + "\n";

    // Sendet den vorbereiteten String an den Arduino .
    if (!WriteFile(hSerial, command_with_newline.c_str(), command_with_newline.length(), &bytes_written, NULL)) {
        std::cerr << "Fehler beim Senden der Daten." << std::endl;
        CloseHandle(hSerial);
        return "";
    }
    std::cout << "[DEBUG] " << bytes_written << " Bytes erfolgreich gesendet." << std::endl;

    // Erstellt einen Puffer f�r die ankommenden Daten vom Arduino.
    char read_buf[256] = { 0 };
    // Versucht, Daten vom Arduino zu lesen und im 'read_buf' zu speichern.
    if (!ReadFile(hSerial, read_buf, sizeof(read_buf) - 1, &bytes_read, NULL)) {
        std::cerr << "Fehler beim Lesen der Daten." << std::endl;
        CloseHandle(hSerial);
        return "";
    }

    // Verarbeitet die empfangenen Daten.
    std::string response = "";
    if (bytes_read > 0) { // Pr�ft, ob tats�chlich mehr als 0 Bytes empfangen wurden.
        // Konvertiert den C-artigen char-Puffer in einen modernen C++-String.
        response = std::string(read_buf, bytes_read);
        // Sucht nach dem letzten Zeichen, das kein Zeilenumbruch ist, um die Antwort zu bereinigen.
        size_t endpos = response.find_last_not_of("\r\n");
        if (std::string::npos != endpos) { // Wenn ein solches Zeichen gefunden wurde, schneidet den String so ab, dass die Zeilenumbr�che am Ende entfernt sind.
            response = response.substr(0, endpos + 1);
        }
    }
    std::cout << "[DEBUG] " << bytes_read << " Bytes erfolgreich gelesen. Antwort: " << response << std::endl;

    // Gibt die Verbindung zum seriellen Port wieder frei, damit andere Programme ihn nutzen k�nnen.
    CloseHandle(hSerial);
    std::cout << "[DEBUG] Serielle Verbindung zu COM3 geschlossen." << std::endl;

    return response; // Gibt die empfangene und bereinigte Antwort zur�ck.
}
=======
#include <iostream>
#include <string>
#include <limits>
>>>>>>> 26abe846a5b1d12ff95592a9656ca6845acdb00f

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
<<<<<<< HEAD
=======
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Restlichen Puffer leeren
>>>>>>> 26abe846a5b1d12ff95592a9656ca6845acdb00f
            return number; // Die g�ltige Zahl zur�ckgeben und die Funktion beenden.
        }
        else {
            // Die Eingabe war fehlerhaft, weil die Eingabe z.B. ein Text war.
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

<<<<<<< HEAD
// Fordert den Benutzer auf, die Operanden einzugeben und bereitet die Daten f�r den Versand vor.
=======
// Fordert den Benutzer auf, die Operanden einzugeben und bereitet die Daten f�r den Versand vor (simuliert).
>>>>>>> 26abe846a5b1d12ff95592a9656ca6845acdb00f
// Lagert die Auswahl der verschieden Operationen in eine eigene Funktion aus, um den main-code lesbarer zu machen.
void handleOperation(char op) {
    float operand1, operand2; // Variablen f�r die beiden Gleitkommazahlen, die der Benutzer eingibt.
    std::string operand1_name, operand2_name; // Variablen f�r die textuellen Bezeichnungen z.B. 'Summand'.

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

<<<<<<< HEAD
    // --- ECHTE KOMMUNIKATION ---
    std::cout << "\nSende '" << expression << "' an den Arduino..." << std::endl;
    // Die Funktion aufrufen und die echte Antwort vom Arduino in einer Variable speichern.
    std::string resultFromServer = communicateWithArduino(expression);

    // Das finale Ergebnis (oder eine Fehlermeldung) anzeigen.
    std::cout << "\n--- Ergebnis ---\n";
    // Pr�fen, ob die Kommunikation erfolgreich war (Antwort ist nicht leer)
    if (!resultFromServer.empty()) {
        std::cout << resultFromServer << std::endl;
    }
    else {
        // Falls die Kommunikation fehlschl�gt (leerer String), eine Fehlermeldung ausgeben.
        std::cout << "Fehler: Keine Antwort vom Arduino erhalten." << std::endl;
    }
    std::cout << "----------------\n";
=======
    // --- Platzhalter f�r die Kommunikation ---
    std::cout << "\nAn Arduino gesendet (simuliert): " << expression << std::endl;
    std::string mockResult = "Ergebnis: 25.0 (simuliert)";
    // --- Ende des Platzhalters ---

    // Gibt das (aktuell simulierte) Ergebnis vom Arduino auf der Konsole aus.
    std::cout << mockResult << std::endl;
>>>>>>> 26abe846a5b1d12ff95592a9656ca6845acdb00f
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

<<<<<<< HEAD
        // Leert den Eingabepuffer auch bei g�ltiger Eingabe, um zu verhindern,
        // dass das verbleibende Enter-Zeichen ('\n') zuk�nftige Eingaben st�rt.
=======
        // Leert den Eingabepuffer auch bei g�ltiger Eingabe, um zu verhindern, dass das verbleibende Enter-Zeichen ('\n') zuk�nftige Eingaben st�rt.
>>>>>>> 26abe846a5b1d12ff95592a9656ca6845acdb00f
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