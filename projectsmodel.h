#ifndef PROJECTSMODEL_H
#define PROJECTSMODEL_H

#include <QAbstractTableModel>

class ProjectsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ProjectsModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // PROJECTSMODEL_H