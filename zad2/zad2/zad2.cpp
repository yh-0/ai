#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <random>

using namespace std;

double funkcjaTestowa(double x)
{
    return 3 * sin(M_PI * x / 5) + sin(M_PI * x);
}

double randomInRange(double start, double end, double absStart, double absEnd)
{
    random_device rd;
    default_random_engine eng(rd());
    uniform_real_distribution<double> distr(start, end);

    double x = distr(eng);

    if (x < absStart) x = absStart;
    if (x > absEnd) x = absEnd;

    return x;
}

double wyzarzanie(double start, double end, double t0, double n, int epoki, int proby)
{
    double x0, x1, f0, f1, prob, probRand;

    x0 = randomInRange(0, 10, 0, 10);

    for (int i = 0; i < epoki; i++)
    {
        for (int j = 0; j < proby; j++)
        {
            f0 = funkcjaTestowa(x0);
            x1 = randomInRange(x0 - 2 * t0, x0 + 2 * t0, 0, 10);
            f1 = funkcjaTestowa(x1);

            if (f1 >= f0) x0 = x1;
            if (f1 < f0)
            {
                prob = exp((f1 - f0) / t0);
                probRand = randomInRange(0, 1, 0, 1);
                if (prob > probRand) x0 = x1;
            }
        }

        t0 *= n;
    }

    return x0;
}

int main()
{
    cout << wyzarzanie(0, 10, 1, 0.9, 5, 3);
}
