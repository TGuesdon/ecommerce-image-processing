#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>


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

public slots:
    void OpenFolder();
    void OpenFile();
    void CountImage();
    void ApplyEnhancement();

signals:
    void ImagesPathChanged(QString newPath);

private:
    Ui::MainWindow *ui;
    QMenu *openFolderMenu;
    QWidget *central;

    QCheckBox *compressionCheckBox = nullptr;
    QCheckBox *centerCheckBox = nullptr;
    QCheckBox *uniformizeBgCheckBox = nullptr;
    QCheckBox *watermarkCheckBox = nullptr;
    QCheckBox *uniformizeLCheckBox = nullptr;

    QPushButton *enhanceButton = nullptr;

    QString imagesPath = NULL;
    int nbImages = -1;
    bool isFolder = false;
};
#endif // MAINWINDOW_H
