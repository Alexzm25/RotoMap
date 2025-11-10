#pragma once
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include "Graph.h"

class GraphScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GraphScene(QObject* parent = nullptr);
    
    void setGraph(Graph* graph);
    void drawGraph();
    void highlightPath(const QVector<QString>& path, const QColor& color);
    void highlightTree(const QVector<QPair<QString, QString>>& edges, const QColor& color);
    void clearHighlights();
    void setBackgroundImage(const QString& imagePath, const QSize& viewSize);
    void setClickMode(const QString& mode);
    QString getClickedVertex(const QPointF& pos);
    Edge* getClickedEdge(const QPointF& pos);

signals:
    void vertexClicked(const QString& vertexName);
    void edgeClicked(Edge* edge);
    void emptySpaceClicked(const QPointF& position);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    
private:
    Graph* m_graph;
    QGraphicsPixmapItem* m_backgroundItem;
    QVector<QGraphicsItem*> m_graphItems;
    QVector<QGraphicsItem*> m_highlightItems;
    QString m_clickMode;
    
    void drawVertex(Vertex* vertex);
    void drawEdge(Edge* edge);
    QPointF getScaledPosition(const QPointF& pos);
};
