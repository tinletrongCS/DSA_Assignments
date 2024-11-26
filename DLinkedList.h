// ASSIGNMENT 2 ---------------------- *************** ----------------------
/*
* Faculity of Computer Science and Engineering
* Author: Le Trong Tin
* ID: 2313452
* Date: November 10th, 2024
*/
/*
 * File:   DLinkedList.h
 */
// FILE ĐỊNH DẠNG MỚI

// file chỉnh ngày 09 tháng 10
#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H
#include "IList.h"
//#include "list/IList.h"
#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;

template <class T>
class DLinkedList : public IList<T>
{
public:
    class Node;        // Forward declaration
    class Iterator;    // Forward declaration
    class BWDIterator; // Forward declaration

protected:
    Node *head; // this node does not contain user's data
    Node *tail; // this node does not contain user's data
    int count;
    bool (*itemEqual)(T &lhs, T &rhs);        // function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(DLinkedList<T> *); // function pointer: be called to remove items (if they are pointer type)

public:
    DLinkedList(
            void (*deleteUserData)(DLinkedList<T> *) = 0,
            bool (*itemEqual)(T &, T &) = 0);
    DLinkedList(const DLinkedList<T> &list);
    DLinkedList<T> &operator=(const DLinkedList<T> &list);
    ~DLinkedList();

    // Inherit from IList: BEGIN
    void add(T e);
    void add(int index, T e);
    T removeAt(int index);
    bool removeItem(T item, void (*removeItemData)(T) = 0);
    bool empty();
    int size();
    void clear();
    T &get(int index);
    int indexOf(T item);
    bool contains(T item);
    string toString(string (*item2str)(T &) = 0);
    // Inherit from IList: END

    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    bool contains(T array[], int size)
    {
        int idx = 0;
        for (DLinkedList<T>::Iterator it = begin(); it != end(); it++)
        {
            if (!equals(*it, array[idx++], this->itemEqual))
                return false;
        }
        return true;
    }

    /*
     * free(DLinkedList<T> *list):
     *  + to remove user's data (type T, must be a pointer type, e.g.: int*, Point*)
     *  + if users want a DLinkedList removing their data,
     *      he/she must pass "free" to constructor of DLinkedList
     *      Example:
     *      DLinkedList<T> list(&DLinkedList<T>::free);
     */
    static void free(DLinkedList<T> *list)
    {
        typename DLinkedList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }

