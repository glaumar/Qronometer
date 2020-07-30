#ifndef CSVEXPORTER_H
#define CSVEXPORTER_H

#include <AbstractExporter.h>

class CSVExporter : public AbstractExporter
{
  public:
    CSVExporter(QAbstractItemModel* model)
      : AbstractExporter(model){};
    void exportTo(QString path) const override;
};

#endif // CSVEXPORTER_H
