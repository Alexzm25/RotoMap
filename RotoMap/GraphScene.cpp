#include "GraphScene.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <cmath>

GraphScene::GraphScene(QObject* parent)
    : QGraphicsScene(parent), m_graph(nullptr), m_backgroundItem(nullptr), m_clickMode("none")
{
    m_pokeStopIcon = QPixmap(":/MainWindow/assets/PokeStop.png");
    if (m_pokeStopIcon.isNull())
    {
        m_pokeStopIcon = QPixmap("assets/PokeStop.png");
    }
    
    m_accidentSign = QPixmap(":/MainWindow/assets/AccidentSign.png");
    if (m_accidentSign.isNull())
    {
        m_accidentSign = QPixmap("assets/AccidentSign.png");
    }
    
    m_blockedSign = QPixmap(":/MainWindow/assets/BlockSign.png");
    if (m_blockedSign.isNull())
    {
        m_blockedSign = QPixmap("assets/BlockSign.png");
    }
}

void GraphScene::setClickMode(const QString& mode)
{
    m_clickMode = mode;
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QPointF clickPos = event->scenePos();
    
    if (m_clickMode == "addVertex")
    {
        emit emptySpaceClicked(clickPos);
        return;
    }
    
    QString vertexName = getClickedVertex(clickPos);
    if (!vertexName.isEmpty())
    {
        emit vertexClicked(vertexName);
        return;
    }
    
    Edge* edge = getClickedEdge(clickPos);
    if (edge)
    {
        emit edgeClicked(edge);
        return;
    }
    
    QGraphicsScene::mousePressEvent(event);
}

QString GraphScene::getClickedVertex(const QPointF& pos)
{
    if (!m_graph)
    {
        return QString();
    }
    
    for (Vertex* vertex : m_graph->getVertices())
    {
        QPointF vPos = getScaledPosition(vertex->getPosition());
        double distance = QLineF(vPos, pos).length();
        if (distance < 50)
        {
            return vertex->getName();
        }
    }
    return QString();
}

Edge* GraphScene::getClickedEdge(const QPointF& pos)
{
    if (!m_graph)
    {
        return nullptr;
    }
    
    for (Edge* edge : m_graph->getEdges())
    {
        Vertex* from = m_graph->getVertex(edge->getFrom());
        Vertex* to = m_graph->getVertex(edge->getTo());
        
        if (from && to)
        {
            QPointF p1 = getScaledPosition(from->getPosition());
            QPointF p2 = getScaledPosition(to->getPosition());
            QLineF line(p1, p2);
            
            double minDistance = std::numeric_limits<double>::infinity();
            
            for (double t = 0; t <= 1.0; t += 0.01)
            {
                QPointF point = line.pointAt(t);
                double d = QLineF(point, pos).length();
                if (d < minDistance)
                {
                    minDistance = d;
                }
            }
            
            if (minDistance < 15)
            {
                return edge;
            }
        }
    }
    return nullptr;
}

void GraphScene::setGraph(Graph* graph)
{
    m_graph = graph;
}

void GraphScene::drawGraph()
{
    qDeleteAll(m_graphItems);
    m_graphItems.clear();
    clearHighlights();
    
    if (!m_graph)
    {
        return;
    }
    
    for (Edge* edge : m_graph->getEdges())
    {
        drawEdge(edge);
    }
    
    for (Vertex* vertex : m_graph->getVertices())
    {
        drawVertex(vertex);
    }
}

void GraphScene::drawVertex(Vertex* vertex)
{
    QPointF pos = getScaledPosition(vertex->getPosition());
    
    if (!m_pokeStopIcon.isNull())
    {
        QPixmap scaledIcon = m_pokeStopIcon.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QGraphicsPixmapItem* iconItem = addPixmap(scaledIcon);
        iconItem->setPos(pos.x() - scaledIcon.width() / 2, pos.y() - scaledIcon.height() / 2);
        iconItem->setZValue(2);
        m_graphItems.append(iconItem);
    }
    else
    {
        QGraphicsEllipseItem* circle = addEllipse(pos.x() - 45, pos.y() - 45, 90, 90,
                                                  QPen(Qt::black, 3), QBrush(QColor(100, 150, 255)));
        circle->setZValue(2);
        m_graphItems.append(circle);
    }
    
    QGraphicsTextItem* textBg = addText(vertex->getName());
    QFont font = textBg->font();
    font.setBold(true);
    font.setPointSize(16);
    textBg->setFont(font);
    
    qreal textWidth = textBg->boundingRect().width();
    qreal textHeight = textBg->boundingRect().height();
    
    QGraphicsRectItem* bgRect = addRect(0, 0, textWidth + 10, textHeight + 4, 
                                        QPen(Qt::NoPen), 
                                        QBrush(QColor(0, 0, 0, 180)));
    bgRect->setPos(pos.x() - (textWidth + 10) / 2, pos.y() + 55);
    bgRect->setZValue(3);
    m_graphItems.append(bgRect);
    
    QGraphicsTextItem* text = addText(vertex->getName());
    text->setFont(font);
    text->setDefaultTextColor(Qt::white);
    text->setPos(pos.x() - textWidth / 2, pos.y() + 57);
    text->setZValue(4);
    m_graphItems.append(text);
    
    delete textBg;
}

