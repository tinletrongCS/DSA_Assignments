/*
 * File:   AbstractGraph.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 17:48
 */
/**
 * Người thực hiện : LÊ TRỌNG TÍN
 * MSSV : 2313452
 */
#ifndef ABSTRACTGRAPH_H
#define ABSTRACTGRAPH_H
#include "graph/IGraph.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

template <class T>
class AbstractGraph : public IGraph<T>
{
public:
    class Edge;       // forward declaration
    class VertexNode; // forward declaration
    class Iterator;   // forward declaration

private:
protected:
    // Using the adjacent list technique, so need to store list of nodes (nodeList)
    DLinkedList<VertexNode *> nodeList;

    // Function pointers:
    bool (*vertexEQ)(T &, T &); // to compare two vertices
    string (*vertex2str)(T &);  // to obtain string representation of vertices

    VertexNode *getVertexNode(T &vertex)
    {
        typename DLinkedList<VertexNode *>::Iterator it = nodeList.begin();
        while (it != nodeList.end())
        {
            VertexNode *node = *it;
            if (vertexEQ(node->vertex, vertex))
                return node;
            it++;
        }
        return 0;
    }

    string vertex2Str(VertexNode &node)
    {
        return vertex2str(node.vertex);
    }

    string edge2Str(Edge &edge)
    {
        stringstream os;
        os << "E("
           << vertex2str(edge.from->vertex)
           << ","
           << vertex2str(edge.to->vertex)
           << ")";
        return os.str();
    }

public:
    AbstractGraph(
        bool (*vertexEQ)(T &, T &) = 0,
        string (*vertex2str)(T &) = 0)
    {

        this->vertexEQ = vertexEQ;
        this->vertex2str = vertex2str;
    }

    virtual ~AbstractGraph() {}

    typedef bool (*vertexEQFunc)(T &, T &);

    typedef string (*vertex2strFunc)(T &);

    vertexEQFunc getVertexEQ()
    {
        return this->vertexEQ;
    }
    vertex2strFunc getVertex2Str()
    {
        return this->vertex2str;
    }

    //////////////////////////////////////////////////////////////////////
    //////////////////// IMPLEMENTATION of IGraph API ////////////////////
    //////////////////////////////////////////////////////////////////////
    /* connect, disconnect, and remove: will be overridden in:
     *  >> UGraphModel
     *  >> DGraphModel
     */
    virtual void connect(T from, T to, float weight = 0) = 0;

    virtual void disconnect(T from, T to) = 0;

    virtual void remove(T vertex) = 0;

    /* The following are common methods for UGraphModel and DGraphModel
     */
    virtual void add(T vertex)
    {
        if (!this->contains(vertex))
        {
            VertexNode *newVertex = new VertexNode(vertex, this->vertexEQ, this->vertex2str);
            this->nodeList.add(newVertex);
        }
    }

    virtual bool contains(T vertex)
    {
        for (typename DLinkedList<VertexNode *>::Iterator it = this->nodeList.begin(); it != this->nodeList.end(); it++)
        {
            VertexNode *pVertex = *it;
            if (vertexEQ(vertex, pVertex->getVertex()))
                return true;
        }
        return false;
    }

    virtual float weight(T from, T to)
    {
        if (!this->getVertexNode(from))
        {
            throw VertexNotFoundException(vertex2str(from));
        }

        if (!this->getVertexNode(to))
        {
            throw VertexNotFoundException(vertex2str(to));
        }

        VertexNode *node = this->getVertexNode(from);
        for (typename DLinkedList<Edge *>::Iterator it = node->adList.begin(); it != node->adList.end(); it++)
        {
            Edge *pEdge = *it;
            if (vertexEQ(pEdge->to->getVertex(), to))
            {
                return pEdge->weight;
            }
        }

        Edge tempEdge(getVertexNode(from), getVertexNode(to));
        throw EdgeNotFoundException(edge2Str(tempEdge));

        return 0;
    }

    virtual DLinkedList<T> getOutwardEdges(T from)
    {
        if (!this->getVertexNode(from))
        {
            throw VertexNotFoundException(vertex2str(from));
        }

        VertexNode *node = this->getVertexNode(from);
        return node->getOutwardEdges();
    }

