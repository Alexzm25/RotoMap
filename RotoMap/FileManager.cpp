#include "FileManager.h"
#include <QFile>
#include <QTextStream>

bool FileManager::loadGraph(const QString& filename, Graph* graph)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }
    
    graph->clear();
    QTextStream in(&file);
    
    QString line = in.readLine();
    if (line.startsWith("VERTICES"))
    {
        while (!in.atEnd())
        {
            line = in.readLine().trimmed();
            if (line.isEmpty() || line.startsWith("EDGES"))
            {
                break;
            }
            
            QStringList parts = line.split(',');
            if (parts.size() >= 3)
            {
                QString name = parts[0].trimmed();
                double x = parts[1].trimmed().toDouble();
                double y = parts[2].trimmed().toDouble();
                graph->addVertex(name, QPointF(x, y));
            }
        }
    }
    
    if (line.startsWith("EDGES"))
    {
        while (!in.atEnd())
        {
            line = in.readLine().trimmed();
            if (line.isEmpty())
            {
                continue;
            }
            
            QStringList parts = line.split(',');
            if (parts.size() >= 3)
            {
                QString from = parts[0].trimmed();
                QString to = parts[1].trimmed();
                double weight = parts[2].trimmed().toDouble();
                graph->addEdge(from, to, weight);
                
                if (parts.size() >= 4)
                {
                    QString status = parts[3].trimmed().toLower();
                    Edge* edge = graph->getEdge(from, to);
                    if (edge)
                    {
                        if (status == "blocked")
                        {
                            edge->setStatus(EdgeStatus::Blocked);
                        }
                        else if (status == "accident")
                        {
                            edge->setStatus(EdgeStatus::Accident);
                        }
                    }
                }
            }
        }
    }
    
    file.close();
    return true;
}

bool FileManager::saveGraph(const QString& filename, Graph* graph)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }
    
    QTextStream out(&file);
    
    out << "VERTICES\n";
    for (Vertex* vertex : graph->getVertices())
    {
        out << vertex->getName() << ", "
            << vertex->getPosition().x() << ", "
            << vertex->getPosition().y() << "\n";
    }
    
    out << "\nEDGES\n";
    for (Edge* edge : graph->getEdges())
    {
        out << edge->getFrom() << ", "
            << edge->getTo() << ", "
            << edge->getWeight();
        
        if (edge->getStatus() == EdgeStatus::Blocked)
        {
            out << ", blocked";
        }
        else if (edge->getStatus() == EdgeStatus::Accident)
        {
            out << ", accident";
        }
        
        out << "\n";
    }
    
    file.close();
    return true;
}
