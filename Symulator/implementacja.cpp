#define _CRT_SECURE_NO_WARNINGS
#include "klasy.h"

using namespace std;

int main() {
    std::vector<double> a = { -0.4 };
    std::vector<double> b = { 0.6 };
    ARXModel arxModel(a, b, 0.01);
    PIDController pid(1.0, 0.1, 0.05);

    pid.ustawLimity(-1.0, 1.0);

    double wartoscZadana = 1.0;
    double wartoscProcesu = 0.0;
    for (int i = 0; i < 100; ++i) {
        double sygnalKontrolny = pid.oblicz(wartoscZadana, wartoscProcesu);
        wartoscProcesu = arxModel.krok(sygnalKontrolny);
        std::cout << "Krok: " << i
            << " -> Sterowanie: " << sygnalKontrolny
            << " Wyjscie: " << wartoscProcesu
            << std::endl;
    }

    // Zapis do plików
    arxModel.zapiszText("arx.txt");
    pid.zapiszText("pid.txt");
    arxModel.zapiszBin("arx.bin");
    pid.zapiszBin("pid.bin");

    // Odczyt z plików
    ARXModel wczytajARX({0}, {0});
    PIDController wczytajPID(0.0, 0.0, 0.0);
    wczytajARX.wczytajText("arx.txt");
    wczytajPID.wczytajText("pid.txt");

    pid.ustawLimity(-1.0, 1.0);

    double wartoscZadana2 = 1.0;
    double wartoscProcesu2 = 0.0;
    for (int i = 0; i < 100; ++i) {
        double sygnalKontrolny2 = wczytajPID.oblicz(wartoscZadana2, wartoscProcesu2);
        wartoscProcesu2 = wczytajARX.krok(sygnalKontrolny2);
        std::cout << "Krok: " << i
            << " -> Sterowanie: " << sygnalKontrolny2
            << " Wyjscie: " << wartoscProcesu2
            << std::endl;
    }

    return 0;

}