    virtual DLinkedList<T> getInwardEdges(T to)
    {
        DLinkedList<T> inwardEdges;

        if (!this->getVertexNode(to))
        {
            throw VertexNotFoundException(vertex2str(to));
        }

        for (typename DLinkedList<VertexNode *>::Iterator it = nodeList.begin(); it != nodeList.end(); it++)
        {
            VertexNode *pVertex = *it;
            for (typename DLinkedList<Edge *>::Iterator it1 = pVertex->adList.begin(); it1 != pVertex->adList.end(); it1++)
            {
                Edge *pEdge = *it1;
                if (this->vertexEQ(pEdge->to->vertex, to))
                {
                    inwardEdges.add(pEdge->from->getVertex());
                }
            }
        }

        return inwardEdges;
    }

    virtual int size()
    {
        return this->nodeList.size();
    }

    virtual bool empty()
    {
        return this->nodeList.size() == 0;
    };

    virtual void clear()
    {
        for (typename DLinkedList<VertexNode *>::Iterator it = this->nodeList.begin(); it != this->nodeList.end(); it++)
        {
            VertexNode *pVertex = *it;
            pVertex->adList.clear();
            delete pVertex;
        }
        this->nodeList.clear();
    }

    virtual int inDegree(T vertex)
    {
        if (!this->contains(vertex))
        {
            throw VertexNotFoundException(vertex2str(vertex));
        }
        VertexNode *node = this->getVertexNode(vertex);
        return node->inDegree();
    }

    virtual int outDegree(T vertex)
    {
        if (!this->contains(vertex))
        {
            throw VertexNotFoundException(vertex2str(vertex));
        }
        VertexNode *node = this->getVertexNode(vertex);
        return node->outDegree();
    }

    virtual DLinkedList<T> vertices()
    {
        DLinkedList<T> verticesList;
        for (typename DLinkedList<VertexNode *>::Iterator it = this->nodeList.begin(); it != this->nodeList.end(); it++)
        {
            VertexNode *pVertex = *it;
            verticesList.add(pVertex->getVertex());
        }
        return verticesList;
    }

    virtual bool connected(T from, T to)
    {
        if (!this->contains(from))
        {
            throw VertexNotFoundException(vertex2str(from));
        }

        if (!this->contains(to))
        {
            throw VertexNotFoundException(vertex2str(to));
        }

        VertexNode *node = this->getVertexNode(from);

        for (typename DLinkedList<Edge *>::Iterator it = node->adList.begin(); it != node->adList.end(); it++)
        {
            Edge *pEdge = *it;
            if (vertexEQ(pEdge->to->getVertex(), to))
            {
                return true;
            }
        }

        return false;
    }

    void println()
    {
        cout << this->toString() << endl;
    }

    virtual string toString()
    {
        string mark(50, '=');
        stringstream os;
        os << mark << endl;
        os << left << setw(12) << "Vertices:" << endl;
        typename DLinkedList<VertexNode *>::Iterator nodeIt = nodeList.begin();
        while (nodeIt != nodeList.end())
        {
            VertexNode *node = *nodeIt;
            os << node->toString() << endl;
            nodeIt++;
        }
        string mark2(30, '-');
        os << mark2 << endl;

        os << left << setw(12) << "Edges:" << endl;

        nodeIt = nodeList.begin();
        while (nodeIt != nodeList.end())
        {
            VertexNode *node = *nodeIt;

            typename DLinkedList<Edge *>::Iterator edgeIt = node->adList.begin();
            while (edgeIt != node->adList.end())
            {
                Edge *edge = *edgeIt;
                os << edge->toString() << endl;

                edgeIt++;
            }

            nodeIt++;
        }
        os << mark << endl;
        return os.str();
    }

    /* begin, end: will be used to traverse on graph's vertices
     * example: assume that "graph" is a graph.
     * Code:
     *      AbstractGraph<T>::Iterator it;
     *      for(it = graph.begin(); it != graph.end(); it++){
     *          T vertex = *it;
     *          //continue to process vertex here!
     *      }
     */

    Iterator begin()
    {
        return Iterator(this, true);
    }

    Iterator end()
    {
        return Iterator(this, false);
    }

    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////

public:
    // BEGIN of VertexNode
    class VertexNode
    {
    private:
        template <class U>
        friend class UGraphModel; // UPDATED: added
        T vertex;
        int inDegree_, outDegree_;
        DLinkedList<Edge *> adList;

        friend class Edge;

        friend class AbstractGraph;

        bool (*vertexEQ)(T &, T &);

        string (*vertex2str)(T &);

    public:
        VertexNode() : adList(&DLinkedList<Edge *>::free, &Edge::edgeEQ) {}

