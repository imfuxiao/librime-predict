#include <rime/component.h>
#include <rime/registry.h>
#include <rime_api.h>
#include <rime/service.h>
#include <rime/registry.h>
#include <rime/context.h>

#include "predictor.h"
#include "predict_engine.h"
#include "predict_translator.h"

using namespace rime;

static an<PredictEngineComponent> engine_factory;

static void rime_predict_initialize() {
  Registry& r = Registry::instance();
  if (!engine_factory) {
    engine_factory = New<PredictEngineComponent>();
  }
  r.Register("predictor", new PredictorComponent(engine_factory));
  r.Register("predict_translator",
             new PredictTranslatorComponent(engine_factory));
}

static void rime_predict_finalize() {
  if (engine_factory) {
    engine_factory.reset();
  }
}

RIME_REGISTER_MODULE(predict)

namespace hamster {

Bool predict_query(RimeSessionId session_id, const char* query) {
  if (!engine_factory)
    return false;

  an<Session> session(Service::instance().GetSession(session_id));
  if (!session)
    return False;

  Context* ctx = session->context();
  Schema* schema = session->schema();
  if (!ctx)
    return False;
  if (!schema)
    return False;

  Ticket ticket{schema, "predictor"};
  an<PredictEngine> engine = engine_factory->GetInstance(ticket);
  if (!engine)
    return False;

  return engine->Predict(ctx, query);
}

const char* predict_candidate(RimeSessionId session_id, int index) {
  an<Session> session(Service::instance().GetSession(session_id));
  if (!session)
    return nullptr;

  Context* ctx = session->context();
  Schema* schema = session->schema();
  if (!ctx)
    return nullptr;
  if (!schema)
    return nullptr;

  Ticket ticket{schema, "predictor"};
  an<PredictEngine> engine = engine_factory->GetInstance(ticket);
  if (!engine)
    return nullptr;

  int num_candidates = engine->num_candidates();
  int max_candidates = engine->max_candidates();
  int total = num_candidates > max_candidates ? max_candidates : num_candidates;
  if (index >= total) {
    return nullptr;
  }
  static std::string str;
  str = engine->candidate(index);
  return str.c_str();
}

void reset_commit_text(RimeSessionId session_id) {
  an<Session> session(Service::instance().GetSession(session_id));
  if (!session)
    return;
  session->ResetCommitText();
}

}  // namespace hamster
