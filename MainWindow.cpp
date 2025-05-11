/**
 * @file MainWindow.cpp
 * @brief Implementation of the main window
 * @author Ehcochwy
 * @date 2025-05-10
 */

#include "MainWindow.h"
#include "DiagramCanvas.h"
#include "ShapeToolBox.h"
#include "PropertyPanel.h"
#include "FlowIO.h"
#include <QMenuBar>
#include <QToolBar>
#include <QDockWidget>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QKeySequence>
#include <QClipboard>
#include <QMimeData>
#include <QApplication>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_currentFilePath("")
{
    setWindowTitle(tr("Diagram Editor"));

    // Create canvas
    m_canvas = new DiagramCanvas(this);
    setCentralWidget(m_canvas);

    // Create toolbox and property panel
    m_toolBox = new ShapeToolBox(this);
    m_propertyPanel = new PropertyPanel(this);

    createActions();
    createMenus();
    createToolBar();
    createDockWidgets();
    createStatusBar();
    createShortcuts();
    setupConnections();
}

void MainWindow::createActions()
{
    m_newAction = new QAction(tr("New"), this);
    m_openAction = new QAction(tr("Open..."), this);
    m_saveAction = new QAction(tr("Save"), this);
    m_saveAsAction = new QAction(tr("Save As..."), this);
    m_exportPngAction = new QAction(tr("Export as PNG..."), this);
    m_exportSvgAction = new QAction(tr("Export as SVG..."), this);

    m_copyAction = new QAction(tr("Copy"), this);
    m_cutAction = new QAction(tr("Cut"), this);
    m_pasteAction = new QAction(tr("Paste"), this);
    m_duplicateAction = new QAction(tr("Duplicate"), this);
    m_deleteAction = new QAction(tr("Delete"), this);

    m_bringToFrontAction = new QAction(tr("Bring to Front"), this);
    m_sendToBackAction = new QAction(tr("Send to Back"), this);
    m_bringForwardAction = new QAction(tr("Bring Forward"), this);
    m_sendBackwardAction = new QAction(tr("Send Backward"), this);

    m_backgroundColorAction = new QAction(tr("Background Color..."), this);
    m_canvasSizeAction = new QAction(tr("Canvas Size..."), this);
}

void MainWindow::createMenus()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(m_newAction);
    fileMenu->addAction(m_openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_saveAction);
    fileMenu->addAction(m_saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(m_exportPngAction);
    fileMenu->addAction(m_exportSvgAction);

    QMenu* editMenu = menuBar()->addMenu(tr("Edit"));
    editMenu->addAction(m_copyAction);
    editMenu->addAction(m_cutAction);
    editMenu->addAction(m_pasteAction);
    editMenu->addAction(m_duplicateAction);
    editMenu->addSeparator();
    editMenu->addAction(m_deleteAction);

    QMenu* arrangeMenu = menuBar()->addMenu(tr("Arrange"));
    arrangeMenu->addAction(m_bringToFrontAction);
    arrangeMenu->addAction(m_sendToBackAction);
    arrangeMenu->addAction(m_bringForwardAction);
    arrangeMenu->addAction(m_sendBackwardAction);

    QMenu* pageMenu = menuBar()->addMenu(tr("Page"));
    pageMenu->addAction(m_backgroundColorAction);
    pageMenu->addAction(m_canvasSizeAction);
}

void MainWindow::createToolBar()
{
    QToolBar* toolbar = addToolBar(tr("Toolbar"));
    toolbar->setMovable(false);
    m_toolBox->addToolBarActions(toolbar);

    toolbar->addSeparator();
    toolbar->addAction(m_copyAction);
    toolbar->addAction(m_cutAction);
    toolbar->addAction(m_pasteAction);
    toolbar->addAction(m_deleteAction);
}

