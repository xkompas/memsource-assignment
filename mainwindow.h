#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include "response.h"
#include "project.h"

QT_FORWARD_DECLARE_CLASS(Connector)
QT_FORWARD_DECLARE_CLASS(ProjectsModel)

using Languages = QMap<QString, QString>;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:
    void initialize();

protected:
    void showEvent(QShowEvent* event) override;

private:
    Ui::MainWindow *ui;
    Connector *connector;
    QString apiToken;
    ProjectsModel *projectsModel;
    Languages languages;

    QString codeToLanguage(const QString code) const;

    void performLogin(const QString error = "");
    void login(const QString userName, const QString password);
    void processLoginResponse(const Response &response);
    QString parseLoginJson(const QJsonDocument &jsonDocument);

    void loadLanguages();
    void processLanguagesResponse(const Response &response);
    Languages parseLanguagesJson(const QJsonDocument &jsonDocument);

    void loadProjects();
    void processProjectsResponse(const Response &response);
    QList<Project> parseProjectsJson(const QJsonDocument &jsonDocument);
};

#endif // MAINWINDOW_H
