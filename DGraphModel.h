/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   DGraphModel.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 19:36
 */

#ifndef DGRAPHMODEL_H
#define DGRAPHMODEL_H
#include "graph/AbstractGraph.h"
#include "AbstractGraph.h"
//////////////////////////////////////////////////////////////////////
///////////// GraphModel: Directed Graph Model    ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class DGraphModel : public AbstractGraph<T>
{
private:
public:
    DGraphModel(
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &)) : AbstractGraph<T>(vertexEQ, vertex2str)
    {
    }

    // connect OK
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
        nodeFrom->connect(nodeTo, weight);
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
        
        typename AbstractGraph<T>::VertexNode *fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *toNode = this->getVertexNode(to);

        // Lấy cạnh nối từ from tới to
        typename AbstractGraph<T>::Edge *edge = fromNode->getEdge(toNode);

        if (edge == nullptr)
        {
            typename AbstractGraph<T>::Edge tempEdge(this->getVertexNode(from), this->getVertexNode(to));
            throw EdgeNotFoundException(AbstractGraph<T>::edge2Str(tempEdge));
        }

        fromNode->removeTo(toNode);
    }

    void remove(T vertex)
    {
        // Xóa một đỉnh và tất cả các cạnh liên quan đến đỉnh đó
        typename AbstractGraph<T>::VertexNode *nodeToDelete= this->getVertexNode(vertex);

        if (nodeToDelete == nullptr)
        {
            throw VertexNotFoundException(AbstractGraph<T>::vertex2str(vertex));
        }

        for (auto it : this->nodeList)
        {
            typename AbstractGraph<T>::VertexNode* pVertex = it;
            nodeToDelete->removeTo(pVertex);
            pVertex->removeTo(nodeToDelete);
        }
        
        this->nodeList.removeItem(nodeToDelete);

        delete nodeToDelete;
    }

    static DGraphModel<T> *create(T *vertices, int nvertices, Edge<T> *edges, int nedges, bool (*vertexEQ)(T &, T &), string (*vertex2str)(T &))
    {
        // *vertices là mảng các vertices
        DGraphModel<T> *new_graph = new DGraphModel<T>(vertexEQ, vertex2str);
        for (int i = 0; i < nvertices; i++)
        {
            new_graph->add(vertices[i]);
        }
        for (int i = 0; i < nedges; i++)
        {
            new_graph->connect(edges[i].from, edges[i].to, edges[i].weight);
        }

        return new_graph;
    }
};

#endif /* DGRAPHMODEL_H */
