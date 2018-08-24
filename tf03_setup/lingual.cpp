#include "lingual.h"

Language gCurrentLanguage = Language::chinese;
std::shared_mutex gCurrentLanguageMutex;

Language current_language() {
  std::shared_lock<std::shared_mutex> lock(gCurrentLanguageMutex);
  return gCurrentLanguage;
}

void set_current_language(const Language &language) {
  std::unique_lock<std::shared_mutex> lock(gCurrentLanguageMutex);
  gCurrentLanguage = language;
}

QString which_lingual(const Lingual &lingual) {
  auto language = current_language();
  switch (language) {
  case Language::chinese : return lingual.chn;
  case Language::english : return lingual.eng;
  }
  return "";
}
