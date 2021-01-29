#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"
#include "enhancer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ConfigWindow();
    ConstructMenu();
    Connector();
}

/**
 * @brief MainWindow::ConfigWindow
 * Configure various parameters of MainWindow
 */
void MainWindow::ConfigWindow(){
    setWindowTitle("Ecommerce image processing");
}

/**
 * @brief MainWindow::ConstructMenu
 * Initialize various QMenu and their QAction
 */
void MainWindow::ConstructMenu(){
    openFolderMenu = new QMenu("File");
    menuBar()->addMenu(openFolderMenu);

    QAction * openFolder = new QAction("Open folder", this);
    openFolder->setShortcut(QKeySequence::Open);
    connect(openFolder, SIGNAL(triggered()), this, SLOT(OpenFolder()));

    QAction * openFile = new QAction("Open file", this);
    connect(openFile, SIGNAL(triggered()), this, SLOT(OpenFile()));

    openFolderMenu->addAction(openFolder);
    openFolderMenu->addAction(openFile);

}

/**
 * @brief MainWindow::Connector
 * Connect custom signals to their function
 */
void MainWindow::Connector(){
    connect(this, SIGNAL(ImagesPathChanged(QString)), this, SLOT(CountImage()));
    connect(this, SIGNAL(ImagesPathChanged(QString)), this, SLOT(ApplyEnhancement()));
}

//---------FILES RELATED FUNCTIONS-----------//

/**
 * @brief MainWindow::CountImage
 * Instanciate enhancer and launch process.
 */
void MainWindow::ApplyEnhancement(){
    Enhancer enhancer = Enhancer(true,true,true,true,true);

    if(isFolder){
        QDir directory(imagesPath);
        QStringList imagesList = directory.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG", QDir::Files);
        for(auto path : imagesList){
            enhancer.process(imagesPath + "/" + path);
        }
    }else{
        enhancer.process(imagesPath);
    }
}

/**
 * @brief MainWindow::CountImage
 * Count image in opened file or dir.
 * Set isFolder value.
 */
void MainWindow::CountImage(){
    if(imagesPath != NULL){
        const QFileInfo dir(imagesPath);
        if(dir.exists() && dir.isDir()){
            QDir directory(imagesPath);
            //Filter only image and not actual and parent reference
            directory.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
            QStringList imagesList = directory.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG", QDir::Files);
            int cpt = imagesList.count();
            if(cpt == 0){
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText("There is no images in this folder. JPG and PNG are compatible file formats.");
                msgBox.exec();
            }
            nbImages = cpt;
            isFolder = true;
        }else{
            nbImages = 1;
            isFolder = false;
        }
    }
}

//-----------MENU FUNCTIONS------------//

/**
 * @brief MainWindow::OpenFolder
 * TODO:Open a QFileDialog
 */
void MainWindow::OpenFolder(){
    imagesPath = QFileDialog::getExistingDirectory(this, tr("Open images folder"), ".", QFileDialog::ShowDirsOnly);
    emit ImagesPathChanged(imagesPath);
}

/**
 * @brief MainWindow::OpenFolder
 * TODO:Open a QFileDialog
 */
void MainWindow::OpenFile(){
    imagesPath = QFileDialog::getOpenFileName(this, tr("Open Image"), "~", tr("Image Files (*.png *.jpg *.bmp)"));
    emit ImagesPathChanged(imagesPath);
}

MainWindow::~MainWindow()
{
    delete ui;
}

