#include "analyzer_any_parameters_seen.h"

#include "util.h"

void Analyzer_Any_Parameters_Seen::evaluate()
{
    if (_vehicle->param_count() > 0) {
        any_parameters_seen = true;
    }
}

void Analyzer_Any_Parameters_Seen::end_of_log(const uint32_t packet_count UNUSED)
{
    Analyzer_Any_Parameters_Seen_Result *result = new Analyzer_Any_Parameters_Seen_Result();
    if (any_parameters_seen) {
        result->set_status(analyzer_status_ok);
        result->set_reason("Parameters seen");
        result->add_source(_data_sources.get("PARAM"));
    } else {
        result->set_status(analyzer_status_fail);
        result->set_reason("No parameters seen");
        result->increase_severity_score(20);
        result->add_source(_data_sources.get("PARAM"));
    }
    add_result(result);
}
