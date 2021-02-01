#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "enhancer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ConfigWindow();
    ConstructMenu();
    ConstructCentralWidget();
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
 * @brief MainWindow::ConstructCentralWidget
 * Initialize and add checkbox/button to the central widget
 */
void MainWindow::ConstructCentralWidget(){
    compressionCheckBox = new QCheckBox("Compress images");
    centerCheckBox = new QCheckBox("Center images");
    uniformizeBgCheckBox = new QCheckBox("Uniformize background");
    watermarkCheckBox = new QCheckBox("Apply Watermark");
    illuminationCorrectionCheckBox = new QCheckBox("Uniformize lightning");

    chooseDstBtn = new QPushButton("Choose destination folder");
    enhanceButton = new QPushButton("Enhance");

    savePath = QDir::currentPath();
    savePathLabel = new QLabel();
    savePathLabel->setText("Saving directory : " + savePath);
    savePathLabel->setWordWrap(true);

    central = new QWidget();
    setCentralWidget(central);
    QVBoxLayout * layout = new QVBoxLayout(centralWidget());

    layout->addWidget(compressionCheckBox);
    layout->addWidget(centerCheckBox);
    layout->addWidget(uniformizeBgCheckBox);
    layout->addWidget(watermarkCheckBox);
    layout->addWidget(illuminationCorrectionCheckBox);
    layout->addWidget(chooseDstBtn);
    layout->addWidget(savePathLabel);
    layout->addWidget(enhanceButton);
}

/**
 * @brief MainWindow::Connector
 * Connect custom signals to their function
 */
void MainWindow::Connector(){
    connect(this, SIGNAL(ImagesPathChanged()), this, SLOT(CheckPath()));
    connect(this, SIGNAL(ImagesPathChanged()), this, SLOT(CountImage()));
    connect(enhanceButton, SIGNAL(clicked()), this, SLOT(ApplyEnhancement()));
    connect(chooseDstBtn, SIGNAL(clicked()), this, SLOT(OpenSaveFolder()));
    connect(this, SIGNAL(SavePathChanged()), this, SLOT(UpdateSaveLabel()));
}

//---------FILES RELATED FUNCTIONS-----------//

/**
 * @brief MainWindow::CountImage
 * Instanciate enhancer and launch process.
 */
void MainWindow::ApplyEnhancement(){
    Enhancer enhancer = Enhancer(compressionCheckBox->isChecked(),centerCheckBox->isChecked(),uniformizeBgCheckBox->isChecked(),watermarkCheckBox->isChecked(),illuminationCorrectionCheckBox->isChecked());

    if(isFolder){
        QDir directory(imagesPath);
        QStringList imagesList = directory.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.jpeg" << "*.JPEG", QDir::Files);
        for(auto path : imagesList){
            cv::Mat res = enhancer.process(imagesPath + "/" + path);
            SaveImage(res, savePath + "/" + path);
        }
    }else{
        cv::Mat res = enhancer.process(imagesPath);
        QFile f(imagesPath);
        QFileInfo fileInfo(f.fileName());
        QString filename(fileInfo.fileName());
        SaveImage(res, savePath + "/" + filename);
    }
}

/**
 * @brief MainWindow::SaveImage
 * @param img to save
 * @param path
 * @return true if saved
 */
void MainWindow::SaveImage(cv::Mat img, QString path){
    if(!cv::imwrite(path.toStdString(), img)){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Failed save of image : " + path);
        msgBox.exec();
    }
}

/**
 * @brief MainWindow::CheckPath
 * Check if path is a folder or an image, update isFolder value.
 */
void MainWindow::CheckPath(){
    if(imagesPath != NULL){
        const QFileInfo dir(imagesPath);
        if(dir.exists() && dir.isDir()){
            isFolder = true;
        }else{
            isFolder = false;
        }
    }
}

/**
 * @brief MainWindow::CountImage
 * Count image in opened file or dir.
 * Set isFolder value.
 */
void MainWindow::CountImage(){
    if(isFolder){
        QDir directory(imagesPath);
        //Filter only image and not actual and parent reference
        QStringList imagesList = directory.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png" << "*.PNG" << "*.jpeg" << "*.JPEG", QDir::Files);
        int cpt = imagesList.count();
        if(cpt == 0){
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("There is no images in this folder. JPG, PNG and JPEG are compatible file formats.");
            msgBox.exec();
        }
        nbImages = cpt;
    }else{
        nbImages = 1;
    }
}

/**
 * @brief MainWindow::OpenSaveFolder
 * Open QFileDialog for choosing save folder
 */
void MainWindow::OpenSaveFolder(){
    savePath = QFileDialog::getExistingDirectory(this, tr("Open images folder"), ".", QFileDialog::ShowDirsOnly);
    emit SavePathChanged();
}

void MainWindow::UpdateSaveLabel(){
    savePathLabel->setText("Saving directory : " + savePath);
}

//-----------MENU FUNCTIONS------------//

/**
 * @brief MainWindow::OpenFolder
 * Open a QFileDialog
 */
void MainWindow::OpenFolder(){
    imagesPath = QFileDialog::getExistingDirectory(this, tr("Open images folder"), ".", QFileDialog::ShowDirsOnly);
    emit ImagesPathChanged();
}

/**
 * @brief MainWindow::OpenFolder
 * Open a QFileDialog
 */
void MainWindow::OpenFile(){
    imagesPath = QFileDialog::getOpenFileName(this, tr("Open Image"), "~", tr("Image Files (*.png *.jpg *.jpeg)"));
    emit ImagesPathChanged();
}

MainWindow::~MainWindow()
{
    delete ui;
}

