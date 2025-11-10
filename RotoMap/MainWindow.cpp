#include "MainWindow.h"
#include "FileManager.h"
#include "GraphAlgorithms.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_graph(new Graph()), m_currentAction("none")
{
    ui.setupUi(this);

    ui.mainToolBar->addAction(ui.actionLoadRoutes);
    ui.mainToolBar->addAction(ui.actionSaveRoutes);

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
    
    QSize viewSize(800, 600);
    
    m_routeScene->setBackgroundImage(":/MainWindow/assets/Map.jpg", viewSize);
    m_algScene->setBackgroundImage(":/MainWindow/assets/Map.jpg", viewSize);
    
    ui.mapRoute->setScene(m_routeScene);
    ui.mapAlg->setScene(m_algScene);
    
    ui.mapRoute->setRenderHint(QPainter::Antialiasing);
    ui.mapAlg->setRenderHint(QPainter::Antialiasing);
    
    ui.mapRoute->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.mapRoute->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.mapAlg->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.mapAlg->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    ui.mapRoute->setFixedSize(viewSize);
    ui.mapAlg->setFixedSize(viewSize);
    
    ui.comboBox->addItem("Dijkstra (Camino más corto)");
    ui.comboBox->addItem("Floyd-Warshall (Camino más corto)");
    ui.comboBox->addItem("Prim (Árbol de expansión mínima)");
    ui.comboBox->addItem("Kruskal (Árbol de expansión mínima)");
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
    connect(ui.btnExportResults, &QPushButton::clicked, this, &MainWindow::onExportResults);
    connect(ui.btnSaveResults, &QPushButton::clicked, this, &MainWindow::onSaveResults);
    
    connect(m_routeScene, &GraphScene::vertexClicked, this, &MainWindow::onVertexClicked);
    connect(m_routeScene, &GraphScene::edgeClicked, this, &MainWindow::onEdgeClicked);
    connect(m_routeScene, &GraphScene::emptySpaceClicked, this, &MainWindow::onEmptySpaceClicked);
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
        QMessageBox::information(this, "Éxito", "Rutas cargadas correctamente");
    }
    else
    {
        QMessageBox::critical(this, "Error", "No se pudo cargar el archivo");
    }
}

void MainWindow::onSaveRoutes()
{
    QString filename = QFileDialog::getSaveFileName(this, "Guardar Rutas", "", "Text Files (*.txt)");
    if (filename.isEmpty())
    {
        return;
    }
    
    if (FileManager::saveGraph(filename, m_graph))
    {
        QMessageBox::information(this, "Éxito", "Rutas guardadas correctamente");
    }
    else
    {
        QMessageBox::critical(this, "Error", "No se pudo guardar el archivo");
    }
}

void MainWindow::onSaveStop()
{
    m_currentAction = "addVertex";
    m_routeScene->setClickMode("addVertex");
    ui.statusBar->showMessage("Haga clic en el mapa para colocar la nueva parada");
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
            
            bool ok;
            double weight = QInputDialog::getDouble(this, "Peso de Arista",
                                                    QString("Distancia entre %1 y %2 (km):").arg(m_selectedVertex1, m_selectedVertex2),
                                                    0, 0, 10000, 1, &ok);
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
        bool ok;
        QString reason = QInputDialog::getText(this, "Bloquear Ruta",
                                              QString("Razón del bloqueo en la ruta %1 - %2:").arg(edge->getFrom(), edge->getTo()),
                                              QLineEdit::Normal, "", &ok);
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
        bool ok;
        QString reason = QInputDialog::getText(this, "Accidente en Ruta",
                                              QString("Razón del accidente en la ruta %1 - %2:").arg(edge->getFrom(), edge->getTo()),
                                              QLineEdit::Normal, "", &ok);
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
        QString name = QInputDialog::getText(this, "Registrar Parada",
                                            "Nombre de la parada:", QLineEdit::Normal, "", &ok);
        if (!ok || name.isEmpty())
        {
            resetAction();
            return;
        }
        
        if (m_graph->hasVertex(name))
        {
            QMessageBox::warning(this, "Advertencia", "Ya existe una parada con ese nombre");
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
            auto reply = QMessageBox::question(this, "Conectar Parada",
                                              "¿Desea conectar esta parada con otra?",
                                              QMessageBox::Yes | QMessageBox::No);
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
        QMessageBox::warning(this, "Advertencia", "No hay paradas para eliminar");
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
        QMessageBox::warning(this, "Advertencia", "No hay rutas para bloquear");
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
        QMessageBox::warning(this, "Advertencia", "No hay rutas para reportar accidentes");
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
        QMessageBox::warning(this, "Advertencia", "No hay paradas en el grafo");
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
        
        bool ok;
        QString start = QInputDialog::getItem(this, "Seleccionar Inicio",
                                              "Parada de inicio:", vertices, 0, false, &ok);
        if (!ok) return;
        
        QString end = QInputDialog::getItem(this, "Seleccionar Destino",
                                            "Parada de destino:", vertices, 0, false, &ok);
        if (!ok) return;
        
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
            resultText += QString("\n\nDistancia total: %1 km").arg(result.totalDistance, 0, 'f', 2);
            
            ui.resultsLbl->setText(resultText);
            m_lastResult = resultText;
        }
        else
        {
            QMessageBox::warning(this, "Resultado", "No se encontró un camino");
            ui.resultsLbl->setText("No se encontró un camino entre los puntos seleccionados");
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
        resultText += QString("\nPeso total: %1 km").arg(result.totalWeight, 0, 'f', 2);
        
        ui.resultsLbl->setText(resultText);
        m_lastResult = resultText;
    }
}

void MainWindow::onExportResults()
{
    if (m_lastResult.isEmpty())
    {
        QMessageBox::warning(this, "Advertencia", "No hay resultados para exportar");
        return;
    }
    
    QString filename = QFileDialog::getSaveFileName(this, "Exportar Resultados", 
                                                    "", "Text Files (*.txt)");
    if (filename.isEmpty())
    {
        return;
    }
    
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << m_lastResult;
        file.close();
        QMessageBox::information(this, "Éxito", "Resultados exportados correctamente");
    }
    else
    {
        QMessageBox::critical(this, "Error", "No se pudo exportar el archivo");
    }
}

void MainWindow::onSaveResults()
{
    if (m_lastResult.isEmpty())
    {
        QMessageBox::warning(this, "Advertencia", "No hay resultados para guardar");
        return;
    }
    
    QString filename = QFileDialog::getSaveFileName(this, "Guardar Resultados", 
                                                    "", "Text Files (*.txt)");
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
        QMessageBox::information(this, "Éxito", "Resultados guardados correctamente");
    }
    else
    {
        QMessageBox::critical(this, "Error", "No se pudo guardar el archivo");
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

