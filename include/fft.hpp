#include <complex>

using Complex = std::complex<double>;
const Complex I{0, 1};
#define PI2 6.28318530717958647692

void tfft_genw(int i, int b, Complex z, Complex *w)
{
    if (b == 0)
        w[i] = z;
    else
    {
        tfft_genw(i, b >> 1, z, w);
        tfft_genw(i | b, b >> 1, z * w[b], w);
    }
}

void tfft_init(int k, Complex *w)
{
    int i, j;
    const int m = 1 << k;
    const double arg = -PI2 / m;
    for (i = 1, j = m / 4; j; i <<= 1, j >>= 1)
    {
        w[i] = std::exp(I * (arg * j));
    }
    tfft_genw(0, m / 4, 1, w);
}

void tfft_fft(int k, Complex *A, const Complex *w)
{
    const int m = 1 << k;
    int u = 1;
    int v = m / 4;
    int i, j;
    if (k & 1)
    {
        for (j = 0; j < m / 2; j++)
        {
            Complex Ajv = A[j + (m / 2)];
            A[j + (m / 2)] = A[j] - Ajv;
            A[j] += Ajv;
        }
        u <<= 1;
        v >>= 1;
    }
    for (i = k & ~1; i > 0; i -= 2)
    {
        int jh;
        for (jh = 0; jh < u; jh++)
        {
            Complex wj = w[jh << 1];
            Complex wj2 = w[jh];
            Complex wj3 = wj2 * wj;
            int je;
            for (j = jh << i, je = j + v; j < je; j++)
            {
                Complex tmp0 = A[j];
                Complex tmp1 = wj * A[j + v];
                Complex tmp2 = wj2 * A[j + 2 * v];
                Complex tmp3 = wj3 * A[j + 3 * v];

                Complex ttmp0 = tmp0 + tmp2;
                Complex ttmp2 = tmp0 - tmp2;
                Complex ttmp1 = tmp1 + tmp3;
                Complex ttmp3 = -I * (tmp1 - tmp3);

                A[j] = ttmp0 + ttmp1;
                A[j + v] = ttmp0 - ttmp1;
                A[j + 2 * v] = ttmp2 + ttmp3;
                A[j + 3 * v] = ttmp2 - ttmp3;
            }
        }
        u <<= 2;
        v >>= 2;
    }
}

void tfft_ifft(int k, Complex *A, const Complex *w)
{
    const int m = 1 << k;
    int u = m / 4;
    int v = 1;
    int i, j;
    for (i = 2; i <= k; i += 2)
    {
        int jh;
        for (jh = 0; jh < u; jh++)
        {
            Complex wj = conj(w[jh << 1]);
            Complex wj2 = conj(w[jh]);
            Complex wj3 = wj2 * wj;
            int je;
            for (j = jh << i, je = j + v; j < je; j++)
            {
                Complex tmp0 = A[j];
                Complex tmp1 = A[j + v];
                Complex tmp2 = A[j + 2 * v];
                Complex tmp3 = A[j + 3 * v];

                Complex ttmp0 = tmp0 + tmp1;
                Complex ttmp1 = tmp0 - tmp1;
                Complex ttmp2 = tmp2 + tmp3;
                Complex ttmp3 = I * (tmp2 - tmp3);

                A[j] = ttmp0 + ttmp2;
                A[j + v] = wj * (ttmp1 + ttmp3);
                A[j + 2 * v] = wj2 * (ttmp0 - ttmp2);
                A[j + 3 * v] = wj3 * (ttmp1 - ttmp3);
            }
        }
        u >>= 2;
        v <<= 2;
    }
    if (k & 1)
    {
        for (j = 0; j < m / 2; j++)
        {
            Complex Ajv = A[j + (m / 2)];
            A[j + (m / 2)] = A[j] - Ajv;
            A[j] += Ajv;
        }
    }
}

void tfft_convolver(int k, Complex *A, const Complex *w)
{
    int i, y;
    const int m = 1 << k;

    tfft_fft(k, A, w);
    A[0] = 4 * std::real(A[0]) * std::imag(A[0]) * I;
    A[1] = 4 * std::real(A[1]) * std::imag(A[1]) * I;
    i = 2;
    for (y = 2; y < m; y <<= 1)
    {
        for (; i < 2 * y; i += 2)
        {
            int j = i ^ (y - 1);
            A[i] = (A[i] + conj(A[j])) * (A[i] - conj(A[j]));
            A[j] = -conj(A[i]);
        }
    }

    for (i = 0; i < m; i += 2)
    {
        A[i / 2] = (-(A[i] + A[i ^ 1]) * I + (A[i] - A[i ^ 1]) * conj(w[i / 2])) / Complex(4 * m, 0);
    }

    tfft_ifft(k - 1, A, w);
}