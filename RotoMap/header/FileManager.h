#pragma once
#include <QString>
#include "Graph.h"

class FileManager
{
public:
    static bool loadGraph(const QString& filename, Graph* graph);
    static bool saveGraph(const QString& filename, Graph* graph);
};
