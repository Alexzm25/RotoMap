#pragma once
#include <QVector>
#include <QString>
#include <QMap>

struct PathResult
{
    QVector<QString> path;
    double totalDistance;
    QMap<QString, double> distances;
    bool found;
    
    PathResult() : totalDistance(0.0), found(false) {}
};

struct TreeResult
{
    QVector<QPair<QString, QString>> edges;
    double totalWeight;
    QString root;
    
    TreeResult() : totalWeight(0.0) {}
};
