#pragma once
#include <QString>
#include <QPointF>

class Vertex
{
public:
    Vertex(const QString& name, const QPointF& position);
    
    QString getName() const;
    QPointF getPosition() const;
    void setPosition(const QPointF& position);
    
private:
    QString m_name;
    QPointF m_position;
};
