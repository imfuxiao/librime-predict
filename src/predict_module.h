#ifndef PREDICT_MODULE_H_
#define PREDICT_MODULE_H_

#include <rime_api.h>

namespace hamster {
extern Bool predict_query(RimeSessionId session_id, const char* query);
extern const char* predict_candidate(RimeSessionId session_id, int index);
extern void reset_commit_text(RimeSessionId session_id);
}  // namespace hamster

#endif  // PREDICT_MODULE_H_