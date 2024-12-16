#ifndef PREDICT_MODULE_H_
#define PREDICT_MODULE_H_

#include <rime_api.h>

namespace hamster {
extern Bool predict_query(RimeSessionId session_id, const char* query);
extern const char* predict_candidate(RimeSessionId session_id, int index);
}  // namespace hamster

#endif  // PREDICT_MODULE_H_