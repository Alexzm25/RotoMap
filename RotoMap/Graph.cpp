#include "Graph.h"

Graph::Graph()
{
}

void Graph::addVertex(const QString& name, const QPointF& position)
{
    if (!m_vertices.contains(name))
    {
        m_vertices[name] = new Vertex(name, position);
    }
}

void Graph::removeVertex(const QString& name)
{
    if (m_vertices.contains(name))
    {
        delete m_vertices[name];
        m_vertices.remove(name);
        
        for (int i = m_edges.size() - 1; i >= 0; --i)
        {
            if (m_edges[i]->getFrom() == name || m_edges[i]->getTo() == name)
            {
                delete m_edges[i];
                m_edges.removeAt(i);
            }
        }
    }
}

void Graph::addEdge(const QString& from, const QString& to, double weight)
{
    if (m_vertices.contains(from) && m_vertices.contains(to))
    {
        Edge* existingEdge = getEdge(from, to);
        if (!existingEdge)
        {
            m_edges.append(new Edge(from, to, weight));
        }
    }
}

void Graph::removeEdge(const QString& from, const QString& to)
{
    for (int i = 0; i < m_edges.size(); ++i)
    {
        if (m_edges[i]->getFrom() == from && m_edges[i]->getTo() == to)
        {
            delete m_edges[i];
            m_edges.removeAt(i);
            break;
        }
    }
}

bool Graph::hasVertex(const QString& name) const
{
    return m_vertices.contains(name);
}

Vertex* Graph::getVertex(const QString& name)
{
    return m_vertices.value(name, nullptr);
}

QVector<Vertex*> Graph::getVertices() const
{
    return m_vertices.values().toVector();
}

QVector<Edge*> Graph::getEdges() const
{
    return m_edges;
}

QVector<Edge*> Graph::getEdgesFrom(const QString& vertexName) const
{
    QVector<Edge*> result;
    for (Edge* edge : m_edges)
    {
        if (edge->getFrom() == vertexName || edge->getTo() == vertexName)
        {
            result.append(edge);
        }
    }
    return result;
}

Edge* Graph::getEdge(const QString& from, const QString& to) const
{
    for (Edge* edge : m_edges)
    {
        if (edge->getFrom() == from && edge->getTo() == to)
        {
            return edge;
        }
    }
    return nullptr;
}

void Graph::clear()
{
    qDeleteAll(m_vertices);
    m_vertices.clear();
    qDeleteAll(m_edges);
    m_edges.clear();
}

int Graph::getVertexCount() const
{
    return m_vertices.size();
}

int Graph::getEdgeCount() const
{
    return m_edges.size();
}
