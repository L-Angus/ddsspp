
#include <iostream>

#include "dsp.hpp"

int main()
{
    // std::cout << "Hello World!" << std::endl;
    // ARRAY_D array1{2.0, 3.0, -4.0, 5.0, 6.0};
    // ARRAY_D res;
    // // 写一个复数的vector

    // ARRAY_COMPLEX array2{COMPLEX(1.0, 2.0), COMPLEX(3.0, 4.0), COMPLEX(5.0, 6.0), COMPLEX(7.0, 8.0)};
    // ARRAY_COMPLEX array3;

    // DSP::DSP_ABS_VEC(array2, res);

    ARRAY_D array1{2.0, 3.0, -4.0, 5.0, 6.0};
    double mean, sigma;

    DSP::DSP_MEAN<double>(array1, &mean, &sigma);

    for (auto &r : array1)
    {
        std::cout << r << std::endl;
    }

    std::cout << "mean: " << mean << std::endl;
    std::cout << "sigma: " << sigma << std::endl;

    return 0;
}
