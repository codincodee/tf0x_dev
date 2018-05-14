#include "qt_helpers.h"
#include <QFileDialog>
#include <QStandardPaths>

QString QtHelpers::SelectFolder(QWidget *parent) {
  return SelectFolder(parent, SelectFileFrom());
}

QString QtHelpers::SelectFolder(QWidget *parent, QString &from) {
  return QFileDialog::getExistingDirectory(
      parent,
      "Select Folder",
      from,
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}

QString QtHelpers::SelectFileFrom() {
  return SystemHomeFolderPath();
}

QString QtHelpers::ChooseFile(QWidget *parent) {
  return ChooseFile(parent, SelectFileFrom());
}

QString QtHelpers::ChooseFile(QWidget *parent, QString &from) {
  return QFileDialog::getOpenFileName(
      parent,
      "Select File",
      from);
}

QString QtHelpers::SystemDocumentFolderPath() {
  return
      QStandardPaths::locate(
          QStandardPaths::DocumentsLocation, QString(),
          QStandardPaths::LocateDirectory);
}

QString QtHelpers::SystemHomeFolderPath() {
  return
      QStandardPaths::locate(
          QStandardPaths::HomeLocation, QString(),
          QStandardPaths::LocateDirectory);
}

