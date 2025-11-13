#include "../header/Vertex.h"

Vertex::Vertex(const QString& name, const QPointF& position)
    : m_name(name), m_position(position)
{
}

QString Vertex::getName() const
{
    return m_name;
}

QPointF Vertex::getPosition() const
{
    return m_position;
}

void Vertex::setPosition(const QPointF& position)
{
    m_position = position;
}