void MainWindow::createDockWidgets()
{
    QDockWidget* toolboxDock = new QDockWidget(tr("Shape Library"), this);
    toolboxDock->setWidget(m_toolBox);
    toolboxDock->setAllowedAreas(Qt::LeftDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, toolboxDock);

    QDockWidget* propertyDock = new QDockWidget(tr("Properties"), this);
    propertyDock->setWidget(m_propertyPanel);
    propertyDock->setAllowedAreas(Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, propertyDock);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createShortcuts()
{
    m_newAction->setShortcut(QKeySequence::New);
    m_openAction->setShortcut(QKeySequence::Open);
    m_saveAction->setShortcut(QKeySequence::Save);
    m_saveAsAction->setShortcut(QKeySequence("Ctrl+Shift+S"));

    m_copyAction->setShortcut(QKeySequence::Copy);
    m_cutAction->setShortcut(QKeySequence::Cut);
    m_pasteAction->setShortcut(QKeySequence::Paste);
    m_duplicateAction->setShortcut(QKeySequence("Ctrl+D"));
    m_deleteAction->setShortcut(QKeySequence::Delete);
}

void MainWindow::setupConnections()
{
    connect(m_newAction, &QAction::triggered, this, &MainWindow::onCreateNewFile);
    connect(m_openAction, &QAction::triggered, this, &MainWindow::onOpenFile);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::onSaveFile);
    connect(m_saveAsAction, &QAction::triggered, this, &MainWindow::onSaveAsFile);
    connect(m_exportPngAction, &QAction::triggered, this, &MainWindow::onExportToPng);
    connect(m_exportSvgAction, &QAction::triggered, this, &MainWindow::onExportToSvg);

    connect(m_copyAction, &QAction::triggered, this, &MainWindow::onCopySelected);
    connect(m_cutAction, &QAction::triggered, this, &MainWindow::onCutSelected);
    connect(m_pasteAction, &QAction::triggered, this, &MainWindow::onPasteFromClipboard);
    connect(m_duplicateAction, &QAction::triggered, this, &MainWindow::onDuplicateSelected);
    connect(m_deleteAction, &QAction::triggered, this, &MainWindow::onDeleteSelected);

    connect(m_bringToFrontAction, &QAction::triggered, m_canvas, &DiagramCanvas::bringToFront);
    connect(m_sendToBackAction, &QAction::triggered, m_canvas, &DiagramCanvas::sendToBack);
    connect(m_bringForwardAction, &QAction::triggered, m_canvas, &DiagramCanvas::bringForward);
    connect(m_sendBackwardAction, &QAction::triggered, m_canvas, &DiagramCanvas::sendBackward);

    connect(m_backgroundColorAction, &QAction::triggered, m_canvas, &DiagramCanvas::chooseBackgroundColor);
    connect(m_canvasSizeAction, &QAction::triggered, m_canvas, [this](bool) { m_canvas->setCanvasSize(); });

    connect(m_canvas, &DiagramCanvas::shapeSelected, m_propertyPanel, &PropertyPanel::setShape);
    connect(m_propertyPanel, &PropertyPanel::shapeChanged, m_canvas, &DiagramCanvas::refreshCanvas);
    


    connect(m_toolBox, &ShapeToolBox::shapeToolSelected, m_canvas, &DiagramCanvas::setActiveShapeTool);

    connect(m_canvas, &DiagramCanvas::selectionChanged, [this](bool hasSelection) {
        m_copyAction->setEnabled(hasSelection);
        m_cutAction->setEnabled(hasSelection);
        m_duplicateAction->setEnabled(hasSelection);
        m_deleteAction->setEnabled(hasSelection);
        m_bringToFrontAction->setEnabled(hasSelection);
        m_sendToBackAction->setEnabled(hasSelection);
        m_bringForwardAction->setEnabled(hasSelection);
        m_sendBackwardAction->setEnabled(hasSelection);
        });
}

