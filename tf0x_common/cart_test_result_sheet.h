#ifndef TF0X_COMMON_CART_TEST_RESULT_SHEET_H_
#define TF0X_COMMON_CART_TEST_RESULT_SHEET_H_

#include "export.h"
#include <QPlainTextEdit>
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
  CartTestResultSheet(QPlainTextEdit* id, QPlainTextEdit* cart_pos, QPlainTextEdit* dist);
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

  QPlainTextEdit* id_widget_;
  QPlainTextEdit* cart_position_widget_;
  QPlainTextEdit* distance_widget_;
  std::vector<QPlainTextEdit*> widgets_;
};
}

#endif // CART_TEST_RESULT_SHEET_H
