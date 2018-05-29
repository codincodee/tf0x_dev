#include "cart_test_result_sheet.h"
#include <sstream>
#include <QScrollBar>

namespace tf0x_common {
CartTestResultSheet::CartTestResultSheet(
    QListWidget* id, QListWidget* cart_pos, QListWidget* dist)
  : id_widget_(id),
    cart_position_widget_(cart_pos),
    distance_widget_(dist)
{
  cart_position_widget_->verticalScrollBar()->setVisible(false);
  distance_widget_->verticalScrollBar()->setVisible(false);

  widgets_.push_back(id_widget_);
  widgets_.push_back(cart_position_widget_);
  widgets_.push_back(distance_widget_);

  if (id_widget_) {
    connect(
        id_widget_, SIGNAL(currentRowChanged(int)),
        this, SLOT(OnCurrentRowChanged(int)));
    auto scroll = id_widget_->verticalScrollBar();
    if (scroll) {
      connect(scroll, SIGNAL(valueChanged(int)), this, SLOT(OnScrollValueChanged(int)));
    }
  }
}

CartTestResultSheet::~CartTestResultSheet() {

}

void CartTestResultSheet::OnCurrentRowChanged(int i) {
  if (cart_position_widget_) {
    cart_position_widget_->setCurrentRow(i);
  }
  if (distance_widget_) {
    distance_widget_->setCurrentRow(i);
  }
}

void CartTestResultSheet::OnScrollValueChanged(int i) {
  if (cart_position_widget_) {
    cart_position_widget_->verticalScrollBar()->setValue(i);
  }
  if (distance_widget_) {
    distance_widget_->verticalScrollBar()->setValue(i);
  }
}

int CartTestResultSheet::Size() {
  if (!id_widget_) {
    return 0;
  }
  return id_widget_->count();
}

CartTestEntry CartTestResultSheet::GetLastEntry() {
  CartTestEntry entry;
  // ID
  if (!id_widget_) {
    return entry;
  }
  if (!id_widget_->count()) {
    return entry;
  }
  auto item = id_widget_->item(id_widget_->count() - 1);
  if (!item) {
    return entry;
  }
  entry.id = item->text().toInt();

  // Cart Position
  if (cart_position_widget_) {
    if (cart_position_widget_->count()) {
      auto im = cart_position_widget_->item(cart_position_widget_->count() - 1);
      if (im) {
        entry.pos = im->text().toFloat();
      }
    }
  }

  // Distance
  if (distance_widget_) {
    if (distance_widget_->count()) {
      auto im = distance_widget_->item(distance_widget_->count() - 1);
      if (im) {
        entry.dist = im->text().toFloat();
      }
    }
  }
  return entry;
}

void CartTestResultSheet::AddEntry(const CartTestEntry &entry) {
  if (!id_widget_) {
    return;
  }
  id_widget_->addItem(QString::number(entry.id));
  if (cart_position_widget_) {
    cart_position_widget_->addItem(QString::number(entry.pos));
  }
  if (distance_widget_) {
    distance_widget_->addItem(QString::number(entry.dist));
  }
  // MoveCursorToLastItem();
  id_widget_->setCurrentRow(id_widget_->count() - 1);
}

void CartTestResultSheet::MoveCursorToLastItem() {
  for (auto& widget : widgets_) {
    widget->setCurrentRow(widget->count() - 1);
  }
}
}
