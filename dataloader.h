/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/*
 * File:   dataloader.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 4:01 PM
 */

/*
Người thực hiện : Lê Trọng Tín
MSSV : 2313452
Ngày thực hiện : 01 tháng 11 năm 2024 
*/

#ifndef DATALOADER_H
#define DATALOADER_H
#include "tensor/xtensor_lib.h"
#include "loader/dataset.h"

using namespace std;

template <typename DType, typename LType>
class DataLoader
{
public:
    class Iterator; // forward declaration for class Iterator

private:
    Dataset<DType, LType> *ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;
    int nbatch;
    ulong_tensor item_indices;
    int m_seed;
    TensorDataset<DType, LType> *tensor_pointer;

public:
    DataLoader(Dataset<DType, LType> *ptr_dataset,
               int batch_size, bool shuffle = true,
               bool drop_last = false, int seed = -1)
        : ptr_dataset(ptr_dataset),
          batch_size(batch_size),
          shuffle(shuffle),
          m_seed(seed)
    {
        nbatch = ptr_dataset->len() / batch_size;
        item_indices = xt::arange(0, ptr_dataset->len());
        if (this->shuffle)
        {
            // seed >= 0 thì xử lí seed trước khi shuffle, không thì thôi
            if (this->m_seed >= 0)
            {
                xt::random::seed(m_seed);
            }
            xt::random::shuffle(this->item_indices);
        }
        this->tensor_pointer = dynamic_cast<TensorDataset<DType, LType> *>(ptr_dataset);
    }
    virtual ~DataLoader() {}

    // New method: from V2: begin
    int get_batch_size() { return batch_size; }
    int get_sample_count() { return ptr_dataset->len(); }
    int get_total_batch() { return int(ptr_dataset->len() / batch_size); }

    // New method: from V2: end
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// START: Section                                                     //
    /////////////////////////////////////////////////////////////////////////
public:
    Iterator begin()
    {
        return DataLoader::Iterator(this, 0);
    }
    Iterator end()
    {
        return DataLoader::Iterator(this, this->get_total_batch());
    }

    // BEGIN of Iterator
        class Iterator
    {
    private:
        DataLoader *loader;
        int current_batch_index;

    public:
        Iterator(DataLoader *loader, int batch_index)
        {
            this->loader = loader;
            this->current_batch_index = batch_index;
        }

        Iterator &operator=(const Iterator &iterator)
        {
            this->loader = iterator.loader;
            this->current_batch_index = iterator.current_batch_index;
            return *this;
        }

        Iterator &operator++()
        {
            this->current_batch_index++;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }

        bool operator!=(const Iterator &other) const
        {
            return (this->current_batch_index != other.current_batch_index || this->loader != other.loader);
        }

        Batch<DType, LType> operator*() const
        {
            // cout << "current batch : " << this->current_batch_index << '\n';
            int start_index = current_batch_index * loader->get_batch_size();
            int end_index;

            if (current_batch_index == loader->get_total_batch() - 1)
            {
                if (this->loader->drop_last)
                {
                    end_index = start_index + this->loader->get_batch_size();
                }
                else if (!this->loader->drop_last)
                {
                    end_index = this->loader->item_indices.size();
                }
            }
            else
            {
                end_index = start_index + this->loader->get_batch_size();
            }
            // cout << "start = " << start_index << "," << "end=" << end_index << '\n';
            xt::svector<unsigned long> data_shape = loader->tensor_pointer->get_data_shape();
            xt::svector<unsigned long> label_shape = loader->tensor_pointer->get_label_shape();

            xt::xarray<DType> batch_data = xt::empty<DType>(data_shape);
            xt::xarray<LType> batch_label = xt::empty<LType>(label_shape);

            for (int i = start_index; i != end_index; ++i)
            {
                DataLabel<DType, LType> return_item = this->loader->tensor_pointer->getitem(loader->item_indices[i]);
                xt::view(batch_data, i - start_index) = return_item.getData();
                // cout << "return data item : " << return_item.getData() << '\n';

                if (loader->tensor_pointer->getLabel().shape().size() == 0)
                {
                    batch_label = xt::xarray<LType>{};
                }
                else
                {
                    xt::view(batch_label, i - start_index) = return_item.getLabel();
                }
                // cout << "return label item : " << return_item.getLabel() << '\n';
            }

            // Kiểm tra và loại bỏ hàng thừa
            if (batch_data.shape()[0] > (end_index - start_index))
            {
                batch_data = xt::view(batch_data, xt::range(0, end_index - start_index), xt::all());
            }

            if (batch_label.shape()[0] > (end_index - start_index))
            {
                batch_label = xt::view(batch_label, xt::range(0, end_index - start_index));
            }
            return Batch<DType, LType>(batch_data, batch_label);
        }
    };

    // END of Iterator

    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// END: Section                                                       //
    /////////////////////////////////////////////////////////////////////////
};

#endif /* DATALOADER_H */
