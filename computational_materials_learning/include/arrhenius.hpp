#pragma once
#include<vector>
double arrhenius_diffusivity(double T,
                             double D0,
                             double Q,
                             double R);
std::vector<double> arrhenius_diffusivities(
    const std::vector<double>& temperatures,
    double D0,
    double Q,
    double R);
