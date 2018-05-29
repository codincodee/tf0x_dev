#ifndef CART_TEST_MULTI_RESULT_SHEETS_H
#define CART_TEST_MULTI_RESULT_SHEETS_H

#include "cart_test_result_sheet.h"
#include <memory>
#include <vector>
#include "export.h"

namespace tf0x_common {
class API CartTestMultiResultSheets
{
 public:
  CartTestMultiResultSheets();
  void AddSheet(std::shared_ptr<CartTestResultSheet> sheet);
  std::shared_ptr<CartTestResultSheet> CurrentSheet();
  virtual ~CartTestMultiResultSheets();
  void SheetDone();
 private:
  std::vector<std::shared_ptr<CartTestResultSheet>> sheets_;
  int current_index_;
};
}

#endif // CART_TEST_MULTI_RESULT_SHEETS_H
