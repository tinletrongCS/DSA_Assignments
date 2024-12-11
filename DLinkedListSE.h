// COPY FILE BTL3

/*
 * File:   DLinkedListSE.h
 * Author: LTSACH
 *
 * Created on 31 August 2020, 14:13
 */

#ifndef DLINKEDLISTSE_H
#define DLINKEDLISTSE_H
#include "list/DLinkedList.h"
#include "sorting/ISort.h"

template <class T>
class DLinkedListSE : public DLinkedList<T>
{
public:
    DLinkedListSE(
        void (*removeData)(DLinkedList<T> *) = 0,
        bool (*itemEQ)(T &, T &) = 0) : DLinkedList<T>(removeData, itemEQ) {

                                        };

    DLinkedListSE(const DLinkedList<T> &list)
    {
        this->copyFrom(list);
    }

    void sort(int (*comparator)(T &, T &) = 0)
    {
        this->head = mergeSort(this->head, comparator);
    };

private:
    typename DLinkedList<T>::Node *split(typename DLinkedList<T>::Node *head)
    {
        // trả về node giữa của list
        typename DLinkedList<T>::Node *slow = head;
        typename DLinkedList<T>::Node *fast = head->next;

        // Sử dụng 2 con trỏ để tìm điểm giữa của danh sách
        while (fast != this->tail && fast->next != this->tail)
        {
            slow = slow->next;
            fast = fast->next->next;
        }

        typename DLinkedList<T>::Node *mid = slow->next;
        slow->next = this->tail; // Chia đôi danh sách
        return mid;              // Trả về điểm giữa
    }

    typename DLinkedList<T>::Node *merge(typename DLinkedList<T>::Node *left,
                                         typename DLinkedList<T>::Node *right,
                                         int (*comparator)(T &, T &) = 0)
    {
        // base case 
        if (left == this->tail)
            return right;
        if (right == this->tail)
            return left;

        if (compare(left->data, right->data, comparator) <= 0)
        {
            left->next = merge(left->next, right, comparator); // liên kết xuôi truyền left vô mảng kết quả 
            if (left->next != this->tail)
            {
                left->next->prev = left; // liên kết ngược
            }
            return left; // Trả về node nhỏ hơn
        }
        else
        {
            right->next = merge(left, right->next, comparator);
            if (right->next != this->tail)
            {
                right->next->prev = right; // Liên kết ngược
            }
            return right; // Trả về node nhỏ hơn
        }
    }

    typename DLinkedList<T>::Node *mergeSort(typename DLinkedList<T>::Node *head,
                                             int (*comparator)(T &, T &) = 0)
    {
        if (head == this->tail || head->next == this->tail)
        {
            return head;
        }
                    
        typename DLinkedList<T>::Node *mid = split(head);
        typename DLinkedList<T>::Node *left = mergeSort(head, comparator);
        typename DLinkedList<T>::Node *right = mergeSort(mid, comparator);

        return merge(left, right, comparator); // Gộp hai nửa đã sắp xếp
    }

protected:
    static int compare(T &lhs, T &rhs, int (*comparator)(T &, T &) = 0)
    {
        if (comparator != 0)
            return comparator(lhs, rhs);
        else
        {
            if (lhs < rhs)
                return -1;
            else if (lhs > rhs)
                return +1;
            else
                return 0;
        }
    }
};

#endif /* DLINKEDLISTSE_H */