void MainWindow::onCreateNewFile()
{
    if (m_canvas->isModified()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, tr("Save Changes"),
            tr("The current diagram has unsaved changes. Save now?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
        );
        if (reply == QMessageBox::Yes) onSaveFile();
        if (reply == QMessageBox::Cancel) return;
    }
    m_canvas->clear();
    m_currentFilePath.clear();
    setWindowTitle(tr("Diagram Editor - Untitled"));
    m_canvas->setModified(false);
}

void MainWindow::onOpenFile()
{
    if (m_canvas->isModified()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, tr("Save Changes"),
            tr("The current diagram has unsaved changes. Save now?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
        );
        if (reply == QMessageBox::Yes) onSaveFile();
        if (reply == QMessageBox::Cancel) return;
    }

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Diagram"), "", tr("Diagram Files (*.flow)"));
    if (fileName.isEmpty()) return;

    if (FlowIO::load(fileName, m_canvas)) {
        m_currentFilePath = fileName;
        setWindowTitle(tr("Diagram Editor - %1").arg(QFileInfo(fileName).fileName()));
        m_canvas->setModified(false);
        statusBar()->showMessage(tr("Loaded: %1").arg(fileName), 5000);
    }
    else {
        QMessageBox::warning(this, tr("Failed to Open"), tr("Cannot open file: %1").arg(fileName));
    }
}

void MainWindow::onSaveFile()
{
    if (m_currentFilePath.isEmpty()) {
        onSaveAsFile();
        return;
    }

    if (FlowIO::save(m_currentFilePath, m_canvas)) {
        m_canvas->setModified(false);
        statusBar()->showMessage(tr("Saved: %1").arg(m_currentFilePath), 5000);
    }
    else {
        QMessageBox::warning(this, tr("Failed to Save"), tr("Cannot save file: %1").arg(m_currentFilePath));
    }
}

void MainWindow::onSaveAsFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Diagram"), "", tr("Diagram Files (*.flow)"));
    if (fileName.isEmpty()) return;

    if (!fileName.endsWith(".flow", Qt::CaseInsensitive)) {
        fileName += ".flow";
    }

    if (FlowIO::save(fileName, m_canvas)) {
        m_currentFilePath = fileName;
        setWindowTitle(tr("Diagram Editor - %1").arg(QFileInfo(fileName).fileName()));
        m_canvas->setModified(false);
        statusBar()->showMessage(tr("Saved: %1").arg(fileName), 5000);
    }
    else {
        QMessageBox::warning(this, tr("Failed to Save"), tr("Cannot save file: %1").arg(fileName));
    }
}

void MainWindow::onExportToPng()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export PNG"), "", tr("PNG Image (*.png)"));
    if (fileName.isEmpty()) return;
    if (!fileName.endsWith(".png", Qt::CaseInsensitive)) fileName += ".png";

    if (m_canvas->exportToPng(fileName)) {
        statusBar()->showMessage(tr("Exported PNG: %1").arg(fileName), 5000);
    }
    else {
        QMessageBox::warning(this, tr("Export Failed"), tr("Cannot export to PNG: %1").arg(fileName));
    }
}

void MainWindow::onExportToSvg()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export SVG"), "", tr("SVG Image (*.svg)"));
    if (fileName.isEmpty()) return;
    if (!fileName.endsWith(".svg", Qt::CaseInsensitive)) fileName += ".svg";

    if (m_canvas->exportToSvg(fileName)) {
        statusBar()->showMessage(tr("Exported SVG: %1").arg(fileName), 5000);
    }
    else {
        QMessageBox::warning(this, tr("Export Failed"), tr("Cannot export to SVG: %1").arg(fileName));
    }
}

void MainWindow::onCopySelected() { m_canvas->copySelectedToClipboard(); }
void MainWindow::onCutSelected() { m_canvas->cutSelectedToClipboard(); }
void MainWindow::onPasteFromClipboard() { m_canvas->pasteFromClipboard(); }
void MainWindow::onDuplicateSelected() { m_canvas->duplicateSelected(); }
void MainWindow::onDeleteSelected() { m_canvas->deleteSelected(); }
