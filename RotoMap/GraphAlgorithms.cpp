#include "GraphAlgorithms.h"
#include <QSet>
#include <limits>
#include <algorithm>

PathResult GraphAlgorithms::dijkstra(Graph* graph, const QString& start, const QString& end)
{
    PathResult result;
    
    if (!graph->hasVertex(start) || !graph->hasVertex(end))
    {
        return result;
    }
    
    QMap<QString, double> distances;
    QMap<QString, QString> previous;
    QSet<QString> visited;
    
    for (Vertex* vertex : graph->getVertices())
    {
        distances[vertex->getName()] = std::numeric_limits<double>::infinity();
    }
    distances[start] = 0.0;
    
    while (visited.size() < graph->getVertexCount())
    {
        QString current;
        double minDistance = std::numeric_limits<double>::infinity();
        
        for (Vertex* vertex : graph->getVertices())
        {
            QString name = vertex->getName();
            if (!visited.contains(name) && distances[name] < minDistance)
            {
                minDistance = distances[name];
                current = name;
            }
        }
        
        if (minDistance == std::numeric_limits<double>::infinity())
        {
            break;
        }
        
        visited.insert(current);
        
        if (current == end)
        {
            break;
        }
        
        for (Edge* edge : graph->getEdgesFrom(current))
        {
            if (edge->getStatus() == EdgeStatus::Blocked)
            {
                continue;
            }
            
            QString neighbor = (edge->getFrom() == current) ? edge->getTo() : edge->getFrom();
            
            if (visited.contains(neighbor))
            {
                continue;
            }
            
            double weight = edge->getWeight();
            if (edge->getStatus() == EdgeStatus::Accident)
            {
                weight *= 1.333;
            }
            
            double newDistance = distances[current] + weight;
            
            if (newDistance < distances[neighbor])
            {
                distances[neighbor] = newDistance;
                previous[neighbor] = current;
            }
        }
    }
    
    if (distances[end] != std::numeric_limits<double>::infinity())
    {
        QString current = end;
        while (current != start)
        {
            result.path.prepend(current);
            current = previous[current];
        }
        result.path.prepend(start);
        result.totalDistance = distances[end];
        result.distances = distances;
        result.found = true;
    }
    
    return result;
}

