#include "MainWindow.h"
#include "FileManager.h"
#include "GraphAlgorithms.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QTextStream>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_graph(new Graph()), m_currentAction("none")
{
    ui.setupUi(this);

    ui.mainToolBar->addAction(ui.actionLoadRoutes);
    ui.mainToolBar->addAction(ui.actionSaveRoutes);

    this->setStyleSheet(StyleSheetManager::getRotoMapStyleSheet());
    this->setWindowTitle("RotoMap - Sistema de Rutas Inteligente");
    
    setupScenes();
    connectSignals();
}

MainWindow::~MainWindow()
{
    delete m_graph;
}

void MainWindow::setupScenes()
{
    m_routeScene = new GraphScene(this);
    m_algScene = new GraphScene(this);
    
    m_routeScene->setGraph(m_graph);
    m_algScene->setGraph(m_graph);
    
    ui.mapRoute->setScene(m_routeScene);
    ui.mapAlg->setScene(m_algScene);
    
    ui.mapRoute->setRenderHint(QPainter::Antialiasing);
    ui.mapAlg->setRenderHint(QPainter::Antialiasing);
    ui.mapRoute->setRenderHint(QPainter::SmoothPixmapTransform);
    ui.mapAlg->setRenderHint(QPainter::SmoothPixmapTransform);
    
    ui.mapRoute->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.mapRoute->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.mapAlg->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.mapAlg->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    ui.mapRoute->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui.mapAlg->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    
    ui.mapRoute->setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing, false);
    ui.mapAlg->setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing, false);
    
    ui.comboBox->addItem("Dijkstra (Camino más corto)");
    ui.comboBox->addItem("Floyd-Warshall (Camino más corto)");
    ui.comboBox->addItem("Prim (Árbol de expansión mínima)");
    ui.comboBox->addItem("Kruskal (Árbol de expansión mínima)");
    
    QTimer::singleShot(200, this, [this]() {
        QSize routeViewSize = ui.mapRoute->viewport()->size();
        QSize algViewSize = ui.mapAlg->viewport()->size();
        
        m_routeScene->setBackgroundImage(":/MainWindow/assets/Map.jpg", routeViewSize);
        m_algScene->setBackgroundImage(":/MainWindow/assets/Map.jpg", algViewSize);
        
        ui.mapRoute->resetTransform();
        ui.mapAlg->resetTransform();
        
        ui.mapRoute->fitInView(m_routeScene->sceneRect(), Qt::IgnoreAspectRatio);
        ui.mapAlg->fitInView(m_algScene->sceneRect(), Qt::IgnoreAspectRatio);
        
        ui.mapRoute->update();
        ui.mapAlg->update();
    });
}

