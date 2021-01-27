#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ConfigWindow();
    ConstructMenu();
}

void MainWindow::ConfigWindow(){
    setWindowTitle("Ecommerce image processing");
}

void MainWindow::ConstructMenu(){
    openFolderMenu = new QMenu("File");
    menuBar()->addMenu(this->openFolderMenu);

    QAction * openFolder = new QAction("Open folder", this);
    openFolder->setShortcut(QKeySequence::Open);
    connect(openFolder, SIGNAL(triggered()), this, SLOT(OpenFolder()));

    QAction * openFile = new QAction("Open file", this);
    connect(openFile, SIGNAL(triggered()), this, SLOT(OpenFile()));

    openFolderMenu->addAction(openFolder);
    openFolderMenu->addAction(openFile);

}

void MainWindow::OpenFolder(){
    qDebug() << "Open folder";
}

void MainWindow::OpenFile(){
    qDebug() << "Open file";
}

MainWindow::~MainWindow()
{
    delete ui;
}

