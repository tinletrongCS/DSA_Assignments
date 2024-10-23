// XArayList cua BTL1
/*
* Faculity of Computer Science and Engineering
* Author: Le Trong Tin
* ID: 2313452
* Date: October 6th 2024
*/
/*
 * File:   XArrayList.h
 */
// FILE MỚI CHỈNH ĐỊNH DẠNG
// file chỉnh ngày 09 tháng 10
#ifndef XARRAYLIST_H
#define XARRAYLIST_H
//#include "list/IList.h"
#include "IList.h"
#include <memory.h>
#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;

template <class T>
class XArrayList : public IList<T>
{
public:
    class Iterator; // forward declaration

protected:
    T *data;                                 // dynamic array to store the list's items
    int capacity;                            // size of the dynamic array
    int count;                               // number of items stored in the array
    bool (*itemEqual)(T &lhs, T &rhs);       // function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(XArrayList<T> *); // function pointer: be called to remove items (if they are pointer type)

public:
    XArrayList
            (
                    void (*deleteUserData)(XArrayList<T> *) = 0,
                    bool (*itemEqual)(T &, T &) = 0,
                    int capacity = 10
            );

    XArrayList(const XArrayList<T> &list);

    XArrayList<T> &operator=(const XArrayList<T> &list);

    ~XArrayList();

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
    // Inherit from IList: BEGIN

    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(XArrayList<T> *) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    Iterator begin()
    {
        return Iterator(this, 0);
    }
    Iterator end()
    {
        return Iterator(this, count);
    }

    /** free:
     * if T is pointer type:
     *     pass THE address of method "free" to XArrayList<T>'s constructor:
     *     to:  remove the user's data (if needed)
     * Example:
     *  XArrayList<Point*> list(&XArrayList<Point*>::free);
     *  => Destructor will call free via function pointer "deleteUserData"
     */
    static void free(XArrayList<T> *list)
    {
        typename XArrayList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }

protected:
    void checkIndex(int index);     // check validity of index for accessing
    void ensureCapacity(int index); // auto-allocate if needed

