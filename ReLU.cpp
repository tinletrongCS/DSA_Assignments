/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt
 * to change this license Click
 * nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this
 * template
 */

/*
 * File:   ReLU.cpp
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:44 PM
 */

#include "layer/ReLU.h"
//#include "C:\Users\lttin\Projects VS Code\DSA_24_25\BTL_2_DSA\task_3_p1\include\ann\layer\ReLU.h"
//#include "C:\Users\lttin\Projects VS Code\DSA_24_25\BTL_2_DSA\task_3_p1\include\ann\functions.h"
#include "ann/functions.h"
//#include "C:\Users\lttin\Projects VS Code\DSA_24_25\BTL_2_DSA\task_3_p1\include\sformat\fmt_lib.h"
#include "sformat/fmt_lib.h"

ReLU::ReLU(string name)
{
  if (trim(name).size() != 0)
    m_sName = name;
  else
    m_sName = "ReLU_" + to_string(++m_unLayer_idx);
}

ReLU::ReLU(const ReLU &orig) { m_sName = "ReLU_" + to_string(++m_unLayer_idx); }

ReLU::~ReLU()
{
}

xt::xarray<double> ReLU::forward(xt::xarray<double> X)
{
  m_aMask = xt::greater_equal(X, 0.0);
  xt::xarray<double> Y = X * m_aMask + 0;
  return Y;
}
xt::xarray<double> ReLU::backward(xt::xarray<double> DY)
{
  xt::xarray<double> DX = DY * m_aMask + 0;
  return DX;
}

string ReLU::get_desc()
{
  string desc = fmt::format("{:<10s}, {:<15s}:", "ReLU", this->getname());
  return desc;
}