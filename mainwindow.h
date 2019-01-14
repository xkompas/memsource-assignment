#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "response.h"
#include "project.h"

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

    void processProjectsResponse(const Response &response);
    QList<Project> parseProjectsJson(const QJsonDocument &jsonDocument);
};

#endif // MAINWINDOW_H
