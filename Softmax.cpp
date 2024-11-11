/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt
 * to change this license Click
 * nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this
 * template
 */

/*
 * File:   Softmax.cpp
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:46 PM
 */

#include "layer/Softmax.h"

#include <filesystem>  //require C++17

#include "ann/functions.h"
#include "sformat/fmt_lib.h"
namespace fs = std::filesystem;

Softmax::Softmax(int axis, string name) : m_nAxis(axis) {
  if (trim(name).size() != 0)
    m_sName = name;
  else
    m_sName = "Softmax_" + to_string(++m_unLayer_idx);
}

Softmax::Softmax(const Softmax& orig) {}

Softmax::~Softmax() {}

xt::xarray<double> Softmax::forward(xt::xarray<double> X) {
    // Tính e^X và trừ max(X) để tránh tràn số
    xt::xarray<double> exp_X = xt::exp(X - xt::amax(X));
    xt::xarray<double> softmax_output = exp_X / xt::sum(exp_X);

    // Lưu giá trị đầu ra để dùng cho backward pass
    this->m_aCached_Y = softmax_output;

    return softmax_output;
}

xt::xarray<double> Softmax::backward(xt::xarray<double> DY) {
    xt::xarray<double> Y = this->m_aCached_Y;
    xt::xarray<double> diag_Y = xt::diag(Y);
    xt::xarray<double> outer_Y = xt::linalg::outer(Y, Y);
    xt::xarray<double> jacobian = diag_Y - outer_Y;
    xt::xarray<double> DZ = xt::linalg::dot(jacobian, DY);

    return DZ;
}


string Softmax::get_desc() {
  string desc = fmt::format("{:<10s}, {:<15s}: {:4d}", "Softmax",
                            this->getname(), m_nAxis);
  return desc;
}
