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
  InitializeCartPage();

  elapsed_timer_.start();

  sensor_thread_.reset(new std::thread(&MainWindow::SensorThread, this));
  cart_thread_.reset(new std::thread(&MainWindow::CartThread, this));
  sensor_log_.reset(new std::list<tf03_driver::Measurement>);
}

MainWindow::~MainWindow()
{
  delete ui;
//  if (main_chart_) {
//    delete main_chart_;
//    main_chart_ = nullptr;
//  }
//  if (main_chart_view_) {
//    delete main_chart_view_;
//    main_chart_view_ = nullptr;
//  }
  SaveSettingsToConfigFile();

  sensor_thread_exit_signal_ = true;
  if (sensor_thread_) {
    if (sensor_thread_->joinable()) {
      sensor_thread_->join();
    }
  }

  cart_thread_exit_signal_ = true;
  if (cart_thread_) {
    if (cart_thread_->joinable()) {
      cart_thread_->join();
    }
  }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
  constexpr int kSettingsPageIndex = 4;
  if (index == kSettingsPageIndex) {
    EnteringSettingsPage();
  }
  if (previous_page_ != index) {
    if (previous_page_ == kSettingsPageIndex) {
      LeavingSettingsPage();
    }
  }
  previous_page_ = index;
}
