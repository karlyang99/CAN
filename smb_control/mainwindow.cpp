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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "console.h"
#include "settingsdialog.h"

#include <QMessageBox>
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include <QVBoxLayout>
#include <QSlider>
#include <QScrollBar>
#include <QDial>
#include <QDebug>
#include <QGroupBox>
#include <QSpinBox>
#include <QWidget>

//! [0]
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//! [0]
    ui->setupUi(this);
    console = new Console;
    console->setEnabled(false);
    // setCentralWidget(console);
    // ui->centralWidget->setLayout(new QVBoxLayout);
    ui->centralWidget->layout()->addWidget(console);

    /*
    slider = new QSlider(this);
    // slider->resize(255, 20);
    slider->setOrientation(Qt::Horizontal);
    // slider->setRange(0, 255);
    ui->centralWidget->layout()->addWidget(slider);

    scroll = new QScrollBar(this);
    scroll->setOrientation(Qt::Horizontal);
    ui->centralWidget->layout()->addWidget(scroll);

    dial = new QDial(this);
    dial->setOrientation(Qt::Horizontal);
    ui->centralWidget->layout()->addWidget(dial);

    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
    connect(scroll, SIGNAL(valueChanged(int)), this, SLOT(onScrollValueChanged(int)));
    connect(dial, SIGNAL(valueChanged(int)), this, SLOT(onDialValueChanged(int)));
*/
    createScrollBars(tr("Parameter Controls"));
    ui->centralWidget->layout()->addWidget(scrollBarsGroup);

    createSettings(tr("Range Settings"));
    ui->centralWidget->layout()->addWidget(settingsGroup);

//! [1]
    serial = new QSerialPort(this);
//! [1]
    settings = new SettingsDialog;

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    status = new QLabel;
    ui->statusBar->addWidget(status);

    initActionsConnections();

    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MainWindow::handleError);

//! [2]
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
//! [2]
    connect(console, &Console::getData, this, &MainWindow::writeData);
//! [3]
}
//! [3]

void MainWindow::createScrollBars(const QString &title)
{
    scrollBarsGroup = new QGroupBox(title);
    frequencyLabel  = new QLabel(tr("Frequency(Hz)"));
    voltageLabel    = new QLabel(tr("Voltage(V)"));
    phaseLabel      = new QLabel(tr("Phase(Degree)"));

    frequencySpin = new QSpinBox;
    frequencySpin->setRange(-100, 100);
    frequencySpin->setSingleStep(1);
    frequencySpin->setFixedWidth(50);

    voltageSpin = new QSpinBox;
    voltageSpin->setRange(-100, 100);
    voltageSpin->setSingleStep(1);
    voltageSpin->setFixedWidth(50);

    phaseSpin = new QSpinBox;
    phaseSpin->setRange(-100, 100);
    phaseSpin->setSingleStep(1);
    phaseSpin->setFixedWidth(50);

    frequency = new QScrollBar(this);
    frequency->setOrientation(Qt::Horizontal);
    frequency->setFixedWidth(280);

    voltage = new QScrollBar(this);
    voltage->setOrientation(Qt::Horizontal);
    voltage->setFixedWidth(280);

    phase = new QScrollBar(this);
    phase->setOrientation(Qt::Horizontal);
    phase->setFixedWidth(280);

    frequency->setValue(50);
    voltage->setValue(110);
    phase->setValue(0);

    connect(frequency, SIGNAL(valueChanged(int)), this, SLOT(onFrequencyValueChanged(int)));
    connect(voltage, SIGNAL(valueChanged(int)), this, SLOT(onVoltageValueChanged(int)));
    connect(phase, SIGNAL(valueChanged(int)), this, SLOT(onPhaseValueChanged(int)));

    QGridLayout *scrollBarsLayout = new QGridLayout;
    scrollBarsLayout->addWidget(frequencyLabel, 0, 0);
    scrollBarsLayout->addWidget(voltageLabel, 1, 0);
    scrollBarsLayout->addWidget(phaseLabel, 2, 0);
    scrollBarsLayout->addWidget(frequencySpin, 0, 1);
    scrollBarsLayout->addWidget(voltageSpin, 1, 1);
    scrollBarsLayout->addWidget(phaseSpin, 2, 1);
    scrollBarsLayout->addWidget(frequency, 0, 2);
    scrollBarsLayout->addWidget(voltage, 1, 2);
    scrollBarsLayout->addWidget(phase, 2, 2);

    scrollBarsGroup->setLayout(scrollBarsLayout);
}

#define SMB_WIDTH   70