void MainWindow::connectSignals()
{
    connect(ui.actionLoadRoutes, &QAction::triggered, this, &MainWindow::onLoadRoutes);
    connect(ui.actionSaveRoutes, &QAction::triggered, this, &MainWindow::onSaveRoutes);
    connect(ui.btnSaveStop, &QPushButton::clicked, this, &MainWindow::onSaveStop);
    connect(ui.btnDeleteStop, &QPushButton::clicked, this, &MainWindow::onDeleteStop);
    connect(ui.btnAddBlock, &QPushButton::clicked, this, &MainWindow::onAddBlock);
    connect(ui.btnAddAccident, &QPushButton::clicked, this, &MainWindow::onAddAccident);
    connect(ui.btnExecuteAlg, &QPushButton::clicked, this, &MainWindow::onExecuteAlgorithm);
    connect(ui.btnLinkRoute, &QPushButton::clicked, this, &MainWindow::onLinkRoute);
    connect(ui.btnSaveResults, &QPushButton::clicked, this, &MainWindow::onSaveResults);
    
    connect(m_routeScene, &GraphScene::vertexClicked, this, &MainWindow::onVertexClicked);
    connect(m_routeScene, &GraphScene::edgeClicked, this, &MainWindow::onEdgeClicked);
    connect(m_routeScene, &GraphScene::emptySpaceClicked, this, &MainWindow::onEmptySpaceClicked);
    
    connect(ui.mainTabs, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
}

void MainWindow::onLoadRoutes()
{
    QString filename = QFileDialog::getOpenFileName(this, "Cargar Rutas", "", "Text Files (*.txt)");
    if (filename.isEmpty())
    {
        return;
    }
    
    if (FileManager::loadGraph(filename, m_graph))
    {
        updateScenes();
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("RotoMap - Éxito");
        msgBox.setText("Rutas cargadas correctamente");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("RotoMap - Error");
        msgBox.setText("No se pudo cargar el archivo");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}

void MainWindow::onSaveRoutes()
{
    QString filename = QFileDialog::getSaveFileName(this, "Guardar Rutas", "rutas.txt", "Text Files (*.txt)");
    if (filename.isEmpty())
    {
        return;
    }
    
    if (FileManager::saveGraph(filename, m_graph))
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("RotoMap - Éxito");
        msgBox.setText("Rutas guardadas correctamente");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("RotoMap - Error");
        msgBox.setText("No se pudo guardar el archivo");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}

void MainWindow::onSaveStop()
{
    m_currentAction = "addVertex";
    m_routeScene->setClickMode("addVertex");
    ui.statusBar->showMessage("Haga clic en el mapa para colocar la nueva parada");
}

void MainWindow::onLinkRoute()
{
    m_currentAction = "selectForEdge";
    m_routeScene->setClickMode("selectForEdge");
    ui.statusBar->showMessage("Seleccione la primera parada para conectar");
}

void MainWindow::onVertexClicked(const QString& vertexName)
{
    if (m_currentAction == "deleteVertex")
    {
        m_graph->removeVertex(vertexName);
        updateScenes();
        resetAction();
        ui.statusBar->showMessage("Parada eliminada");
    }
    else if (m_currentAction == "selectForEdge")
    {
        if (m_selectedVertex1.isEmpty())
        {
            m_selectedVertex1 = vertexName;
            ui.statusBar->showMessage(QString("Seleccionado: %1. Ahora seleccione la parada de destino").arg(vertexName));
        }
        else
        {
            m_selectedVertex2 = vertexName;
            
            QInputDialog dialog(this);
            dialog.setWindowTitle("RotoMap - Tiempo de Ruta");
            dialog.setLabelText(QString("Tiempo entre %1 y %2 (minutos):").arg(m_selectedVertex1, m_selectedVertex2));
            dialog.setDoubleValue(0);
            dialog.setDoubleRange(0, 10000);
            dialog.setDoubleDecimals(0);
            dialog.resize(450, 150);
            bool ok = dialog.exec();
            double weight = dialog.doubleValue();
            
            if (ok)
            {
                m_graph->addEdge(m_selectedVertex1, m_selectedVertex2, weight);
                updateScenes();
                ui.statusBar->showMessage("Ruta agregada");
            }
            resetAction();
        }
    }
}

void MainWindow::onEdgeClicked(Edge* edge)
{
    if (m_currentAction == "addBlock")
    {
        QInputDialog dialog(this);
        dialog.setWindowTitle("RotoMap - Bloquear Ruta");
        dialog.setLabelText(QString("Razón del bloqueo en la ruta %1 - %2:").arg(edge->getFrom(), edge->getTo()));
        dialog.setTextValue("");
        dialog.resize(450, 150);
        bool ok = dialog.exec();
        QString reason = dialog.textValue();
        
        if (ok)
        {
            edge->setStatus(EdgeStatus::Blocked);
            updateScenes();
            ui.statusBar->showMessage("Ruta bloqueada: " + reason);
        }
        resetAction();
    }
    else if (m_currentAction == "addAccident")
    {
        QInputDialog dialog(this);
        dialog.setWindowTitle("RotoMap - Accidente en Ruta");
        dialog.setLabelText(QString("Razón del accidente en la ruta %1 - %2:").arg(edge->getFrom(), edge->getTo()));
        dialog.setTextValue("");
        dialog.resize(450, 150);
        bool ok = dialog.exec();
        QString reason = dialog.textValue();
        
        if (ok)
        {
            edge->setStatus(EdgeStatus::Accident);
            updateScenes();
            ui.statusBar->showMessage("Accidente registrado: " + reason);
        }
        resetAction();
    }
}

void MainWindow::onEmptySpaceClicked(const QPointF& position)
{
    if (m_currentAction == "addVertex")
    {
        bool ok;
        QInputDialog dialog(this);
        dialog.setWindowTitle("RotoMap - Registrar Parada");
        dialog.setLabelText("Nombre de la parada:");
        dialog.setTextValue("");
        dialog.resize(400, 150);
        ok = dialog.exec();
        QString name = dialog.textValue();
        
        if (!ok || name.isEmpty())
        {
            resetAction();
            return;
        }
        
        if (m_graph->hasVertex(name))
        {
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("RotoMap - Advertencia");
            msgBox.setText("Ya existe una parada con ese nombre");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            resetAction();
            return;
        }
        
        m_graph->addVertex(name, position);
        updateScenes();
        
        QStringList vertices;
        for (Vertex* v : m_graph->getVertices())
        {
            if (v->getName() != name)
            {
                vertices.append(v->getName());
            }
        }
        
        if (!vertices.isEmpty())
        {
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("RotoMap - Conectar Parada");
            msgBox.setText("¿Desea conectar esta parada con otra?");
            msgBox.setIcon(QMessageBox::Question);
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            auto reply = msgBox.exec();
            
            if (reply == QMessageBox::Yes)
            {
                m_currentAction = "selectForEdge";
                m_selectedVertex1 = name;
                m_routeScene->setClickMode("none");
                ui.statusBar->showMessage(QString("Parada '%1' creada. Seleccione una parada para conectar").arg(name));
                return;
            }
        }
        
        ui.statusBar->showMessage(QString("Parada '%1' creada").arg(name));
        resetAction();
    }
}

void MainWindow::onDeleteStop()
{
    if (m_graph->getVertexCount() == 0)
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("RotoMap - Advertencia");
        msgBox.setText("No hay paradas para eliminar");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
    
    m_currentAction = "deleteVertex";
    m_routeScene->setClickMode("none");
    ui.statusBar->showMessage("Haga clic en una parada para eliminarla");
}

void MainWindow::onAddBlock()
{
    if (m_graph->getEdgeCount() == 0)
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("RotoMap - Advertencia");
        msgBox.setText("No hay rutas para bloquear");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
    
    m_currentAction = "addBlock";
    m_routeScene->setClickMode("none");
    ui.statusBar->showMessage("Haga clic en una ruta (línea) para bloquearla");
}

void MainWindow::onAddAccident()
{
    if (m_graph->getEdgeCount() == 0)
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("RotoMap - Advertencia");
        msgBox.setText("No hay rutas para reportar accidentes");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
    
    m_currentAction = "addAccident";
    m_routeScene->setClickMode("none");
    ui.statusBar->showMessage("Haga clic en una ruta (línea) para reportar un accidente");
}

void MainWindow::onExecuteAlgorithm()
{
    if (m_graph->getVertexCount() == 0)
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("RotoMap - Advertencia");
        msgBox.setText("No hay paradas en el grafo");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
    
    int algIndex = ui.comboBox->currentIndex();
    
    if (algIndex < 2)
    {
        QStringList vertices;
        for (Vertex* v : m_graph->getVertices())
        {
            vertices.append(v->getName());
        }
        
        QInputDialog startDialog(this);
        startDialog.setWindowTitle("RotoMap - Seleccionar Inicio");
        startDialog.setLabelText("Parada de inicio:");
        startDialog.setComboBoxItems(vertices);
        startDialog.resize(400, 150);
        bool ok1 = startDialog.exec();
        QString start = startDialog.textValue();
        if (!ok1) return;
        
        QInputDialog endDialog(this);
        endDialog.setWindowTitle("RotoMap - Seleccionar Destino");
        endDialog.setLabelText("Parada de destino:");
        endDialog.setComboBoxItems(vertices);
        endDialog.resize(400, 150);
        bool ok2 = endDialog.exec();
        QString end = endDialog.textValue();
        if (!ok2) return;
        
        PathResult result;
        QString algName;
        
        if (algIndex == 0)
        {
            result = GraphAlgorithms::dijkstra(m_graph, start, end);
            algName = "Dijkstra";
        }
        else
        {
            result = GraphAlgorithms::floydWarshall(m_graph, start, end);
            algName = "Floyd-Warshall";
        }
        
        if (result.found)
        {
            QColor pathColor = (algIndex == 0) ? QColor(0, 255, 0) : QColor(0, 200, 255);
            m_algScene->highlightPath(result.path, pathColor);
            
            QString resultText = QString("Algoritmo: %1\n\n").arg(algName);
            resultText += "Ruta:\n";
            for (const QString& vertex : result.path)
            {
                resultText += vertex + " → ";
            }
            resultText.chop(3);
            resultText += QString("\n\nTiempo total: %1 minutos").arg(result.totalDistance, 0, 'f', 0);
            
            ui.resultsText->setPlainText(resultText);
            m_lastResult = resultText;
        }
        else
        {
            QMessageBox msgBox(this);
            msgBox.setWindowTitle("RotoMap - Resultado");
            msgBox.setText("No se encontró un camino");
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
            ui.resultsText->setPlainText("No se encontró un camino entre los puntos seleccionados");
        }
    }
    else
    {
        TreeResult result;
        QString algName;
        
        if (algIndex == 2)
        {
            result = GraphAlgorithms::prim(m_graph);
            algName = "Prim";
        }
        else
        {
            result = GraphAlgorithms::kruskal(m_graph);
            algName = "Kruskal";
        }
        
        QColor treeColor = (algIndex == 2) ? QColor(255, 0, 255) : QColor(255, 200, 0);
        m_algScene->highlightTree(result.edges, treeColor);
        
        QString resultText = QString("Algoritmo: %1\n\n").arg(algName);
        resultText += "Árbol de expansión mínima:\n";
        for (const auto& edge : result.edges)
        {
            resultText += QString("%1 ↔ %2\n").arg(edge.first, edge.second);
        }
        resultText += QString("\nTiempo total: %1 minutos").arg(result.totalWeight, 0, 'f', 0);
        
        ui.resultsText->setPlainText(resultText);
        m_lastResult = resultText;
    }
}

void MainWindow::onSaveResults()
{
    if (m_lastResult.isEmpty())
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("RotoMap - Advertencia");
        msgBox.setText("No hay resultados para guardar");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
    
    QString filename = QFileDialog::getSaveFileName(this, "Guardar Resultados", 
                                                    "resultados.txt", "Text Files (*.txt)");
    if (filename.isEmpty())
    {
        return;
    }
    
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << "RESULTADOS DEL ANÁLISIS\n";
        out << "========================\n\n";
        out << m_lastResult;
        file.close();
        
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("RotoMap - Éxito");
        msgBox.setText("Resultados guardados correctamente");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("RotoMap - Error");
        msgBox.setText("No se pudo guardar el archivo");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}

void MainWindow::updateScenes()
{
    m_routeScene->drawGraph();
    m_algScene->drawGraph();
}

void MainWindow::resetAction()
{
    m_currentAction = "none";
    m_selectedVertex1.clear();
    m_selectedVertex2.clear();
    m_routeScene->setClickMode("none");
    ui.statusBar->clearMessage();
}

void MainWindow::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);
    QTimer::singleShot(50, this, [this]() {
        ui.mapRoute->fitInView(m_routeScene->sceneRect(), Qt::IgnoreAspectRatio);
        ui.mapAlg->fitInView(m_algScene->sceneRect(), Qt::IgnoreAspectRatio);
    });
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    if (m_routeScene && m_algScene)
    {
        QTimer::singleShot(50, this, [this]() {
            ui.mapRoute->fitInView(m_routeScene->sceneRect(), Qt::IgnoreAspectRatio);
            ui.mapAlg->fitInView(m_algScene->sceneRect(), Qt::IgnoreAspectRatio);
        });
    }
}

void MainWindow::onTabChanged(int index)
{
    QTimer::singleShot(50, this, [this, index]() {
        if (index == 0)
        {
            QSize routeViewSize = ui.mapRoute->viewport()->size();
            if (routeViewSize.width() > 0 && routeViewSize.height() > 0)
            {
                ui.mapRoute->resetTransform();
                ui.mapRoute->fitInView(m_routeScene->sceneRect(), Qt::IgnoreAspectRatio);
                ui.mapRoute->update();
            }
        }
        else if (index == 1)
        {
            QSize algViewSize = ui.mapAlg->viewport()->size();
            if (algViewSize.width() > 0 && algViewSize.height() > 0)
            {
                ui.mapAlg->resetTransform();
                ui.mapAlg->fitInView(m_algScene->sceneRect(), Qt::IgnoreAspectRatio);
                ui.mapAlg->update();
            }
        }
    });
}

