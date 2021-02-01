#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>

#include "opencv2/opencv.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void ConfigWindow();
    void ConstructMenu();
    void ConstructCentralWidget();
    void Connector();
    ~MainWindow();

    void SaveImage(cv::Mat img, QString path);
public slots:
    void OpenFolder();
    void OpenFile();
    void CheckPath();
    void CountImage();
    void ApplyEnhancement();
    void OpenSaveFolder();
    void UpdateSaveLabel();

signals:
    void ImagesPathChanged();
    void SavePathChanged();

private:
    Ui::MainWindow *ui;
    QMenu *openFolderMenu;
    QWidget *central;

    QCheckBox *compressionCheckBox = nullptr;
    QCheckBox *centerCheckBox = nullptr;
    QCheckBox *uniformizeBgCheckBox = nullptr;
    QCheckBox *watermarkCheckBox = nullptr;
    QCheckBox *illuminationCorrectionCheckBox = nullptr;

    QPushButton *chooseDstBtn= nullptr;
    QPushButton *enhanceButton = nullptr;

    QLabel * savePathLabel;

    QString imagesPath = NULL;
    QString savePath = NULL;
    int nbImages = -1;
    bool isFolder = false;
};
#endif // MAINWINDOW_H
