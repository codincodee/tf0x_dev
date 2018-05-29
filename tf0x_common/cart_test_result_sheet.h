#ifndef TF0X_COMMON_CART_TEST_RESULT_SHEET_H_
#define TF0X_COMMON_CART_TEST_RESULT_SHEET_H_

#include "export.h"
#include <QListWidget>
#include <QObject>

namespace tf0x_common {
struct CartTestEntry {
  CartTestEntry() : id(0), pos(0.0f), dist(0.0f) {}
  int id;
  float pos;
  float dist;
};

class API CartTestResultSheet : public QObject
{
  Q_OBJECT
 public:
  CartTestResultSheet(QListWidget* id, QListWidget* cart_pos, QListWidget* dist);
  CartTestEntry GetLastEntry();
  int Size();
  void AddEntry(const CartTestEntry& entry);
  virtual ~CartTestResultSheet();
  void Clear();
  CartTestEntry At(const int& index);
 private slots:
  void OnCurrentRowChanged(int i);
  void OnScrollValueChanged(int i);
 private:
  void MoveCursorToLastItem();

  QListWidget* id_widget_;
  QListWidget* cart_position_widget_;
  QListWidget* distance_widget_;
  std::vector<QListWidget*> widgets_;
};
}

#endif // CART_TEST_RESULT_SHEET_H
