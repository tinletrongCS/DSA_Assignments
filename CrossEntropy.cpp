/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt
 * to change this license Click
 * nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this
 * template
 */

/*
 * File:   CrossEntropy.cpp
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:47 PM
 */

#include "loss/CrossEntropy.h"
#include "ann/functions.h"

CrossEntropy::CrossEntropy(LossReduction reduction) : ILossLayer(reduction) {}

CrossEntropy::CrossEntropy(const CrossEntropy& orig) : ILossLayer(orig) {}

CrossEntropy::~CrossEntropy() {}

double CrossEntropy::forward(xt::xarray<double> X, xt::xarray<double> t) {
  int nsamples = X.shape()[0];
    
    // Lưu `Ypred` và `Ytarget` để sử dụng trong backward
    m_aCached_Ypred = X;
    m_aYtarget = t;

    // Thêm 1e-7 để tránh log(0)
    xt::xarray<double> log_X = xt::log(X + 1e-7);
    
    // Tính Cross-Entropy: R = -t * log(X) và sum(R, -1)
    xt::xarray<double> R = -t * log_X;
    R = xt::sum(R, -1);  // Tổng theo hàng (axis=-1)

    // Tính tổng của R
    xt::xarray<double> sum_R = xt::sum(R);

    // Tính Nnorm tùy theo `m_eReduction`
    if (m_eReduction == REDUCE_MEAN) {
        return (sum_R / nsamples)();
    } else if (m_eReduction == REDUCE_SUM) {
        return sum_R();
    } else {
        return R(0);  // REDUCE_NONE
    }
}
xt::xarray<double> CrossEntropy::backward() {
  const double EPSILON = 1e-7;
  int N = this->m_aYtarget.shape()[0];

  xt::xarray<double> grad = - (m_aYtarget / (m_aCached_Ypred + EPSILON));

  if (this->m_eReduction == REDUCE_MEAN)
  {
    grad /= N;
  } 

  return grad;
}