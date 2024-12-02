/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   UGraphModel.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 15:16
 */

#ifndef UGRAPHMODEL_H
#define UGRAPHMODEL_H

#include "graph/AbstractGraph.h"
// #include "AbstractGraph.h"
//////////////////////////////////////////////////////////////////////
///////////// UGraphModel: Undirected Graph Model ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class UGraphModel : public AbstractGraph<T>
{
private:
public:
    // class UGraphAlgorithm;
    // friend class UGraphAlgorithm;

    UGraphModel(
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }

    void connect(T from, T to, float weight = 0)
    {
        if (!this->contains(from))
        {
            throw VertexNotFoundException(AbstractGraph<T>::vertex2str(from));
        }
        if (!this->contains(to))
        {
            throw VertexNotFoundException(AbstractGraph<T>::vertex2str(to));
        }

        typename AbstractGraph<T>::VertexNode *nodeFrom = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *nodeTo = this->getVertexNode(to);

        // nêu đỉnh from và to trùng nhau
        if (nodeFrom->equals(nodeTo))
        {
            nodeFrom->connect(nodeFrom, weight);
        }

        else
        {
            nodeFrom->connect(nodeTo, weight);
            nodeTo->connect(nodeFrom, weight);
        }
    }
    void disconnect(T from, T to)
    {
        if (!this->contains(from))
        {
            throw VertexNotFoundException(AbstractGraph<T>::vertex2str(from));
        }
        if (!this->contains(to))
        {
            throw VertexNotFoundException(AbstractGraph<T>::vertex2str(to));
        }

        typename AbstractGraph<T>::VertexNode *nodeFrom = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *nodeTo = this->getVertexNode(to);

        // Lấy cạnh nối từ from tới to
        typename AbstractGraph<T>::Edge *edge = nodeFrom->getEdge(nodeTo);

        if (edge == nullptr)
        {
            typename AbstractGraph<T>::Edge tempEdge(this->getVertexNode(from), this->getVertexNode(to));
            throw EdgeNotFoundException(AbstractGraph<T>::edge2Str(tempEdge));
        }

        if (nodeFrom->equals(nodeTo))
        {
            nodeFrom->removeTo(nodeTo);
        }
        else
        {
            nodeFrom->removeTo(nodeTo);
            nodeTo->removeTo(nodeFrom);
        }
    }
    void remove(T vertex)
    {
        // Xóa một đỉnh và tất cả các cạnh liên quan đến đỉnh đó
        typename AbstractGraph<T>::VertexNode *nodeToDelete = this->getVertexNode(vertex);

        if (nodeToDelete == nullptr)
        {
            throw VertexNotFoundException(AbstractGraph<T>::vertex2str(vertex));
        }

        for (auto it : this->nodeList)
        {
            typename AbstractGraph<T>::VertexNode *pVertex = it;
            if (this->connected(vertex, pVertex->getVertex()))
            {
                nodeToDelete->removeTo(pVertex);
            }

            if (this->connected(pVertex->getVertex(), vertex))
            {
                pVertex->removeTo(nodeToDelete);
            }
        }

        this->nodeList.removeItem(nodeToDelete);

        delete nodeToDelete;
    }
    static UGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        // *vertices là mảng các vertices
        UGraphModel<T> *new_graph = new UGraphModel<T>(vertexEQ, vertex2str);
        for (int i = 0; i < nvertices; i++)
        {
            new_graph->add(vertices[i]);
        }
        for (int i = 0; i < nedges; i++)
        {
            new_graph->connect(edges[i].from, edges[i].to, edges[i].weight);
            new_graph->connect(edges[i].to, edges[i].from, edges[i].weight);
        }

        return new_graph;
    }
};

#endif /* UGRAPHMODEL_H */
