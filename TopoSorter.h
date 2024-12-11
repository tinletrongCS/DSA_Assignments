/**
 * File : TopoSorter.h
 * Người thực hiện : Lê Trọng Tín
 * MSSV : 2313452
 * Ngày thực hiện : 11 tháng 12, 2024
 */

#ifndef TOPOSORTER_H
#define TOPOSORTER_H
#include "graph/DGraphModel.h"
#include "hash/xMap.h"
#include "list/DLinkedList.h"
#include "sorting/DLinkedListSE.h"
#include "stacknqueue/Queue.h"
#include "stacknqueue/Stack.h"

template <class T, class V>
using XHashMap = xMap<T, V>;
/*
 ! build code graph :
 g++ -fsanitize=address -fsanitize=undefined -std=c++17 -o main -Iinclude -Itest main.cpp test/unit_test/sort_topo/unit_test.cpp test/unit_test/sort_topo/test/*.cpp  -DTEST_SORT_TOPO

  ! build code topo : g++ -fsanitize=address -fsanitize=undefined -std=c++17 -o
 main -Iinclude -Itest main.cpp
 test/unit_test/graph/unit_test.cpptest/unit_test/graph/test/*.cpp  -DTEST_GRAPH
 */
template <class T>
class TopoSorter
{
public:
    static int DFS;
    static int BFS;

protected:
    DGraphModel<T> *graph;
    int (*hash_code)(T &, int);

public:
    TopoSorter(DGraphModel<T> *graph, int (*hash_code)(T &, int) = 0)
    {
        this->graph = graph;
        this->hash_code = hash_code;
    }

    DLinkedList<T> sort(int mode = 0, bool sorted = true)
    {
        return (mode == BFS) ? bfsSort(sorted) : dfsSort(sorted);
    }

    DLinkedList<T> dfsSort(bool sorted = true)
    {
        xMap<T, bool> visited(hash_code);
        Stack<T> resultStack;
        DLinkedList<T> result;

        // Lấy danh sách các đỉnh từ đồ thị
        DLinkedListSE<T> vertices = graph->vertices();

        if (sorted)
        {
            vertices.sort();
        }

        // Đánh dấu các đỉnh là chưa thăm
        for (T vertex : vertices)
        {
            visited.put(vertex, false);
        }
        for (T vertex : vertices)
        {
            if (!visited.get(vertex))
            {
                dfsIterative(vertex, visited, resultStack);
            }
        }

        while (!resultStack.empty())
        {
            result.add(resultStack.peek());
            resultStack.pop();
        }
        return result;
    }

    void dfsIterative(T start, xMap<T, bool> &visited, Stack<T> &resultStack)
    {
        Stack<T> stack;
        stack.push(start);

        while (!stack.empty())
        {
            T u = stack.peek();

            if (!visited.get(u))
            {
                visited.put(u, true); // Đánh dấu đỉnh đã thăm
            }

            bool allNeighborsVisited = true;

            for (T neighbor : this->graph->getOutwardEdges(u))
            {
                if (!visited.get(neighbor))
                {
                    stack.push(neighbor);
                    allNeighborsVisited = false;
                    break;
                }
            }

            if (allNeighborsVisited)
            {
                stack.pop();
                resultStack.push(u);
            }
        }
    }

    DLinkedList<T> bfsSort(bool sorted = true)
    {
        xMap<T, int> inDegree(hash_code);
        DLinkedListSE<T> vertices = this->graph->vertices();
        if (sorted)
        {
            vertices.sort();
        }

        for (T vertex : vertices)
        {
            inDegree.put(vertex, 0); // Khởi tạo in-degree = 0 cho tất cả các đỉnh
        }

        for (T vertex : vertices)
        {
            for (T neighbor : graph->getOutwardEdges(vertex))
            {
                inDegree.put(neighbor, inDegree.get(neighbor) + 1); // Tăng in-degree cho đỉnh kề
            }
        }

        Queue<T> queue;
        for (T vertex : vertices)
        {
            if (inDegree.get(vertex) == 0)
            {
                queue.push(vertex);
            }
        }

        DLinkedList<T> result;
        while (!queue.empty())
        {
            T node = queue.peek();
            queue.pop();
            result.add(node);

            for (T neighbor : graph->getOutwardEdges(node))
            {
                inDegree.put(neighbor, inDegree.get(neighbor) - 1); // Giảm in-degree của đỉnh kề
                if (inDegree.get(neighbor) == 0)
                {
                    queue.push(neighbor); // Đưa đỉnh vào hàng đợi nếu in-degree = 0
                }
            }
        }

        return result;
    }

protected:
    // Helper functions
    XHashMap<T, int> vertex2inDegree(int (*hash)(T &, int))
    {
        XHashMap<T, int> inDegreeMap;
        for (auto vertex : this->graph->nodeList)
        {
            int index = hash(vertex->getVertex(), this->inDegreeMap.capacity);
            (inDegreeMap.table[index]).add(vertex->inDegree()); // Khởi tạo bậc vào là 0
        }
        return inDegreeMap;
    };
    XHashMap<T, int> vertex2outDegree(int (*hash)(T &, int))
    {
        XHashMap<T, int> outDegreeMap;
        for (auto vertex : this->graph->nodeList)
        {
            int index = hash(vertex->getVertex(), this->outDegreeMap.capacity);
            (outDegreeMap.table[index]).add(vertex->outDegree()); // Khởi tạo bậc vào là 0
        }
        return outDegreeMap;
    };
    DLinkedList<T> listOfZeroInDegrees()
    {
        DLinkedList<T> zeroInDegreeList;
        for (auto vertex : this->graph->nodeList)
        {
            int inDegree1 = vertex->inDegree();
            if (inDegree1 == 0)
                zeroInDegreeList.add(vertex->getVertex());
        }
        return zeroInDegreeList;
    };
}; // TopoSorter
template <class T>
int TopoSorter<T>::DFS = 0;
template <class T>
int TopoSorter<T>::BFS = 1;

/////////////////////////////End of TopoSorter//////////////////////////////////

#endif /* TOPOSORTER_H */