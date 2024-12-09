#define _CRT_SECURE_NO_WARNINGS
#include "klasy.h"

using namespace std;

int main() {

    std::vector<double> a = { -0.4 };
    std::vector<double> b = { 0.6 };
    UkladSterowania uklad;
    uklad.setARX(a, b, 0.01);
    uklad.setPID(1.0, 0.1, 0.05, -1, 1);
    uklad.setWartosc(rodzajeWartosci::sinus, -1, 1, 20);
    std::vector<double> wyniki = uklad.symulacja(100);
    uklad.zapiszPlik("arx.txt", "pid.txt", "wartosc.txt");
    uklad.wczytajPlik("arx.txt", "pid.txt", "wartosc.txt");
    std::vector<double> wyniki2 = uklad.symulacja(100);
    return 0;

}
