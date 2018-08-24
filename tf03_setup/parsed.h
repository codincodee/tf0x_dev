#ifndef PARSED_H
#define PARSED_H

#include <QString>
#include "lingual.h"

struct ParsedBase {
  unsigned long id = 0;
  virtual ~ParsedBase();
};

struct StatusEcho : public ParsedBase {
  Lingual command;
  bool success;
};

struct MeasureBasic : public ParsedBase {
  unsigned short dist;
  unsigned short amp;
};

#endif // PARSED_H
