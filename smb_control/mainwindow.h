/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>

#include <QMainWindow>

#include <QtSerialPort/QSerialPort>

#include <QSlider>
#include <QScrollBar>
#include <QDial>

#define SMB_WIDTH1   50
#define SMB_WIDTH2   300
#define SMB_WIDTH3   70


#define SMB_F_MIN    45
#define SMB_F_VAL    50
#define SMB_F_MAX    55

#define SMB_V_MIN    100
#define SMB_V_VAL    110
#define SMB_V_MAX    120

#define SMB_P_MIN    (-45)
#define SMB_P_VAL    0
#define SMB_P_MAX    45

QT_BEGIN_NAMESPACE

class QLabel;

namespace Ui {
class MainWindow;
}

class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QSpinBox;

QT_END_NAMESPACE

class Console;
class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void openSerialPort();
    void closeSerialPort();
    void about();
    void writeData(const QByteArray &data);
    void readData();
    void onFrequencyValueChanged(int value);
    void onVoltageValueChanged(int value);
    void onPhaseValueChanged(int value);
    void onFrequencySpinChanged(int value);
    void onVoltageSpinChanged(int value);
    void onPhaseSpinChanged(int value);
    void setFrequencyMinimum(int value);
    void setFrequencyMaximum(int value);
    void setVoltageMinimum(int value);
    void setVoltageMaximum(int value);
    void setPhaseMinimum(int value);
    void setPhaseMaximum(int value);
    void onApply(int value);

    void handleError(QSerialPort::SerialPortError error);

private:
    void initActionsConnections();
    void on_FrequencyValueChanged(int value);
    void on_VoltageValueChanged(int value);
    void on_PhaseValueChanged(int value);

private:
    void showStatusMessage(const QString &message);
    void createScrollBars(const QString &title);
    // void createSettings(const QString &title);

    Ui::MainWindow *ui;
    QLabel *status;
    Console *console;

    QGroupBox *scrollBarsGroup;
    QLabel *frequencyLabel;
    QLabel *voltageLabel;
    QLabel *phaseLabel;
    QSpinBox *frequencySpin;
    QSpinBox *voltageSpin;
    QSpinBox *phaseSpin;
    QScrollBar *frequency;
    QScrollBar *voltage;
    QScrollBar *phase;

    /*
    QGroupBox *settingsGroup;
    QLabel *minFrequencyLabel;
    QLabel *maxFrequencyLabel;
    QSpinBox *minFrequencyBox;
    QSpinBox *maxFrequencyBox;
    QLabel *minVoltageLabel;
    QLabel *maxVoltageLabel;
    QSpinBox *minVoltageBox;
    QSpinBox *maxVoltageBox;
    QLabel *minPhaseLabel;
    QLabel *maxPhaseLabel;
    QSpinBox *minPhaseBox;
    QSpinBox *maxPhaseBox;
    */
    SettingsDialog *settings;
    QSerialPort *serial;
};

#endif // MAINWINDOW_H