void GraphScene::drawEdge(Edge* edge)
{
    Vertex* from = m_graph->getVertex(edge->getFrom());
    Vertex* to = m_graph->getVertex(edge->getTo());
    
    if (!from || !to)
    {
        return;
    }
    
    QPointF p1 = getScaledPosition(from->getPosition());
    QPointF p2 = getScaledPosition(to->getPosition());
    
    QPen pen;
    pen.setWidth(7);
    
    switch (edge->getStatus())
    {
    case EdgeStatus::Blocked:
        pen.setColor(Qt::red);
        pen.setStyle(Qt::DashLine);
        break;
    case EdgeStatus::Accident:
        pen.setColor(QColor(255, 165, 0));
        break;
    default:
        pen.setColor(QColor(0, 217, 255));
        break;
    }
    
    QGraphicsLineItem* line = addLine(p1.x(), p1.y(), p2.x(), p2.y(), pen);
    line->setZValue(1);
    m_graphItems.append(line);
    
    QPointF midPoint = (p1 + p2) / 2.0;
    QString weightText = QString::number(edge->getWeight(), 'f', 1) + " km";
    
    if (edge->getStatus() == EdgeStatus::Accident && !m_accidentSign.isNull())
    {
        QPixmap scaledSign = m_accidentSign.scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QGraphicsPixmapItem* signItem = addPixmap(scaledSign);
        signItem->setPos(midPoint.x() - scaledSign.width() / 2, midPoint.y() - scaledSign.height() / 2 - 15);
        signItem->setZValue(5);
        m_graphItems.append(signItem);
        
        QGraphicsTextItem* weightTextBg = addText(weightText);
        QFont font = weightTextBg->font();
        font.setBold(true);
        font.setPointSize(16);
        weightTextBg->setFont(font);
        
        qreal textWidth = weightTextBg->boundingRect().width();
        qreal textHeight = weightTextBg->boundingRect().height();
        
        QGraphicsRectItem* bgRect = addRect(0, 0, textWidth + 10, textHeight + 4, 
                                            QPen(Qt::NoPen), 
                                            QBrush(QColor(0, 0, 0, 200)));
        bgRect->setPos(midPoint.x() - (textWidth + 10) / 2, midPoint.y() + 30);
        bgRect->setZValue(5);
        m_graphItems.append(bgRect);
        
        QGraphicsTextItem* weightTextItem = addText(weightText);
        weightTextItem->setFont(font);
        weightTextItem->setDefaultTextColor(Qt::yellow);
        weightTextItem->setPos(midPoint.x() - textWidth / 2, midPoint.y() + 32);
        weightTextItem->setZValue(6);
        m_graphItems.append(weightTextItem);
        
        delete weightTextBg;
    }
    else if (edge->getStatus() == EdgeStatus::Blocked && !m_blockedSign.isNull())
    {
        QPixmap scaledSign = m_blockedSign.scaled(70, 70, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QGraphicsPixmapItem* signItem = addPixmap(scaledSign);
        signItem->setPos(midPoint.x() - scaledSign.width() / 2, midPoint.y() - scaledSign.height() / 2 - 15);
        signItem->setZValue(5);
        m_graphItems.append(signItem);
        
        QGraphicsTextItem* weightTextBg = addText(weightText);
        QFont font = weightTextBg->font();
        font.setBold(true);
        font.setPointSize(16);
        weightTextBg->setFont(font);
        
        qreal textWidth = weightTextBg->boundingRect().width();
        qreal textHeight = weightTextBg->boundingRect().height();
        
        QGraphicsRectItem* bgRect = addRect(0, 0, textWidth + 10, textHeight + 4, 
                                            QPen(Qt::NoPen), 
                                            QBrush(QColor(0, 0, 0, 200)));
        bgRect->setPos(midPoint.x() - (textWidth + 10) / 2, midPoint.y() + 30);
        bgRect->setZValue(5);
        m_graphItems.append(bgRect);
        
        QGraphicsTextItem* weightTextItem = addText(weightText);
        weightTextItem->setFont(font);
        weightTextItem->setDefaultTextColor(Qt::yellow);
        weightTextItem->setPos(midPoint.x() - textWidth / 2, midPoint.y() + 32);
        weightTextItem->setZValue(6);
        m_graphItems.append(weightTextItem);
        
        delete weightTextBg;
    }
    else
    {
        QGraphicsTextItem* weightTextItem = addText(weightText);
        
        QFont font = weightTextItem->font();
        font.setBold(true);
        font.setPointSize(16);
        weightTextItem->setFont(font);
        
        weightTextItem->setDefaultTextColor(Qt::yellow);
        
        QPen outlinePen(Qt::black);
        outlinePen.setWidth(3);
        
        QGraphicsTextItem* outline = addText(weightText);
        outline->setFont(font);
        outline->setDefaultTextColor(Qt::black);
        outline->setPos(midPoint.x() - outline->boundingRect().width() / 2,
                       midPoint.y() - outline->boundingRect().height() / 2);
        outline->setZValue(5);
        m_graphItems.append(outline);
        
        for (int dx = -1; dx <= 1; ++dx)
        {
            for (int dy = -1; dy <= 1; ++dy)
            {
                if (dx == 0 && dy == 0) continue;
                
                QGraphicsTextItem* shadow = addText(weightText);
                shadow->setFont(font);
                shadow->setDefaultTextColor(Qt::black);
                shadow->setPos(midPoint.x() - shadow->boundingRect().width() / 2 + dx,
                              midPoint.y() - shadow->boundingRect().height() / 2 + dy);
                shadow->setZValue(5);
                m_graphItems.append(shadow);
            }
        }
        
        weightTextItem->setPos(midPoint.x() - weightTextItem->boundingRect().width() / 2,
                          midPoint.y() - weightTextItem->boundingRect().height() / 2);
        weightTextItem->setZValue(6);
        m_graphItems.append(weightTextItem);
    }
}

void GraphScene::highlightPath(const QVector<QString>& path, const QColor& color)
{
    clearHighlights();
    
    if (!m_graph || path.size() < 2)
    {
        return;
    }
    
    for (int i = 0; i < path.size() - 1; ++i)
    {
        Vertex* from = m_graph->getVertex(path[i]);
        Vertex* to = m_graph->getVertex(path[i + 1]);
        
        if (from && to)
        {
            QPointF p1 = getScaledPosition(from->getPosition());
            QPointF p2 = getScaledPosition(to->getPosition());
            
            QPen pen(color, 5);
            QGraphicsLineItem* line = addLine(p1.x(), p1.y(), p2.x(), p2.y(), pen);
            m_highlightItems.append(line);
        }
    }
}

void GraphScene::highlightTree(const QVector<QPair<QString, QString>>& edges, const QColor& color)
{
    clearHighlights();
    
    if (!m_graph)
    {
        return;
    }
    
    for (const auto& edge : edges)
    {
        Vertex* from = m_graph->getVertex(edge.first);
        Vertex* to = m_graph->getVertex(edge.second);
        
        if (from && to)
        {
            QPointF p1 = getScaledPosition(from->getPosition());
            QPointF p2 = getScaledPosition(to->getPosition());
            
            QPen pen(color, 5);
            QGraphicsLineItem* line = addLine(p1.x(), p1.y(), p2.x(), p2.y(), pen);
            m_highlightItems.append(line);
        }
    }
}

void GraphScene::clearHighlights()
{
    for (QGraphicsItem* item : m_highlightItems)
    {
        removeItem(item);
        delete item;
    }
    m_highlightItems.clear();
}

void GraphScene::setBackgroundImage(const QString& imagePath, const QSize& viewSize)
{
    if (m_backgroundItem)
    {
        removeItem(m_backgroundItem);
        delete m_backgroundItem;
    }
    
    QPixmap pixmap(imagePath);
    if (!pixmap.isNull())
    {
        m_backgroundItem = addPixmap(pixmap);
        m_backgroundItem->setZValue(-1);
        m_backgroundItem->setPos(0, 0);
        m_backgroundItem->setTransformationMode(Qt::SmoothTransformation);
        setSceneRect(0, 0, pixmap.width(), pixmap.height());
    }
}

QPointF GraphScene::getScaledPosition(const QPointF& pos)
{
    return pos;
}
