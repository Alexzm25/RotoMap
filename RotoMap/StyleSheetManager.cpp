#include "StyleSheetManager.h"

QString StyleSheetManager::getRotoMapStyleSheet()
{
    return R"(
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #1a1a2e, stop:0.5 #16213e, stop:1 #0f3460);
        }
        
        QTabWidget::pane {
            border: 2px solid #FF6B35;
            border-radius: 8px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2C3E50, stop:1 #1a1a2e);
            padding: 5px;
        }
        
        QTabWidget::tab-bar {
            alignment: center;
        }
        
        QTabBar::tab {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #34495e, stop:1 #2C3E50);
            color: #ECEFF4;
            padding: 12px 30px;
            margin: 2px;
            border: 2px solid #00D9FF;
            border-bottom: none;
            border-top-left-radius: 8px;
            border-top-right-radius: 8px;
            font-weight: bold;
            font-size: 11pt;
            min-width: 150px;
        }
        
        QTabBar::tab:selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #FF6B35, stop:1 #ff8c42);
            color: #ffffff;
            border: 2px solid #00D9FF;
            border-bottom: none;
            box-shadow: 0 0 15px #00D9FF;
        }
        
        QTabBar::tab:hover:!selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ff8c42, stop:1 #FF6B35);
            border: 2px solid #7FDBFF;
        }
        
        QGroupBox {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2C3E50, stop:1 #1e2d3d);
            border: 2px solid #00D9FF;
            border-radius: 10px;
            margin-top: 15px;
            padding-top: 15px;
            font-weight: bold;
            font-size: 10pt;
            color: #ECEFF4;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top center;
            padding: 5px 15px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #FF6B35, stop:0.5 #ff8c42, stop:1 #FF6B35);
            border: 2px solid #00D9FF;
            border-radius: 8px;
            color: #ffffff;
            font-size: 11pt;
        }
        
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #FF6B35, stop:1 #e85d30);
            color: #ffffff;
            border: 2px solid #00D9FF;
            border-radius: 8px;
            padding: 10px 15px;
            font-weight: bold;
            font-size: 10pt;
            min-height: 35px;
        }
        
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ff8c42, stop:1 #FF6B35);
            border: 2px solid #7FDBFF;
            box-shadow: 0 0 10px #00D9FF;
        }
        
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #e85d30, stop:1 #d14d25);
            border: 2px solid #00D9FF;
        }
        
        QPushButton:disabled {
            background: #4a4a4a;
            color: #888888;
            border: 2px solid #555555;
        }
        
        QComboBox {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #34495e, stop:1 #2C3E50);
            color: #ECEFF4;
            border: 2px solid #00D9FF;
            border-radius: 6px;
            padding: 8px;
            font-size: 10pt;
            min-height: 30px;
        }
        
        QComboBox:hover {
            border: 2px solid #7FDBFF;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #3d5a70, stop:1 #34495e);
        }
        
        QComboBox::drop-down {
            border: none;
            width: 30px;
        }
        
        QComboBox::down-arrow {
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 7px solid #00D9FF;
            margin-right: 8px;
        }
        
        QComboBox QAbstractItemView {
            background: #2C3E50;
            color: #ECEFF4;
            selection-background-color: #FF6B35;
            selection-color: #ffffff;
            border: 2px solid #00D9FF;
            border-radius: 4px;
            padding: 5px;
        }
        
        QComboBox QAbstractItemView::item {
            min-height: 30px;
            padding: 5px;
        }
        
        QComboBox QAbstractItemView::item:hover {
            background: #ff8c42;
            color: #ffffff;
        }
        
        QLabel {
            color: #ECEFF4;
            font-size: 10pt;
            background: transparent;
        }
        
        QGraphicsView {
            background: #1a1a2e;
            border: 3px solid #00D9FF;
            border-radius: 10px;
        }
        
        QStatusBar {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #2C3E50, stop:1 #34495e);
            color: #ECEFF4;
            border-top: 2px solid #00D9FF;
            font-size: 10pt;
            padding: 3px;
        }
        
        QToolBar {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #2C3E50, stop:0.5 #34495e, stop:1 #2C3E50);
            border-bottom: 2px solid #FF6B35;
            spacing: 5px;
            padding: 5px;
        }
        
        QToolBar::separator {
            background: #00D9FF;
            width: 2px;
            margin: 5px;
        }
        
        QToolButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #FF6B35, stop:1 #e85d30);
            color: #ffffff;
            border: 2px solid #00D9FF;
            border-radius: 6px;
            padding: 8px;
            margin: 2px;
            font-weight: bold;
        }
        
        QToolButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ff8c42, stop:1 #FF6B35);
            border: 2px solid #7FDBFF;
        }
        
        QToolButton:pressed {
            background: #e85d30;
        }
        
        QMenuBar {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #2C3E50, stop:1 #34495e);
            color: #ECEFF4;
            border-bottom: 2px solid #00D9FF;
            padding: 3px;
        }
        
        QMenuBar::item {
            background: transparent;
            padding: 8px 15px;
            border-radius: 4px;
        }
        
        QMenuBar::item:selected {
            background: #FF6B35;
            color: #ffffff;
        }
        
        QMenuBar::item:pressed {
            background: #e85d30;
        }
        
        QMenu {
            background: #2C3E50;
            color: #ECEFF4;
            border: 2px solid #00D9FF;
            border-radius: 6px;
            padding: 5px;
        }
        
        QMenu::item {
            padding: 8px 30px 8px 20px;
            border-radius: 4px;
        }
        
        QMenu::item:selected {
            background: #FF6B35;
            color: #ffffff;
        }
        
        QMenu::separator {
            height: 2px;
            background: #00D9FF;
            margin: 5px 10px;
        }
        
        QScrollBar:vertical {
            background: #2C3E50;
            width: 14px;
            border-radius: 7px;
            margin: 0;
        }
        
        QScrollBar::handle:vertical {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #FF6B35, stop:1 #ff8c42);
            border: 1px solid #00D9FF;
            border-radius: 6px;
            min-height: 30px;
        }
        
        QScrollBar::handle:vertical:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #ff8c42, stop:1 #FF6B35);
        }
        
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0;
        }
        
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            background: none;
        }
        
        QScrollBar:horizontal {
            background: #2C3E50;
            height: 14px;
            border-radius: 7px;
            margin: 0;
        }
        
        QScrollBar::handle:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #FF6B35, stop:1 #ff8c42);
            border: 1px solid #00D9FF;
            border-radius: 6px;
            min-width: 30px;
        }
        
        QScrollBar::handle:horizontal:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #ff8c42, stop:1 #FF6B35);
        }
        
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            width: 0;
        }
        
        QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
            background: none;
        }
        
        QMessageBox {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2C3E50, stop:1 #1a1a2e);
        }
        
        QMessageBox QLabel {
            color: #ECEFF4;
            font-size: 11pt;
            min-width: 300px;
            padding: 10px;
        }
        
        QMessageBox QPushButton {
            min-width: 80px;
            margin: 5px;
        }
        
        QInputDialog {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2C3E50, stop:1 #1a1a2e);
        }
        
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #2C3E50, stop:1 #1a1a2e);
        }
        
        QLineEdit {
            background: #34495e;
            color: #ECEFF4;
            border: 2px solid #00D9FF;
            border-radius: 6px;
            padding: 8px;
            font-size: 10pt;
            selection-background-color: #FF6B35;
            selection-color: #ffffff;
        }
        
        QLineEdit:focus {
            border: 2px solid #7FDBFF;
            background: #3d5a70;
        }
        
        QLineEdit:hover {
            border: 2px solid #7FDBFF;
        }
        
        QTextEdit {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #34495e, stop:1 #2C3E50);
            color: #ECEFF4;
            border: 2px solid #00D9FF;
            border-radius: 8px;
            padding: 10px;
            font-size: 11pt;
            font-family: "Consolas", "Courier New", monospace;
            selection-background-color: #FF6B35;
            selection-color: #ffffff;
        }
        
        QTextEdit:focus {
            border: 2px solid #7FDBFF;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #3d5a70, stop:1 #34495e);
        }
        
        QFileDialog {
            background: #2C3E50;
            color: #ECEFF4;
        }
        
        QFileDialog QPushButton {
            min-width: 70px;
        }
        
        QFileDialog QTreeView {
            background: #34495e;
            color: #ECEFF4;
            border: 2px solid #00D9FF;
            border-radius: 6px;
            selection-background-color: #FF6B35;
        }
        
        QFileDialog QListView {
            background: #34495e;
            color: #ECEFF4;
            border: 2px solid #00D9FF;
            border-radius: 6px;
            selection-background-color: #FF6B35;
        }
        
        Line {
            background: #00D9FF;
        }
    )";
}