    /* begin, end and Iterator helps user to traverse a list forwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::Iterator it;
     for(it = list.begin(); it != list.end(); it++){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    Iterator begin()
    {
        return Iterator(this, true);
    }
    Iterator end()
    {
        return Iterator(this, false);
    }

    /* last, beforeFirst and BWDIterator helps user to traverse a list backwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::BWDIterator it;
     for(it = list.last(); it != list.beforeFirst(); it--){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    BWDIterator bbegin()
    {
        return BWDIterator(this, true);
    }
    BWDIterator bend()
    {
        return BWDIterator(this, false);
    }

protected:
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }
    void copyFrom(const DLinkedList<T> &list);
    void removeInternalData();
    Node *getPreviousNodeOf(int index);

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    class Node
    {
    public:
        T data;
        Node *next;
        Node *prev;
        friend class DLinkedList<T>;

    public:
        Node(Node *next = 0, Node *prev = 0)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node *next = 0, Node *prev = 0)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    //////////////////////////////////////////////////////////////////////
    class Iterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;

    public:
        Iterator(DLinkedList<T> *pList = 0, bool begin = true)
        {
            if (begin)
            {
                if (pList != 0)
                    this->pNode = pList->head->next;
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->tail;
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        Iterator &operator=(const Iterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->prev; // MUST prev, so iterator++ will go to end
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T &operator*()
        {
            return pNode->data;
        }
        bool operator!=(const Iterator &iterator)
        {
            return pNode != iterator.pNode;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            pNode = pNode->next;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };


    class BWDIterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;
    public:
        BWDIterator(DLinkedList<T> *pList = 0, bool begin = true)
        {
            this->pList = pList;
            if (pList != 0)
            {
                if (begin)
                {
                    this->pNode = pList->tail->prev;
                }
                else
                {
                    this->pNode = pList->head;
                }
            }
            else
            {
                this->pNode = 0;
            }
        }

        Iterator &operator=(const BWDIterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->prev; // MUST prev, so iterator++ will go to end
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T &operator*()
        {
            return pNode->data;
        }
        bool operator!=(const BWDIterator &iterator)
        {
            return pNode != iterator.pNode;
        }
        // Prefix ++ overload
        BWDIterator &operator++()
        {
            pNode = pNode->prev;
            return *this;
        }
        BWDIterator &operator--()
        {
            pNode = pNode->prev;
            return *this;
        }
        // Postfix ++ overload
        BWDIterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
        BWDIterator operator--(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };
};


//////////////////////////////////////////////////////////////////////
// Define a shorter name for DLinkedList:

template <class T>
using List = DLinkedList<T>;

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
DLinkedList<T>::DLinkedList(
        void (*deleteUserData)(DLinkedList<T> *),
        bool (*itemEqual)(T &, T &))
{
    this->deleteUserData = deleteUserData;
    this->itemEqual = itemEqual;
    this->count = 0;

    // initialize for head
    this->head = new Node();
    this->head->prev = nullptr;

    //initialize for tail
    this->tail = new Node();
    this->tail->next = nullptr;

    // liên kết head và tail
    this->head->next = this->tail;
    this->tail->prev = this->head;
}

template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &list)
{
    // khởi tạo head
    this->head = new Node();
    this->head->prev = nullptr;
    // khởi tạo tail
    this->tail = new Node();
    this->tail->next = nullptr;
    // nối head và tail
    this->head->next = this->tail;
    this->tail->prev = this->head;

    //this->count = list.count; wtf???
    this->count = 0;

    this->deleteUserData = list.deleteUserData;
    this->itemEqual = list.itemEqual;

    Node *to_list = list.head->next;
    while (to_list != list.tail)
    {
        this->add(to_list->data);
        to_list = to_list->next;
    }
}

// assigning operator
template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list)
{
    // kiểm tra tự gán
    if (this == &list)
    {
        return *this;
    }

    this->clear();
    this->deleteUserData = list.deleteUserData;
    this->itemEqual = list.itemEqual;

    Node *current = list.head;
    while (current != list.tail)
    {
        this->add(current->data);
        current = current->next;
    }

    return *this;
}

template <class T>
DLinkedList<T>::~DLinkedList()
{
    this->removeInternalData();
    delete this->head;
    delete this->tail;
}


// add(e)
template <class T>
void DLinkedList<T>::add(T e)
{
    Node *new_node = new Node(e);
    new_node->prev = tail->prev;
    new_node->next = tail;
    tail->prev->next = new_node;
    tail->prev = new_node;

    this->count++;
}

// add(index, e)
template <class T>
void DLinkedList<T>::add(int index, T e)
{
    // dùng getPreviousNodeOf(index);
    if (index < 0 || index > this->size())
    {
        throw std::out_of_range("Index is out of range!");
    }
    if (index == 0 || this->size() == 0)
    {
        Node *new_node = new Node(e);
        new_node->next = this->head->next;
        this->head->next->prev = new_node;
        this->head->next = new_node;
        new_node->prev = this->head;
    }

    else
    {
        Node *pre = this->getPreviousNodeOf(index);
        //in ra trước xem cout << "pre data : " << pre->data << '\n';
        Node *new_node = new Node(e);
        new_node->next = pre->next;
        pre->next->prev = new_node;
        pre->next = new_node;
        new_node->prev = pre;
    }

    this->count++;
}

template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index)
{
    int k = (this->size() - 1) / 2;
    // cout << "k = " << k << '\n';
    Node *tmp = new Node();
    if (index <= k)
    {
        //chạy nửa đầu danh sách
        tmp = this->head;
        int l = 0;
        while (l < index - 1)
        {
            tmp = tmp->next;
            l++;
        }
        return tmp->next;
    }
    else
    {
        // chạy nửa sau danh sách
        tmp = this->tail;
        int r = this->count - 1;
        while (r > index - 1)
        {
            tmp = tmp->prev;
            r--;
        }
        return tmp->prev;
    }
}
/*
 * Author : Le Trong Tin
 * ID : 2313452
 */
template <class T>
T DLinkedList<T>::removeAt(int index)
{
    if (index < 0 || index >= this->size()) {
        throw std::out_of_range("Index is out of range!");
    }
    Node *tmp;
    if (this->size() == 1)
    {
        tmp = head->next;        // Node duy nhất giữa head và tail
        head->next = tail;
        tail->prev = head;
    }
    else if (index == 0)
    {
        if (this->count == 0) // trường hợp danh sách rỗng nối head và tail lại
        {
            head->next = tail;
            tail->prev = head;
        }
        else
        {
            tmp = head->next;
            head->next = tmp->next;
            tmp->next->prev = head;
        }

    }
    else
    {
        Node *pre = this->getPreviousNodeOf(index);
        tmp = pre->next;
        pre->next = tmp->next;
        tmp->next->prev = pre;
    }

    T res = tmp->data;
    // deallocate memory of tmp
    delete tmp;

    this->count--;
    return res;
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T))
{
    Node* hientai=head->next;
    for(int i=0; i<this->size(); i++){
        if (equals(hientai->data, item, itemEqual)) {
            Node* tuonglai=hientai->next;
            Node* quakhu=hientai->prev;
            (hientai->prev)->next=tuonglai;
            (hientai->next)->prev=quakhu;
            if (removeItemData!= nullptr) removeItemData(hientai->data);
            if(hientai!=nullptr) delete hientai;
            count--;
            return true;
        }
        hientai=hientai->next;
    }
    return false;
}

