
#ifndef XTENSOR_LIB_H
#define XTENSOR_LIB_H
#include <string>
#include <sstream>
using namespace std;

//#include "xtensor/xio.hpp"
#include "C:\Users\lttin\Projects VS Code\DSA_24_25\BTL_1_DSA\include\xtensor\xio.hpp"
//#include "xtensor/xview.hpp"
#include "C:\Users\lttin\Projects VS Code\DSA_24_25\BTL_1_DSA\include\xtensor\xview.hpp"
//#include "xtensor/xslice.hpp"
#include "C:\Users\lttin\Projects VS Code\DSA_24_25\BTL_1_DSA\include\xtensor\xslice.hpp"
//#include "xtensor/xbuilder.hpp"
#include "C:\Users\lttin\Projects VS Code\DSA_24_25\BTL_1_DSA\include\xtensor\xbuilder.hpp"
//#include "xtensor-blas/xlinalg.hpp"
#include "C:\Users\lttin\Projects VS Code\DSA_24_25\BTL_1_DSA\include\xtensor-blas\xlinalg.hpp"
//#include "xtensor/xrandom.hpp"
#include "C:\Users\lttin\Projects VS Code\DSA_24_25\BTL_1_DSA\include\xtensor\xrandom.hpp"
//#include "xtensor/xindex_view.hpp"
#include "C:\Users\lttin\Projects VS Code\DSA_24_25\BTL_1_DSA\include\xtensor\xindex_view.hpp"
//#include "xtensor/xsort.hpp"
#include "C:\Users\lttin\Projects VS Code\DSA_24_25\BTL_1_DSA\include\xtensor\xsort.hpp"
//#include "xtensor/xarray.hpp"
#include "C:\Users\lttin\Projects VS Code\DSA_24_25\BTL_1_DSA\include\xtensor\xarray.hpp"
#include "C:\Users\lttin\Projects VS Code\DSA_24_25\BTL_1_DSA\include\xtl\xclosure.hpp"
#include <ctime>

typedef size_t ulong;
typedef xt::xarray<ulong> ulong_array;
typedef xt::xarray<double> double_array;
enum class_metrics{
    ACCURACY = 0,
    PRECISION_MACRO,
    PRECISION_WEIGHTED,
    RECALL_MACRO,
    RECALL_WEIGHTED,
    F1_MEASURE_MACRO,
    F1_MEASURE_WEIGHTED,
    NUM_CLASS_METRICS
};


string shape2str(xt::svector<size_t> vec);
int positive_index(int idx, int size);
xt::xarray<double> outer_stack(xt::xarray<double> X, xt::xarray<double>  Y);
xt::xarray<double> diag_stack(xt::xarray<double> X);
xt::xarray<double> matmul_on_stack(xt::xarray<double> X, xt::xarray<double>  Y);

xt::xarray<ulong> confusion_matrix(xt::xarray<ulong> y_true, xt::xarray<ulong> y_pred);
xt::xarray<ulong> class_count(xt::xarray<ulong> confusion);
double_array calc_metrics(ulong_array y_true, ulong_array y_pred);


#endif /* XTENSOR_LIB_H */

