#ifndef QT_HELPERS_H
#define QT_HELPERS_H

#include <QString>
#include <QWidget>
#include "export.h"
#include <QComboBox>

class API QtHelpers
{
 public:
  static QString SelectFolder(QWidget* parent);
  static QString SelectFolder(QWidget* parent, QString& from);
  static QString ChooseFile(QWidget* parent);
  static QString ChooseFile(QWidget* parent, QString& from);
  static QString SystemDocumentFolderPath();
  static QString SystemHomeFolderPath();
  static void UpdateComboBoxWithoutCurrentChanged(
      QComboBox& combo, QStringList& new_list);
 private:
  static QString SelectFileFrom();
};

#endif // QT_HELPERS_H
