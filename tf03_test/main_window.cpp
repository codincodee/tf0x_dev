#include "main_window.h"
#include "ui_main_window.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  this->resize(800, 800);
  ui->tabWidget->setCurrentIndex(0);

  InitializeSettingsPage();
  InitializeReadingsPage();
  InitializeCommandPageCartSection();
  InitializeTimerEvent();
}

MainWindow::~MainWindow()
{
  delete ui;
  if (main_chart_) {
    delete main_chart_;
    main_chart_ = nullptr;
  }
  if (main_chart_view_) {
    delete main_chart_view_;
    main_chart_view_ = nullptr;
  }
  SaveSettingsToConfigFile();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
  if (index == 3) {
    EnteringSettingsPage();
  }
  if (previous_page_ != index) {
    if (previous_page_ == 3) {
      LeavingSettingsPage();
    }
  }
  previous_page_ = index;
}
