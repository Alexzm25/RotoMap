#pragma once
#include <QString>

enum class EdgeStatus
{
    Normal,
    Blocked,
    Accident
};

class Edge
{
public:
    Edge(const QString& from, const QString& to, double weight);
    
    QString getFrom() const;
    QString getTo() const;
    double getWeight() const;
    void setWeight(double weight);
    
    EdgeStatus getStatus() const;
    void setStatus(EdgeStatus status);
    
private:
    QString m_from;
    QString m_to;
    double m_weight;
    EdgeStatus m_status;
};
