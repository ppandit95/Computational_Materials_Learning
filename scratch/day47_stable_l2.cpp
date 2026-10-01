#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

double l2_error(const std::vector<double>& numerical,
                const std::vector<double>& reference);

int main()
{
    const std::vector<double> numerical{1e200, 1e200};
    const std::vector<double> reference{0.0, 0.0};

    std::cout << l2_error(numerical, reference) << '\n';

    return 0;
}

double l2_error(const std::vector<double>& numerical,
                const std::vector<double>& reference)
{
    if (numerical.size() != reference.size()) {
        throw std::invalid_argument(
            "The size of 2 vectors should match");
    }

    if (numerical.empty()) {
        return 0.0;
    }

    double scale = 0.0;
    double sumsq = 1.0;

    for (std::size_t i = 0; i < numerical.size(); ++i) {

        const double a =
            std::abs(numerical[i] - reference[i]);

        if (a != 0.0) {

            if (scale < a) {

                const double ratio = scale / a;

                sumsq =
                    1.0 + sumsq * ratio * ratio;

                scale = a;

            } else {

                const double ratio = a / scale;

                sumsq += ratio * ratio;
            }
        }
    }

    return scale * std::sqrt(sumsq);
}