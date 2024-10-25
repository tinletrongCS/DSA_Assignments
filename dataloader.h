/*
 * Faculity of Computer Science and Engineering
 * Author: Le Trong Tin
 * ID: 2313452
 * Date: October 6th 2024
 */
/*
* Update: October 8th 2024
* 10:25 PM
*/
#ifndef DATALOADER_H
#define DATALOADER_H
#include "ann/xtensor_lib.h"
#include "ann/dataset.h"
using namespace std;

template <typename DType, typename LType>
class DataLoader
{
public:
    class Iterator; // forward declaration

private:
    Dataset<DType, LType> *ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;
    int m_seed;

    int num_batches;
    xt::xarray<unsigned long> indices;
    TensorDataset<DType, LType> *tensor_pointer;

public:
    DataLoader(Dataset<DType, LType> *ptr_dataset, int batch_size, bool shuffle = true, 
    bool drop_last = false, int seed = -1)
    {
        this->ptr_dataset = ptr_dataset;
        this->batch_size = batch_size;
        this->shuffle = shuffle;
        this->drop_last = drop_last;
        this->m_seed = seed;

        this->num_batches = ptr_dataset->len() / this->batch_size;

        this->indices = xt::arange(0, ptr_dataset->len());
        if (this->shuffle)
        {
            // seed >= 0 thì xử lí seed trước khi shuffle, không thì thôi
            if (this->m_seed >= 0)
            {
                xt::random::seed(m_seed);
            }
            xt::random::shuffle(this->indices);
        }
        this->tensor_pointer = dynamic_cast<TensorDataset<DType, LType> *>(ptr_dataset);
    }

    virtual ~DataLoader() {}
    Iterator begin()
    {
        return DataLoader::Iterator(this, 0);
    }

    Iterator end()
    {
        return DataLoader::Iterator(this, this->num_batches);
    }

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
            int start_index = current_batch_index * loader->batch_size;
            int end_index;

            if (current_batch_index == loader->num_batches - 1)
            {
                if (this->loader->drop_last)
                {
                    end_index = start_index + this->loader->batch_size;
                }
                else if (!this->loader->drop_last)
                {
                    end_index = this->loader->indices.size();
                }
            }
            else
            {
                end_index = start_index + this->loader->batch_size;
            }
            // cout << "start = " << start_index << "," << "end=" << end_index << '\n';
            xt::svector<unsigned long> data_shape = loader->tensor_pointer->get_data_shape();
            xt::svector<unsigned long> label_shape = loader->tensor_pointer->get_label_shape();

            xt::xarray<DType> batch_data = xt::empty<DType>(data_shape);
            xt::xarray<LType> batch_label = xt::empty<LType>(label_shape);

            for (int i = start_index; i != end_index; ++i)
            {
                DataLabel<DType, LType> return_item = this->loader->tensor_pointer->getitem(loader->indices[i]);
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
};

#endif /* DATALOADER_H */
