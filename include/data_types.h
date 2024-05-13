#include <vector>
#include <complex>

typedef double DOUBLE;
typedef int INT;

using ARRAY_I = std::vector<int>;
using ARRAY_D = std::vector<double>;
using COMPLEX = std::complex<double>;
using ARRAY_COMPLEX = std::vector<COMPLEX>;

enum class WindowType
{
    Bartlett = 0,    /*!< Bartlett Window */
    Blackman,        /*!< Blackman Window */
    BlackmanHarris,  /*!< Blackman-Harris Window */
    BlackmanNuttall, /*!< Blackman-Nuttall Window*/
    Boxcar,          /*!< Boxcar Window */
    FlatTop,         /*!< FlatTop Window */
    Hamming,         /*!< Hamming Window */
    Hanning,         /*!< Hanning Window */
    Rectangular,     /*!< Rectangular Window */
    Triangular,      /*!< Triangular Window */
    Welch            /*!< Welch Window */
};