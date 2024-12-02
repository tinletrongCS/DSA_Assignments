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


    // connect OKK
    void connect(T from, T to, float weight = 0)
    {
        if (!this->contains(from))
        {
        }
        if (!this->contains(to))
        {
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
            //...
        }
        if (!this->contains(to))
        {
            //...
        }

        typename AbstractGraph<T>::VertexNode *nodeFrom = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *nodeTo = this->getVertexNode(to);

        // Lấy cạnh nối từ from tới to
        typename AbstractGraph<T>::Edge *edge = nodeFrom->getEdge(nodeTo);

        if (edge == nullptr)
        {
            // chưa biết throw sao cả...
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
        // TODO
    }
    static UGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        // TODO
    }
};

#endif /* UGRAPHMODEL_H */
