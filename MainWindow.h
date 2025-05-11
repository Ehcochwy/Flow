/**
 * @file MainWindow.h
 * @brief Main window for DiagramEditor application
 * @author Ehcochwy
 * @date 2025-05-10
 */

#pragma once
#include <QMainWindow>
#include <QClipboard>

class DiagramCanvas;
class ShapeToolBox;
class PropertyPanel;
class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() = default;

private slots:
    void onCreateNewFile();
    void onOpenFile();
    void onSaveFile();
    void onSaveAsFile();
    void onExportToPng();
    void onExportToSvg();
    
    void onCopySelected();
    void onCutSelected();
    void onPasteFromClipboard();
    void onDuplicateSelected();
    void onDeleteSelected();
    
private:
    void createMenus();
    void createToolBar();
    void createDockWidgets();
    void createStatusBar();
    void createActions();
    void createShortcuts();
    void setupConnections();
    
    DiagramCanvas* m_canvas;
    ShapeToolBox* m_toolBox;
    PropertyPanel* m_propertyPanel;
    
    //FILE
    QAction* m_newAction;
    QAction* m_openAction;
    QAction* m_saveAction;
    QAction* m_saveAsAction;
    QAction* m_exportPngAction;
    QAction* m_exportSvgAction;
    
    //EDITOR
    QAction* m_copyAction;
    QAction* m_cutAction;
    QAction* m_pasteAction;
    QAction* m_duplicateAction;
    QAction* m_deleteAction;
    
    //LAYOUT
    QAction* m_bringToFrontAction;
    QAction* m_sendToBackAction;
    QAction* m_bringForwardAction;
    QAction* m_sendBackwardAction;
    
    //PAGE
    QAction* m_backgroundColorAction;
    QAction* m_canvasSizeAction;
    
    QString m_currentFilePath;
};