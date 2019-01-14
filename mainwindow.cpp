#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <functional>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "projectsmodel.h"
#include "connector.h"
// TODO add login
#include "api_token.h"

namespace  {

QDateTime parseDate(const QString text)
{
    // TODO time zones
    auto lastPlus = text.lastIndexOf("+");
    auto dateString{(lastPlus == -1) ? text : text.left(lastPlus)};
    return QDateTime::fromString(dateString, "yyyy-MM-ddTHH:mm:ss");
}

}

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    projectsModel(new ProjectsModel)
{
    ui->setupUi(this);

    ui->projectsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->projectsTableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->projectsTableView->setModel(projectsModel);

    QObject::connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));

    loadProjects();
}

void MainWindow::loadProjects()
{
    Connector *connector = new Connector(this);
    RequestParams params;
    params.insert("token", API_TOKEN);
    connector->get("/api2/v1/projects", params, std::bind(&MainWindow::processProjectsResponse, this, std::placeholders::_1));
}

void MainWindow::processProjectsResponse(const Response &response)
{
    if (!response.ok)
    {
        // TODO display error
        return;
    }

    ProjectList projectList = parseProjectsJson(response.jsonDocument);
    projectsModel->load(projectList);
}

QList<Project> MainWindow::parseProjectsJson(const QJsonDocument &jsonDocument)
{
    QList<Project> projectList;

    if (jsonDocument.isObject())
    {
        QJsonObject root = jsonDocument.object();
        QJsonArray content = root["content"].toArray();

        foreach (const QJsonValue &projectValue, content)
        {
            QJsonObject projectObject = projectValue.toObject();

            Project project;
            project.name = projectObject["name"].toString();

            // TODO translate languages
            project.sourceLanguage = projectObject["sourceLang"].toString();

            QJsonArray targetLanguages = projectObject["targetLangs"].toArray();
            foreach (const QJsonValue &targetLanguageValue, targetLanguages)
            {
                project.targetLanguages.append(targetLanguageValue.toString());
            }

            project.created = parseDate(projectObject["dateCreated"].toString());

            projectList.append(project);
        }
    }

    return projectList;
}

MainWindow::~MainWindow()
{
    delete ui;
}
