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
    ARXModel wczytajARX({}, {});
    PIDController wczytajPID(0.0, 0.0, 0.0);
    wczytajARX.wczytajText("arx.txt");
    wczytajPID.wczytajText("pid.txt");

    return 0;

}