void MainWindow::createSettings(const QString &title)
{
    settingsGroup = new QGroupBox(title);
    minFrequencyLabel = new QLabel(tr("Frequency Min"));
    minFrequencyLabel->setFixedWidth(SMB_WIDTH);
    maxFrequencyLabel = new QLabel(tr("Max"));
    maxFrequencyLabel->setFixedWidth(SMB_WIDTH/2);
    minFrequencyBox = new QSpinBox;
    minFrequencyBox->setRange(10, 50);
    minFrequencyBox->setSingleStep(1);
    minFrequencyBox->setFixedWidth(SMB_WIDTH);
    maxFrequencyBox = new QSpinBox;
    maxFrequencyBox->setRange(40, 90);
    maxFrequencyBox->setSingleStep(1);
    maxFrequencyBox->setFixedWidth(SMB_WIDTH);

    minVoltageLabel = new QLabel(tr("Voltage Min"));
    maxVoltageLabel = new QLabel(tr("Max"));
    minVoltageBox = new QSpinBox;
    minVoltageBox->setRange(20, 100);
    minVoltageBox->setSingleStep(1);
    minVoltageBox->setFixedWidth(SMB_WIDTH);
    maxVoltageBox = new QSpinBox;
    maxVoltageBox->setRange(60, 240);
    maxVoltageBox->setSingleStep(1);
    maxVoltageBox->setFixedWidth(SMB_WIDTH);

    minPhaseLabel = new QLabel(tr("Phase Min"));
    maxPhaseLabel = new QLabel(tr("Max"));
    minPhaseBox = new QSpinBox;
    minPhaseBox->setRange(-90, 0);
    minPhaseBox->setSingleStep(1);
    minPhaseBox->setFixedWidth(SMB_WIDTH);
    maxPhaseBox = new QSpinBox;
    maxPhaseBox->setRange(0, 90);
    maxPhaseBox->setSingleStep(1);
    maxPhaseBox->setFixedWidth(SMB_WIDTH);

    minFrequencyBox->setValue(40);
    maxFrequencyBox->setValue(70);
    minVoltageBox->setValue(70);
    maxVoltageBox->setValue(110);
    minPhaseBox->setValue(-45);
    maxPhaseBox->setValue(45);

    connect(minFrequencyBox, SIGNAL(valueChanged(int)), this, SLOT(setFrequencyMinimum(int)));
    connect(maxFrequencyBox, SIGNAL(valueChanged(int)), this, SLOT(setFrequencyMaximum(int)));
    connect(minVoltageBox,   SIGNAL(valueChanged(int)), this, SLOT(setVoltageMinimum(int)));
    connect(maxVoltageBox,   SIGNAL(valueChanged(int)), this, SLOT(setVoltageMaximum(int)));
    connect(minPhaseBox,     SIGNAL(valueChanged(int)), this, SLOT(setPhaseMinimum(int)));
    connect(maxPhaseBox,     SIGNAL(valueChanged(int)), this, SLOT(setPhaseMaximum(int)));

    QGridLayout *settingsLayout = new QGridLayout;
    settingsLayout->addWidget(minFrequencyLabel, 0, 0);
    settingsLayout->addWidget(maxFrequencyLabel, 0, 2);
    settingsLayout->addWidget(minFrequencyBox,   0, 1);
    settingsLayout->addWidget(maxFrequencyBox,   0, 3);

    settingsLayout->addWidget(minVoltageLabel, 1, 0);
    settingsLayout->addWidget(maxVoltageLabel, 1, 2);
    settingsLayout->addWidget(minVoltageBox,   1, 1);
    settingsLayout->addWidget(maxVoltageBox,   1, 3);

    settingsLayout->addWidget(minPhaseLabel, 2, 0);
    settingsLayout->addWidget(maxPhaseLabel, 2, 2);
    settingsLayout->addWidget(minPhaseBox,   2, 1);
    settingsLayout->addWidget(maxPhaseBox,   2, 3);

    settingsLayout->setAlignment(Qt::AlignLeft);
    settingsGroup->setLayout(settingsLayout);
}

void MainWindow::setFrequencyMinimum(int value)
{
    frequency->setMinimum(value);
}

void MainWindow::setFrequencyMaximum(int value)
{
    frequency->setMaximum(value);
}

void MainWindow::setVoltageMinimum(int value)
{
    voltage->setMinimum(value);
}

void MainWindow::setVoltageMaximum(int value)
{
    voltage->setMaximum(value);

}

void MainWindow::setPhaseMinimum(int value)
{
     phase->setMinimum(value);
}

void MainWindow::setPhaseMaximum(int value)
{
    phase->setMaximum(value);
}

void MainWindow::on_valueChanged(int value)
{
    qDebug() << value;
    // ui->progressBar->setValue(value);
    value *=20;
    if(value < 20){
        value = 20;
    }
    if (value > 4000){
        value = 4000;
    }
    QString temp = QString::number(value,10);
    while (temp.length()<4){
        temp = "0" + temp;
    }
    //QString temp = QString::number(value*20,10);
    qDebug() << temp;
    // sliderValue = temp;
    writeData(temp.toUtf8());
}

void MainWindow::onFrequencyValueChanged(int value)
{
    on_valueChanged(value);
}


void MainWindow::onVoltageValueChanged(int value)
{
    on_valueChanged(value);
}

// scroll->setValue(value);
// slider->setValue(value);
void MainWindow::onPhaseValueChanged(int value)
{
    on_valueChanged(value);
}

MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}

//! [4]
void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
        console->setEnabled(true);
        console->setLocalEchoEnabled(p.localEchoEnabled);
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);
        showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        showStatusMessage(tr("Open error"));
    }
}
//! [4]

//! [5]
void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    console->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    showStatusMessage(tr("Disconnected"));
}
//! [5]

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Simple Terminal"),
                       tr("The <b>Simple Terminal</b> example demonstrates how to "
                          "use the Qt Serial Port module in modern GUI applications "
                          "using Qt, with a menu bar, toolbars, and a status bar."));
}

//! [6]
void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}
//! [6]

//! [7]
void MainWindow::readData()
{
    QByteArray data = serial->readAll();
    console->putData(data);
}
//! [7]

//! [8]
void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}
//! [8]

void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionConfigure, &QAction::triggered, settings, &SettingsDialog::show);
    connect(ui->actionClear, &QAction::triggered, console, &Console::clear);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
    connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::showStatusMessage(const QString &message)
{
    status->setText(message);
}