template <class T>
bool DLinkedList<T>::empty()
{
    return (this->count == 0);
}

template <class T>
int DLinkedList<T>::size()
{
    return this->count;
}

template <class T>
void DLinkedList<T>::clear()
{
    if (this->size() == 0) return;
    if (deleteUserData != nullptr)
    {
        deleteUserData(this);
    }
    Node *tmp = head->next;
    while (tmp != tail)
    {
        Node *delete_node = tmp;
        tmp = tmp->next;
        delete delete_node;
    }
    head->next = tail;
    tail->prev = head;
    count = 0;
}

template <class T>
T &DLinkedList<T>::get(int index)
{
    if (index < 0 || index >= this->size())
    {
        throw std::out_of_range("Index is out of range!");
    }
    Node *tmp = this->head->next;
    int indx = 0;
    while (indx != index)
    {
        tmp = tmp->next;
        indx++;
    }
    return tmp->data;
}

template <class T>
int DLinkedList<T>::indexOf(T item)
{
    if (!contains(item)) return -1;
    int res = 0;
    for (DLinkedList<T>::Iterator it = this->begin(); it != this->end(); it++, res++)
    {
        if (equals(*it, item, itemEqual))
        {
            return res;
        }
    }
    return -1;
}

template <class T>
bool DLinkedList<T>::contains(T item)
{
    for (DLinkedList<T>::Iterator it = this->begin(); it != this->end(); it++)
    {
        if (equals(*it, item, itemEqual))
            return true;
    }
    return false;
}

template <class T>
string DLinkedList<T>::toString(string (*item2str)(T &))
{
    stringstream ss;
    ss << "[";
    if (item2str != nullptr)
    {
        Node *tmp = this->head->next;
        int dem = 0;
        while (dem != this->count)
        {
            ss << item2str(tmp->data);
            if (dem != this->count - 1) ss << ", ";
            tmp = tmp->next;
            dem++;
        }
    }
    else
    {
        Node *tmp = this->head->next;
        int dem = 0;
        while (dem != this->count)
        {
            ss << tmp->data;
            if (dem != this->count - 1) ss << ", ";
            tmp = tmp->next;
            dem++;
        }
    }

    ss << "]";
    return ss.str();
}

template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T> &list)
{
    Node *tmp = list.head->next;
    while (tmp != list.tail)
    {
        this->add(tmp->data);
        tmp = tmp->next;
    }
}

template <class T>
void DLinkedList<T>::removeInternalData()
{
    if (deleteUserData)
    {
        deleteUserData(this);
    }
    Node *tmp = head->next;
    while (tmp != tail)
    {
        Node *deleteNode = tmp;
        tmp = tmp->next;
        delete deleteNode;
    }
    this->head->next = tail;
    this->tail->prev = head;
    this->count = 0;
}

#endif /* DLINKEDLIST_H */
