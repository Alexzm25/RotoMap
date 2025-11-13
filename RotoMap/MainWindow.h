#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "Graph.h"
#include "GraphScene.h"
#include "StyleSheetManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoadRoutes();
    void onSaveRoutes();
    void onSaveStop();
    void onDeleteStop();
    void onAddBlock();
    void onAddAccident();
    void onExecuteAlgorithm();
    void onExportResults();
    void onSaveResults();
    void onVertexClicked(const QString& vertexName);
    void onEdgeClicked(Edge* edge);
    void onEmptySpaceClicked(const QPointF& position);
    void onTabChanged(int index);

protected:
    void showEvent(QShowEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    Ui::MainWindowClass ui;
    Graph* m_graph;
    GraphScene* m_routeScene;
    GraphScene* m_algScene;
    QString m_lastResult;
    QString m_currentAction;
    QString m_selectedVertex1;
    QString m_selectedVertex2;
    
    void setupScenes();
    void connectSignals();
    void updateScenes();
    void resetAction();
};

