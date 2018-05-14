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

void QtHelpers::UpdateComboBoxWithoutCurrentChanged(
    QComboBox &combo, QStringList &new_list) {
  auto current = combo.currentText();
  if (new_list.contains(current)) {
    new_list.removeOne(current);
  } else {
    combo.clear();
  }
  for (int i = 0; i < combo.count(); ++i) {
    if (combo.itemText(i) == current) {
      continue;
    } else {
      combo.removeItem(i);
    }
  }
  for (auto& item : new_list) {
    combo.addItem(item);
  }
}

