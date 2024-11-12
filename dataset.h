// TODO CODE PRE TASK
/*
* Faculity of Computer Science and Engineering
* Author: Le Trong Tin
* ID: 2313452
* Date: October 6th 2024
*/
#ifndef DATASET_H
#define DATASET_H
#include "tensor/xtensor_lib.h"
using namespace std;
// file final
template <typename DType, typename LType>
class DataLabel
{
private:
  xt::xarray<DType> data;
  xt::xarray<LType> label;

public:
  DataLabel(xt::xarray<DType> data, xt::xarray<LType> label)
      : data(data), label(label) {}
  xt::xarray<DType> getData() const { return data; }
  xt::xarray<LType> getLabel() const { return label; }
};

template <typename DType, typename LType>
class Batch
{
private:
  xt::xarray<DType> data;
  xt::xarray<LType> label;

public:
  Batch(xt::xarray<DType> data, xt::xarray<LType> label)
      : data(data), label(label) {}
  virtual ~Batch() {}
  xt::xarray<DType> &getData() { return data; }
  xt::xarray<LType> &getLabel() { return label; }
};

template <typename DType, typename LType>
class Dataset
{
private:
public:
  Dataset() {};
  virtual ~Dataset() {};

  virtual int len() = 0;
  virtual DataLabel<DType, LType> getitem(int index) = 0;
  virtual xt::svector<unsigned long> get_data_shape() = 0;
  virtual xt::svector<unsigned long> get_label_shape() = 0;
};

//////////////////////////////////////////////////////////////////////
template <typename DType, typename LType>
class TensorDataset : public Dataset<DType, LType>
{
private:
  xt::xarray<DType> data;
  xt::xarray<LType> label;
  xt::svector<unsigned long> data_shape, label_shape;

public:
  TensorDataset(xt::xarray<DType> data, xt::xarray<LType> label)
  {
    this->data = data;
    this->label = label;
    this->data_shape = xt::svector<unsigned long>(data.shape().begin(), data.shape().end());
    this->label_shape = xt::svector<unsigned long>(label.shape().begin(), label.shape().end());
  }

  int len()
  {
    if (data.size() == 0)
      return 0;
    return this->data_shape[0];
  }

  DataLabel<DType, LType> getitem(int index)
  {
    if (index < 0 || index >= this->len())
    {
      throw std::out_of_range("Index is out of range!");
    }
    if (this->getLabel().shape().size() == 0)
    {
      return DataLabel<DType, LType>(xt::view(this->data, index, xt::all()), xt::xarray<LType>{});
    }
    else if (this->getData().shape().size() == 0)
    {
      return DataLabel<DType, LType>(xt::xarray<DType>{}, xt::view(this->label, index, xt::all()));
    }
    else if (this->getLabel().shape().size() == 1)
    {
      return DataLabel<DType, LType>(xt::view(this->data, index, xt::all()), xt::view(this->label, index));
    }
    else if (this->getLabel().shape().size() == 0 && this->getData().shape().size() == 0)
    {
      return DataLabel<DType, LType>(xt::xarray<DType>{}, xt::xarray<LType>{});
    }
    else
      return DataLabel<DType, LType>(xt::view(this->data, index, xt::all()), xt::view(this->label, index, xt::all()));
  }
  xt::svector<unsigned long> get_data_shape()
  {
    return this->data_shape;
  }
  xt::svector<unsigned long> get_label_shape()
  {
    return this->label_shape;
  }
  xt::xarray<DType> getData()
  {
    return this->data;
  }
  xt::xarray<LType> getLabel()
  {
    return this->label;
  }
};

#endif /* DATASET_H */
