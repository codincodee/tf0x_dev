#ifndef PARSED_H
#define PARSED_H

#include <QString>

struct ParsedBase {
  virtual ~ParsedBase();
};

struct Status : public ParsedBase {
  QString command;
  bool success;
};

struct MeasureBasic : public ParsedBase {
  unsigned short dist;
  unsigned short amp;
};

#endif // PARSED_H
