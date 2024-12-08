#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <vector>
#include <queue>
#include <random>
#include <iostream>
#include <algorithm>

class PIDController;

class ARXModel {
private:
    std::vector<double> A;
    std::vector<double> B;
    std::deque<double> u_hist;
    std::deque<double> y_hist;
    std::default_random_engine generator;
    std::normal_distribution<double> dystrybucja;

    PIDController* pidController;

public:
    ARXModel(const std::vector<double>& a, const std::vector<double>& b, double szum = 0.01)
        : A(a), B(b), dystrybucja(0.0, szum), pidController(nullptr)
    {
        size_t maxSize = std::max(A.size(), B.size());
        u_hist = std::deque<double>(maxSize, 0.0);
        y_hist = std::deque<double>(maxSize, 0.0);
    }

    void setPIDController(PIDController* pid) {
        pidController = pid;
    }

    double krok(double input) {
        u_hist.pop_front();
        u_hist.push_back(input);
        double szum = dystrybucja(generator);
        double y_k = 0.0;
        for (size_t i = 0; i < A.size(); i++) {
            y_k -= A[i] * y_hist[y_hist.size() - 1 - i];
        }
        for (size_t i = 0; i < B.size(); i++) {
            y_k += B[i] * u_hist[u_hist.size() - 1 - i];
        }
        y_k += szum;
        y_hist.pop_front();
        y_hist.push_back(y_k);
        return y_k;
    }

    PIDController* getPIDController() const {
        return pidController;
    }
};

template <typename T>
T filtr(T wartosc, T dolny, T gorny) {
    return std::max(dolny, std::min(wartosc, gorny));
}

class PIDController {
private:
    double kp, ki, kd;
    double calka, bladPoprzedzajacy;
    double dolnyLimit, gornyLimit;
    bool flagaPrzeciwNasyceniowa;

    ARXModel* arxModel;

public:
    PIDController(double kp, double ki, double kd, double dolnyLimit = -1.0, double gornyLimit = 1.0)
        : kp(kp), ki(ki), kd(kd), dolnyLimit(dolnyLimit), gornyLimit(gornyLimit),
        calka(0.0), bladPoprzedzajacy(0.0), flagaPrzeciwNasyceniowa(true), arxModel(nullptr) {}

    void setARXModel(ARXModel* model) {
        arxModel = model;
    }

    void ustawLimity(double nizszy, double wyzszy) {
        dolnyLimit = nizszy;
        gornyLimit = wyzszy;
    }

    void reset() {
        calka = 0.0;
        bladPoprzedzajacy = 0.0;
    }

    double oblicz(double ustawWartosc, double wartoscProcesu) {
        double blad = ustawWartosc - wartoscProcesu;
        calka += blad;
        double pochodna = blad - bladPoprzedzajacy;
        bladPoprzedzajacy = blad;
        double wyjscie = kp * blad + ki * calka + kd * pochodna;
        if (flagaPrzeciwNasyceniowa) {
            wyjscie = filtr(wyjscie, dolnyLimit, gornyLimit);
        }
        return wyjscie;
    }

    ARXModel* getARXModel() const {
        return arxModel;
    }
};