#ifndef ABSTRACTEXPORTER_H
#define ABSTRACTEXPORTER_H

#include <QAbstractItemModel>

class AbstractExporter
{
  public:
    AbstractExporter(QAbstractItemModel* model)
      : model_(model)
    {}

    void setModel(QAbstractItemModel* model) { model_ = model; }

    virtual void exportTo(QString path) const = 0;

  protected:
    QAbstractItemModel* model_;
};
#endif // ABSTRACTEXPORTER_H
