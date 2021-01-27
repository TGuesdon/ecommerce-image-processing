#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>

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
    ~MainWindow();

public slots:
    void OpenFolder();
    void OpenFile();

private:
    Ui::MainWindow *ui;
    QMenu *openFolderMenu;
};
#endif // MAINWINDOW_H