        VertexNode(T vertex, bool (*vertexEQ)(T &, T &), string (*vertex2str)(T &))
            : adList(&DLinkedList<Edge *>::free, &Edge::edgeEQ)
        {
            this->vertex = vertex;
            this->vertexEQ = vertexEQ;
            this->vertex2str = vertex2str;
            this->outDegree_ = this->inDegree_ = 0;
        }

        T &getVertex()
        {
            return this->vertex;
        }

        void connect(VertexNode *to, float weight = 0)
        {
            // Trường hợp đã kết nối với to rồi nhưng kết nối lại với trọng số khác
            // chỉ cập nhật lại trọng số khác
            if (this->getEdge(to) != nullptr)
            {
                this->getEdge(to)->setWeight(weight);
                return;
            }

            Edge *new_edge = new Edge(this, to, weight);
            this->adList.add(new_edge);

            // Đỉnh được nối tới thêm 1 bậc vào
            to->inDegree_++;

            // Đỉnh hiện tại thêm 1 bậc ra
            this->outDegree_++;
        }

        DLinkedList<T> getOutwardEdges()
        {
            DLinkedList<T> outwardEdgesList;
            for (typename DLinkedList<Edge *>::Iterator it = this->adList.begin(); it != this->adList.end(); it++)
            {
                Edge *pEdge = *it;
                outwardEdgesList.add(pEdge->to->getVertex());
            }
            return outwardEdgesList;
        }

        Edge *getEdge(VertexNode *to)
        {
            for (typename DLinkedList<Edge *>::Iterator it = this->adList.begin(); it != this->adList.end(); it++)
            {
                Edge *pEdge = *it;
                if (pEdge->to->equals(to))
                {
                    return pEdge;
                }
            }
            return nullptr;
        }

        bool equals(VertexNode *vertex_node)
        {
            return this->vertex == vertex_node->getVertex();
        }

        void removeTo(VertexNode *to)
        {
            if (to == nullptr)
                return;
            for (typename DLinkedList<Edge *>::Iterator it = this->adList.begin(); it != this->adList.end(); it++)
            {
                Edge *pEdge = *it;
                if (pEdge == nullptr || pEdge->to == nullptr)
                    continue;
                if (pEdge->to->equals(to))
                {
                    this->adList.removeItem(pEdge);
                    this->outDegree_--;
                    pEdge->to->inDegree_--;
                    delete pEdge;
                    return;
                }
            }
        }

        int inDegree()
        {
            return this->inDegree_;
        }

        int outDegree()
        {
            return this->outDegree_;
        }

        string toString()
        {
            stringstream os;
            os << "V("
               << this->vertex << ", "
               << "in: " << this->inDegree_ << ", "
               << "out: " << this->outDegree_ << ")";
            return os.str();
        }
    };

    // END of VertexNode

    // BEGIN of Edge
    class Edge
    {
    private:
        VertexNode *from;
        VertexNode *to;
        float weight;

        friend class VertexNode;

        friend class AbstractGraph;

    public:
        Edge() {}

        Edge(VertexNode *from, VertexNode *to, float weight = 0)
        {
            this->from = from;
            this->to = to;
            this->weight = weight;
        }

        void setWeight(float weight)
        {
            this->weight = weight;
        }

        bool equals(Edge *edge)
        {
            return (this->from->equals(edge->from) && this->to->equals(edge->to));
        }

        static bool edgeEQ(Edge *&edge1, Edge *&edge2)
        {
            return edge1->equals(edge2);
        }

        string toString()
        {
            stringstream os;
            os << "E("
               << this->from->vertex
               << ","
               << this->to->vertex
               << ","
               << this->weight
               << ")";
            return os.str();
        }
    };
    // END of Edge

    // BEGIN of Iterator
public:
    class Iterator
    {
    private:
        typename DLinkedList<VertexNode *>::Iterator nodeIt;

    public:
        Iterator(AbstractGraph<T> *pGraph = 0, bool begin = true)
        {
            if (begin)
            {
                if (pGraph != 0)
                    nodeIt = pGraph->nodeList.begin();
            }
            else
            {
                if (pGraph != 0)
                    nodeIt = pGraph->nodeList.end();
            }
        }

        Iterator &operator=(const Iterator &iterator)
        {
            this->nodeIt = iterator.nodeIt;
            return *this;
        }

        T &operator*()
        {
            return (*nodeIt)->vertex;
        }

        bool operator!=(const Iterator &iterator)
        {
            return nodeIt != iterator.nodeIt;
        }

        // Prefix ++ overload
        Iterator &operator++()
        {
            nodeIt++;
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
    // END of Iterator
};

#endif /* ABSTRACTGRAPH_H */
