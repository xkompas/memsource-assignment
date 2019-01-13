#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_FORWARD_DECLARE_CLASS(ProjectsModel)

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadProjects();

private:
    Ui::MainWindow *ui;
    ProjectsModel *projectsModel;
};

#endif // MAINWINDOW_H
