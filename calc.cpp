/*
Rajout de quelques commentaires
*/
#include "calc.hpp"
#include <chrono>
#include <iostream>
#include <date/date.h>

int mavar_globale;

int add(int i, int j)
{
    return i + i;
}

void print_current_date()
{
    auto nbs = std::chrono::system_clock::now();
    auto nbj = date::floor<date::days>(nbs);
    auto annee_mois_jour = date::year_month_day(nbj);
    std::cout << annee_mois_jour;
}