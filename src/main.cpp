
// #include <iostream>
// #include <vector>
// #include <complex>
// #include "fft.hpp"

// #define K 18

// const int m = 1 << K;

// // 使用std::vector替换原生数组
// std::vector<Complex> w(1 << (K - 1));
// std::vector<Complex> A(1 << K);

// int main()
// {
//     int n;
//     std::cin >> n;

//     for (int i = 1; i <= n; ++i)
//     {
//         int a, b;
//         std::cin >> a >> b;
//         A[i] = Complex(a, b); // 使用std::complex直接构造复数
//     }

//     tfft_init(K, w.data()); // 使用.data()获取底层指针

//     tfft_fft(K, A.data(), w.data());

//     for (int i = 0; i < 2 * n; ++i)
//     {
//         std::cout << A[i].real() << " " << A[i].imag() << '\n';
//     }

//     tfft_ifft(K, A.data(), w.data());
//     std::cout << std::endl;

//     for (int i = 0; i < 2 * n; ++i)
//     {
//         A[i] /= m;
//         std::cout << A[i].real() << " " << A[i].imag() << '\n';
//     }

//     tfft_convolver(K, A.data(), w.data());

//     for (int i = 0; i <= n; ++i)
//     {
//         std::cout << static_cast<int>(A[i].real() + 0.5) << '\n';
//         std::cout << static_cast<int>(A[i].imag() + 0.5) << '\n';
//     }

//     return 0;
// }
#include <base.hpp>
#include <dft.hpp>
#include <dsp.hpp>
#include <io.hpp>
#include <iostream>
using namespace kfr;

int main()
{
    // println(library_version());

    // fft size
    const std::size_t size = 128;

    // initialize input & output buffers
    univector<complex<fbase>, size> in = sin(linspace(0.0, c_pi<fbase, 2> * 4.0, size));
    univector<complex<fbase>, size> out = scalar(qnan);

    // initialize fft
    const dft_plan<fbase> dft(size);

    dft.dump();

    // allocate work buffer for fft (if needed)
    univector<u8> temp(dft.temp_size);

    // perform forward fft
    dft.execute(out, in, temp);

    // scale output
    out = out / size;

    // get magnitude and convert to decibels
    univector<fbase, size> dB = amp_to_dB(cabs(out));

    println("max  = ", maxof(dB));
    println("min  = ", minof(dB));
    println("mean = ", mean(dB));
    println("rms  = ", rms(dB));

    println(in);
    println();
    println(dB);
    return 0;
}
