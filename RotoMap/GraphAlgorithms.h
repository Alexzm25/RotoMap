#pragma once
#include "PathResult.h"
#include "Graph.h"

class GraphAlgorithms
{
public:
    static PathResult dijkstra(Graph* graph, const QString& start, const QString& end);
    static PathResult floydWarshall(Graph* graph, const QString& start, const QString& end);
    static TreeResult prim(Graph* graph);
    static TreeResult kruskal(Graph* graph);
    
private:
    static QString findSet(const QString& vertex, QMap<QString, QString>& parent);
    static void unionSets(const QString& u, const QString& v, QMap<QString, QString>& parent, QMap<QString, int>& rank);
};
