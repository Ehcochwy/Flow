/**
 * @file PropertyPanel.h
 * @brief Property panel for editing shape attributes
 * @author Ehcochwy
 * @date 2025-05-10
 */

#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QComboBox>
#include <memory>
#include "DiagramShape.h"

class PropertyPanel : public QWidget
{
    Q_OBJECT
public:
    PropertyPanel(QWidget* parent = nullptr);
    ~PropertyPanel() = default;
    
    void setShape(std::shared_ptr<DiagramShape> shape);
    
signals:
    void shapeChanged();
    
private slots:
    void onFillColorClicked();
    void onLineColorClicked();
    void onLineWidthChanged(int width);
    void onTextChanged();
    void onTextColorClicked();
    
private:
    void setupUI();
    void updateUI();
    
    std::shared_ptr<DiagramShape> m_shape;
    
    // UI控件
    QLineEdit* m_textEdit;
    QPushButton* m_fillColorBtn;
    QPushButton* m_lineColorBtn;
    QPushButton* m_textColorBtn;
    QSpinBox* m_lineWidthSpin;
    QComboBox* m_fontCombo;
    QSpinBox* m_fontSizeSpin;
};