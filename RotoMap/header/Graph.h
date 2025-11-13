#pragma once
#include <QMap>
#include <QVector>
#include <QPointF>
#include "Vertex.h"
#include "Edge.h"

class Graph
{
public:
    Graph();
    
    void addVertex(const QString& name, const QPointF& position);
    void removeVertex(const QString& name);
    void addEdge(const QString& from, const QString& to, double weight);
    void removeEdge(const QString& from, const QString& to);
    
    bool hasVertex(const QString& name) const;
    Vertex* getVertex(const QString& name);
    QVector<Vertex*> getVertices() const;
    QVector<Edge*> getEdges() const;
    QVector<Edge*> getEdgesFrom(const QString& vertexName) const;
    Edge* getEdge(const QString& from, const QString& to) const;
    
    void clear();
    int getVertexCount() const;
    int getEdgeCount() const;
    
private:
    QMap<QString, Vertex*> m_vertices;
    QVector<Edge*> m_edges;
};
