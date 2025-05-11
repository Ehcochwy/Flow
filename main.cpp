/**
 * @file main.cpp
 * @brief Main entry for DiagramEditor application
 * @author Ehcochwy
 * @date 2025-05-10
 */

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 加载翻译文件
    QTranslator translator;
    if (translator.load("mui/zh_CN/lessoncode.qm")) {
        app.installTranslator(&translator);
    }
    
    MainWindow w;
    w.resize(1200, 800);
    w.show();
    
    return app.exec();
}