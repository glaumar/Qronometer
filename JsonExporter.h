#ifndef JSONEXPORTER_H
#define JSONEXPORTER_H

#include <AbstractExporter.h>

class JsonExporter : public AbstractExporter
{
  public:
    JsonExporter(QAbstractItemModel* model)
      : AbstractExporter(model)
    {}

    void exportTo(QString path) const override;
};

#endif // JSONEXPORTER_H
