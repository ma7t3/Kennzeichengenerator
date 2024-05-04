#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->leText, &QLineEdit::textChanged, this, &MainWindow::refreshPreview);
    connect(ui->cbNarrowFont, &QCheckBox::stateChanged, this, &MainWindow::refreshPreview);
    connect(ui->cbTextColor, &QComboBox::currentIndexChanged, this, &MainWindow::refreshPreview);
    connect(ui->cbType, &QComboBox::currentIndexChanged, this, &MainWindow::refreshPreview);
    connect(ui->deHuDate, &QDateEdit::dateChanged, this, &MainWindow::refreshPreview);
    connect(ui->cbStateEmblem, &QComboBox::currentIndexChanged, this, &MainWindow::refreshPreview);
    connect(ui->leDisctrictName, &QLineEdit::textChanged, this, &MainWindow::refreshPreview);
    connect(ui->sbImgHeight, &QSpinBox::valueChanged, this, &MainWindow::refreshPreview);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::refreshPreview() {
    // refresh here
}
