#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>
#include <QFileDialog>

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
    void Connector();
    ~MainWindow();

public slots:
    void OpenFolder();
    void OpenFile();
    void CountImage();

signals:
    void ImagesPathChanged(QString newPath);

private:
    Ui::MainWindow *ui;
    QMenu *openFolderMenu;

    QString imagesPath = NULL;
    int nbImages = -1;
};
#endif // MAINWINDOW_H
