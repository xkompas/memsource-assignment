#include "projectsmodel.h"

ProjectsModel::ProjectsModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant ProjectsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section) {
        case 0:
            return QString(tr("Project"));
        case 1:
            return QString(tr("Source Language"));
        case 2:
            return QString(tr("Target Languages"));
        case 3:
            return QString(tr("Created"));
        }
        // fall through
    }
    return QVariant();
}

int ProjectsModel::rowCount(const QModelIndex &) const
{
    return projectList.count();
}

int ProjectsModel::columnCount(const QModelIndex &) const
{
    return 4;
}

QVariant ProjectsModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole)
    {
        const Project &project = projectList.at(index.row());
        switch (index.column()) {
        case 0:
            return project.name;
        case 1:
            return project.sourceLanguage;
        case 2:
            return project.targetLanguages.join(", ");
        case 3:
            return project.created.isValid() ? project.created.toString("d MMM yyyy HH:mm") : QString("?");
        }
        // fall through
    }

    return QVariant();
}

void ProjectsModel::load(const ProjectList &aProjectList)
{
    beginResetModel();
    projectList = aProjectList;
    endResetModel();
}