    /** equals:
     * if T: primitive type:
     *      indexOf, contains: will use native operator ==
     *      to: compare two items of T type
     * if T: object type:
     *      indexOf, contains: will use native operator ==
     *      to: compare two items of T type
     *      Therefore, class of type T MUST override operator ==
     * if T: pointer type:
     *      indexOf, contains: will use function pointer "itemEqual"
     *      to: compare two items of T type
     *      Therefore:
     *      (1): must pass itemEqual to the constructor of XArrayList
     *      (2): must define a method for comparing
     *           the content pointed by two pointers of type T
     *          See: definition of "equals" of class Point for more detail
     */
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0) // Không có hàm so sánh tùy chỉnh
            return lhs == rhs;
        else // Có hàm so sânh tùy chỉnh
            return itemEqual(lhs, rhs);
        // Gọi hàm so sánh tùy chỉnh
    }

    void copyFrom(const XArrayList<T> &list);

    void removeInternalData();

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    // Iterator: BEGIN
    ////////////////////////////////////
    class Iterator
    {
    private:
        int cursor;
        XArrayList<T> *pList;

    public:
        Iterator(XArrayList<T> *pList = 0, int index = 0)
        {
            this->pList = pList;
            this->cursor = index;
        }
        Iterator &operator=(const Iterator &iterator)
        {
            cursor = iterator.cursor;
            pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            T item = pList->removeAt(cursor);
            if (removeItemData != 0)
                removeItemData(item);
            cursor -= 1; // MUST keep index of previous, for ++ later
        }

        T &operator*() // Toán tử giải tham khảo lấy dữ liệu là cursor
        {
            return pList->data[cursor];
        }
        bool operator!=(const Iterator &iterator)
        {
            return cursor != iterator.cursor;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            this->cursor++;
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
    // Iterator: END
};

//////////////////////////////////////////////////////////////////////
////////////////////////     METHOD DEFNITION      ///////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
XArrayList<T>::XArrayList(
        void (*deleteUserData)(XArrayList<T> *),
        bool (*itemEqual)(T &, T &),
        int capacity)
{
    this->deleteUserData = deleteUserData;
    this->itemEqual = itemEqual;
    this->capacity = capacity;
    this->count = 0;
    this->data = new T[capacity];
}

template <class T>
void XArrayList<T>::copyFrom(const XArrayList<T> &list)
{
    if (this == &list) return; // tránh việc gán cho chính nó

    T* new_data = nullptr;
    // thử trước
    try
    {
        new_data = new T[list.capacity];
        for (int i = 0; i < list.capacity; i++)
        {
            new_data[i] = list.data[i];
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    delete[] this->data;

    this->capacity = list.capacity;
    this->count = list.count;
    this->deleteUserData = list.deleteUserData;
    this->itemEqual = list.itemEqual;
    this->data = new_data;
}

template <class T>
void XArrayList<T>::removeInternalData()
{
    /*
     * Clears the internal data of the list by deleting the dynamic array and any user-defined data.
     * If a custom deletion function is provided, it is used to free the stored elements.
     * Finally, the dynamic array itself is deallocated from memory.
     */
    if (deleteUserData)
    {
        deleteUserData(this);
    }
    if (data)
    {
        delete[] this->data;
    }
    this->count = 0;
    this->capacity = 0;
    this->data = nullptr;
}

//Khởi tạo một list có tham số truyền vào là một list khác
template <class T>
XArrayList<T>::XArrayList(const XArrayList<T> &list)
{
    this->data = nullptr;
    copyFrom(list);
}

template <class T>
XArrayList<T> &XArrayList<T>::operator=(const XArrayList<T> &list_in)
{
    if (this == &list_in) // so sánh địa chỉ để tránh gán cho chính nó
    {
        this->copyFrom(list_in);
    }
    return *this;
}


template <class T>
XArrayList<T>::~XArrayList()
{
    if (deleteUserData != nullptr)
    {
        deleteUserData(this);
    }
    delete[] data;
}

template <class T>
void XArrayList<T>::add(T e)
{
    if (this->count == capacity)
    {
        ensureCapacity(count);
    }
    this->data[count] = e;
    this->count++;
}

template <class T>
void XArrayList<T>::add(int index, T e)
{
    if (this->count == capacity)
    {
        ensureCapacity(count);
    }
    checkIndex(index);
    for (int i = this->size() ; i > index; i--)
    {
        this->data[i] = this->data[i - 1];
    }

    this->data[index] = e;
    this->count++;
}

template <class T>
T XArrayList<T>::removeAt(int index)
{
    checkIndex(index);
    T tmp_val = this->data[index];
    for (int i = index; i < this->count - 1; i++)
    {
        this->data[i] = this->data[i + 1];
    }
    this->data[this->count - 1] = T();
    this->count--;
    return tmp_val;
}

template <class T>
bool XArrayList<T>::removeItem(T item, void (*removeItemData)(T))
{
    if (!contains(item)) return false;

    int index = 0;
    for (XArrayList<T>::Iterator it = this->begin(); it != this->end(); ++it, ++index)
    {
        if (equals(*it, item, itemEqual))
        {
            if (removeItemData != nullptr)
            {
                removeItemData(*it);
            }

            for (int i = index; i < this->count - 1; i++)
            {
                this->data[i] = this->data[i + 1];
            }

            this->data[this->count - 1] = T();
            this->count--;
            return true;
        }
    }
    return false;
}

template <class T>
bool XArrayList<T>::empty()
{
    return (this->size() == 0);
}

template <class T>
int XArrayList<T>::size()
{
    return this->count;
}

template <class T>
void XArrayList<T>::clear()
{
    if (deleteUserData)
    {
        deleteUserData(this);
    }
    for (int i = 0; i < this->size(); i++)
    {
        data[i] = T(); // set trạng thái mỗi node về mặc định
    }
    count = 0;
}

template <class T>
T &XArrayList<T>::get(int index)
{
    checkIndex(index);
    return this->data[index];
}

template <class T>
int XArrayList<T>::indexOf(T item)
{
    int count = 0;
    for (XArrayList<T>::Iterator it = this->begin(); it != this->end(); it++, count++)
    {
        if (equals(*it, item, this->itemEqual))
            return count;
    }
    return -1;
}

template <class T>
bool XArrayList<T>::contains(T item)
{
    for (XArrayList<T>::Iterator it = this->begin(); it != end(); it++)
    {
        if (equals(*it, item,this->itemEqual))
            return true;
    }
    return false;
}
template <class T>
string XArrayList<T>::toString(string (*item2str)(T &))
{
    stringstream ss;
    ss << "[";
    if (item2str != nullptr)
    {
        int c = 0;
        for (XArrayList<T>::Iterator it = this->begin(); it != this->end(); it++, c++)
        {
            ss << item2str(*it);
            if (c != this->count - 1) ss << ", ";
        }
    }
    else
    {
        for (int i = 0; i < this->count; i++)
        {
            ss << data[i];
            if (i != this->count - 1) ss << ", ";
        }
    }
    ss << "]";
    return ss.str();
}

template <class T>
void XArrayList<T>::checkIndex(int index)
{
    if (index < 0 || index >= this->capacity)
    {
        throw std::out_of_range("Index is out of range!");
    }
}
template <class T>
void XArrayList<T>::ensureCapacity(int index)
{
    if (size() >= this->capacity)
    {
        int new_capa = this->capacity ;
        while (new_capa <= index)
        {
            new_capa *= 2;
        }

        try
        {
            T *new_data = new T[new_capa];

            // copy từ data cũ qua
            for (int i = 0; i < this->size(); i++)
            {
                new_data[i] = this->data[i];
            }

            // xóa data cũ sau đó gán lại cho data bằng new_data
            delete[] this->data;
            this->data = new_data;
            this->capacity = new_capa;
        }
        catch(const std::bad_alloc &e)
        {
            cerr << e.what();
            throw;
        }
    }
}

#endif /* XARRAYLIST_H */
