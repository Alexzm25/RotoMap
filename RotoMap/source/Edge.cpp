#include "../header/Edge.h"

Edge::Edge(const QString& from, const QString& to, double weight)
    : m_from(from), m_to(to), m_weight(weight), m_status(EdgeStatus::Normal)
{
}

QString Edge::getFrom() const
{
    return m_from;
}

QString Edge::getTo() const
{
    return m_to;
}

double Edge::getWeight() const
{
    return m_weight;
}

void Edge::setWeight(double weight)
{
    m_weight = weight;
}

EdgeStatus Edge::getStatus() const
{
    return m_status;
}

void Edge::setStatus(EdgeStatus status)
{
    m_status = status;
}