PathResult GraphAlgorithms::floydWarshall(Graph* graph, const QString& start, const QString& end)
{
    PathResult result;
    
    if (!graph->hasVertex(start) || !graph->hasVertex(end))
    {
        return result;
    }
    
    QVector<Vertex*> vertices = graph->getVertices();
    int n = vertices.size();
    
    QMap<QString, int> vertexIndex;
    for (int i = 0; i < n; ++i)
    {
        vertexIndex[vertices[i]->getName()] = i;
    }
    
    QVector<QVector<double>> dist(n, QVector<double>(n, std::numeric_limits<double>::infinity()));
    QVector<QVector<int>> next(n, QVector<int>(n, -1));
    
    for (int i = 0; i < n; ++i)
    {
        dist[i][i] = 0.0;
    }
    
    for (Edge* edge : graph->getEdges())
    {
        if (edge->getStatus() == EdgeStatus::Blocked)
        {
            continue;
        }
        
        int u = vertexIndex[edge->getFrom()];
        int v = vertexIndex[edge->getTo()];
        double weight = edge->getWeight();
        
        if (edge->getStatus() == EdgeStatus::Accident)
        {
            weight *= 1.333;
        }
        
        if (weight < dist[u][v])
        {
            dist[u][v] = weight;
            dist[v][u] = weight;
            next[u][v] = v;
            next[v][u] = u;
        }
    }
    
    for (int k = 0; k < n; ++k)
    {
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
    
    int startIdx = vertexIndex[start];
    int endIdx = vertexIndex[end];
    
    if (dist[startIdx][endIdx] != std::numeric_limits<double>::infinity())
    {
        int u = startIdx;
        result.path.append(vertices[u]->getName());
        
        while (u != endIdx)
        {
            u = next[u][endIdx];
            result.path.append(vertices[u]->getName());
        }
        
        result.totalDistance = dist[startIdx][endIdx];
        result.found = true;
    }
    
    return result;
}

TreeResult GraphAlgorithms::prim(Graph* graph)
{
    TreeResult result;
    
    if (graph->getVertexCount() == 0)
    {
        return result;
    }
    
    QSet<QString> visited;
    QVector<Vertex*> vertices = graph->getVertices();
    
    visited.insert(vertices[0]->getName());
    
    while (visited.size() < vertices.size())
    {
        Edge* minEdge = nullptr;
        double minWeight = std::numeric_limits<double>::infinity();
        
        for (Edge* edge : graph->getEdges())
        {
            if (edge->getStatus() == EdgeStatus::Blocked)
            {
                continue;
            }
            
            bool fromVisited = visited.contains(edge->getFrom());
            bool toVisited = visited.contains(edge->getTo());
            
            if (fromVisited != toVisited)
            {
                double weight = edge->getWeight();
                if (edge->getStatus() == EdgeStatus::Accident)
                {
                    weight *= 1.333;
                }
                
                if (weight < minWeight)
                {
                    minWeight = weight;
                    minEdge = edge;
                }
            }
        }
        
        if (!minEdge)
        {
            break;
        }
        
        result.edges.append(qMakePair(minEdge->getFrom(), minEdge->getTo()));
        result.totalWeight += minWeight;
        
        visited.insert(minEdge->getFrom());
        visited.insert(minEdge->getTo());
    }
    
    return result;
}

TreeResult GraphAlgorithms::kruskal(Graph* graph)
{
    TreeResult result;
    
    if (graph->getVertexCount() == 0)
    {
        return result;
    }
    
    QVector<Edge*> edges = graph->getEdges();
    QVector<QPair<double, Edge*>> sortedEdges;
    
    for (Edge* edge : edges)
    {
        if (edge->getStatus() == EdgeStatus::Blocked)
        {
            continue;
        }
        
        double weight = edge->getWeight();
        if (edge->getStatus() == EdgeStatus::Accident)
        {
            weight *= 1.333;
        }
        
        sortedEdges.append(qMakePair(weight, edge));
    }
    
    std::sort(sortedEdges.begin(), sortedEdges.end(),
              [](const QPair<double, Edge*>& a, const QPair<double, Edge*>& b) {
                  return a.first < b.first;
              });
    
    QMap<QString, QString> parent;
    QMap<QString, int> rank;
    
    for (Vertex* vertex : graph->getVertices())
    {
        QString name = vertex->getName();
        parent[name] = name;
        rank[name] = 0;
    }
    
    for (const auto& pair : sortedEdges)
    {
        double weight = pair.first;
        Edge* edge = pair.second;
        
        QString set1 = findSet(edge->getFrom(), parent);
        QString set2 = findSet(edge->getTo(), parent);
        
        if (set1 != set2)
        {
            result.edges.append(qMakePair(edge->getFrom(), edge->getTo()));
            result.totalWeight += weight;
            unionSets(set1, set2, parent, rank);
        }
    }
    
    return result;
}

QString GraphAlgorithms::findSet(const QString& vertex, QMap<QString, QString>& parent)
{
    if (parent[vertex] != vertex)
    {
        parent[vertex] = findSet(parent[vertex], parent);
    }
    return parent[vertex];
}

void GraphAlgorithms::unionSets(const QString& u, const QString& v, QMap<QString, QString>& parent, QMap<QString, int>& rank)
{
    if (rank[u] < rank[v])
    {
        parent[u] = v;
    }
    else if (rank[u] > rank[v])
    {
        parent[v] = u;
    }
    else
    {
        parent[v] = u;
        rank[u]++;
    }
}
