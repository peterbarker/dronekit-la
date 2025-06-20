#include "analyze.h"

#include <stdlib.h> // for exit() (fixme)
#include <algorithm> // for for_each

#include "analyzer/analyzer_any_parameters_seen.h"
#include "analyzer/analyzer_arming_checks.h"
#include "analyzer/analyzer_altitude_estimate_divergence.h"
#include "analyzer/analyzer_attitude_estimate_divergence.h"
#include "analyzer/analyzer_attitude_control.h"
#include "analyzer/analyzer_autopilot.h"
#include "analyzer/analyzer_battery.h"
#include "analyzer/analyzer_truncated_log.h"
#include "analyzer/analyzer_compass_offsets.h"
#include "analyzer/analyzer_compass_vector_length.h"
#include "analyzer/analyzer_ever_armed.h"
#include "analyzer/analyzer_good_ekf.h"
#include "analyzer/analyzer_good_nkf.h"
#include "analyzer/analyzer_good_xkf.h"
#include "analyzer/analyzer_gps_fix.h"
#include "analyzer/analyzer_gyro_drift.h"
#include "analyzer/analyzer_notcrashed.h"
#include "analyzer/analyzer_parameters.h"
#include "analyzer/analyzer_position_estimate_divergence.h"
#include "analyzer/analyzer_sensor_health.h"
#include "analyzer/analyzer_subsys_errors.h"
#include "analyzer/analyzer_motorbalance.h"
#include "analyzer/analyzer_vehicle_definition.h"
#include "analyzer/analyzer_velocity_estimate_divergence.h"

#include "analyzer/analyzer_issue_sacc.h"

#include "la-log.h"

void Analyze::set_analyzer_names_to_run(const std::vector<std::string> run_these)
{
    _use_names_to_run = true;
    for (std::vector<std::string>::const_iterator it = run_these.begin();
         it != run_these.end();
         ++it) {
        _names_to_run[(*it)] = true;
    }
}

void Analyze::instantiate_analyzers(INIReader *config)
{
    Analyzer_Any_Parameters_Seen *analyzer_any_parameters_seen = new Analyzer_Any_Parameters_Seen(vehicle,_data_sources);
    if (analyzer_any_parameters_seen != NULL) {
        configure_analyzer(config, analyzer_any_parameters_seen);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_any_parameters_seen");
    }

    Analyzer_Arming_Checks *analyzer_arming_checks = new Analyzer_Arming_Checks(vehicle,_data_sources);
    if (analyzer_arming_checks != NULL) {
        configure_analyzer(config, analyzer_arming_checks);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_arming_checks");
    }

    analyzer_altitude_estimate_divergence = new Analyzer_Altitude_Estimate_Divergence(vehicle,_data_sources);
    if (analyzer_altitude_estimate_divergence != NULL) {
        configure_analyzer(config, analyzer_altitude_estimate_divergence);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_altitude_estimate_divergence");
    }

    Analyzer_Attitude_Estimate_Divergence *analyzer_attitude_estimate_divergence = new Analyzer_Attitude_Estimate_Divergence(vehicle,_data_sources);
    if (analyzer_attitude_estimate_divergence != NULL) {
        configure_analyzer(config, analyzer_attitude_estimate_divergence);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_attitude_estimate_divergence");
    }

    {
        Analyzer_Compass_Offsets *analyzer_compass_offsets = new Analyzer_Compass_Offsets(vehicle,_data_sources, "");
        if (analyzer_compass_offsets != NULL) {
            configure_analyzer(config, analyzer_compass_offsets);
        } else {
            la_log(LOG_INFO, "Failed to create analyzer_compass_offsets");
        }
    }
    {
        Analyzer_Compass_Vector_Length *analyzer_compass_vector_length = new Analyzer_Compass_Vector_Length(vehicle,_data_sources);
        if (analyzer_compass_vector_length != NULL) {
            configure_analyzer(config, analyzer_compass_vector_length);
        } else {
            la_log(LOG_INFO, "Failed to create analyzer_compass_vector_length1");
        }
    }
    {
        Analyzer_Compass_Offsets *analyzer_compass_offsets = new Analyzer_Compass_Offsets(vehicle,_data_sources, "2");
        if (analyzer_compass_offsets != NULL) {
            configure_analyzer(config, analyzer_compass_offsets);
        } else {
            la_log(LOG_INFO, "Failed to create analyzer_compass_offsets");
        }
    }
    {
        Analyzer_Compass_Offsets *analyzer_compass_offsets = new Analyzer_Compass_Offsets(vehicle,_data_sources, "3");
        if (analyzer_compass_offsets != NULL) {
            configure_analyzer(config, analyzer_compass_offsets);
        } else {
            la_log(LOG_INFO, "Failed to create analyzer_compass_offsets");
        }
    }

    Analyzer_Ever_Armed *analyzer_ever_armed = new Analyzer_Ever_Armed(vehicle,_data_sources);
    if (analyzer_ever_armed != NULL) {
        configure_analyzer(config, analyzer_ever_armed);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_ever_armed");
    }

    analyzer_ever_flew = new Analyzer_Ever_Flew(vehicle,_data_sources);
    if (analyzer_ever_flew != NULL) {
        configure_analyzer(config, analyzer_ever_flew);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_ever_flew");
    }


    Analyzer_Good_EKF *analyzer_good_ekf = new Analyzer_Good_EKF(vehicle,_data_sources);
    if (analyzer_good_ekf != NULL) {
        configure_analyzer(config, analyzer_good_ekf);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_good_ekf");
    }

    Analyzer_Good_NKF *analyzer_good_nkf = new Analyzer_Good_NKF(vehicle,_data_sources);
    if (analyzer_good_nkf != NULL) {
        configure_analyzer(config, analyzer_good_nkf);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_good_nkf");
    }

    Analyzer_Good_XKF *analyzer_good_xkf = new Analyzer_Good_XKF(vehicle,_data_sources);
    if (analyzer_good_xkf != NULL) {
        configure_analyzer(config, analyzer_good_xkf);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_good_xkf");
    }

    analyzer_gps_fix = new Analyzer_GPS_Fix(vehicle,_data_sources);
    if (analyzer_gps_fix != NULL) {
        configure_analyzer(config, analyzer_gps_fix);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_gps_fix");
    }

    {
        Analyzer_Gyro_Drift *analyzer_gyro_drift = new Analyzer_Gyro_Drift(vehicle,_data_sources);
        if (analyzer_gyro_drift != NULL) {
            configure_analyzer(config, analyzer_gyro_drift);
        } else {
            la_log(LOG_INFO, "Failed to create analyzer_gyro_drift");
        }
    }

    Analyzer_Attitude_Control *analyzer_attitude_control = new Analyzer_Attitude_Control(vehicle,_data_sources);
    if (analyzer_attitude_control != NULL) {
        configure_analyzer(config, analyzer_attitude_control);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_attitude_control");
    }

    Analyzer_Autopilot *analyzer_autopilot = new Analyzer_Autopilot(vehicle,_data_sources);
    if (analyzer_autopilot != NULL) {
        configure_analyzer(config, analyzer_autopilot);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_autopilot");
    }

    Analyzer_Battery *analyzer_battery = new Analyzer_Battery(vehicle,_data_sources);
    if (analyzer_battery != NULL) {
        configure_analyzer(config, analyzer_battery);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_battery");
    }

    Analyzer_MotorBalance *analyzer_motorbalance = new Analyzer_MotorBalance(vehicle,_data_sources);
    if (analyzer_motorbalance != NULL) {
        configure_analyzer(config, analyzer_motorbalance);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_motorbalance");
    }

    Analyzer_Truncated_Log *analyzer_truncated_log = new Analyzer_Truncated_Log(vehicle,_data_sources);
    if (analyzer_truncated_log != NULL) {
        configure_analyzer(config, analyzer_truncated_log);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_truncated_log");
    }

    analyzer_position_estimate_divergence = new Analyzer_Position_Estimate_Divergence(vehicle,_data_sources);
    if (analyzer_position_estimate_divergence != NULL) {
        configure_analyzer(config, analyzer_position_estimate_divergence);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_position_estimate_divergence");
    }

    Analyzer_NotCrashed *analyzer_notcrashed = new Analyzer_NotCrashed(vehicle,_data_sources);
    if (analyzer_notcrashed != NULL) {
        configure_analyzer(config, analyzer_notcrashed);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_not_crashed");
    }

    Analyzer_Parameters *analyzer_parameters = new Analyzer_Parameters(vehicle,_data_sources);
    if (analyzer_parameters != NULL) {
        configure_analyzer(config, analyzer_parameters);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_parameters");
    }

    Analyzer_Sensor_Health *analyzer_sensor_health = new Analyzer_Sensor_Health(vehicle,_data_sources);
    if (analyzer_sensor_health != NULL) {
        configure_analyzer(config, analyzer_sensor_health);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_sensor_health");
    }

    Analyzer_Subsys_Errors *analyzer_subsys_errors = new Analyzer_Subsys_Errors(vehicle,_data_sources);
    if (analyzer_subsys_errors != NULL) {
        configure_analyzer(config, analyzer_subsys_errors);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_subsys_errors");
    }

    Analyzer_Vehicle_Definition *analyzer_vehicle_definition = new Analyzer_Vehicle_Definition(vehicle,_data_sources);
    if (analyzer_vehicle_definition != NULL) {
        configure_analyzer(config, analyzer_vehicle_definition);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_vehicle_definition");
    }

    analyzer_velocity_estimate_divergence = new Analyzer_Velocity_Estimate_Divergence(vehicle,_data_sources);
    if (analyzer_velocity_estimate_divergence != NULL) {
        configure_analyzer(config, analyzer_velocity_estimate_divergence);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_velocity_estimate_divergence");
    }

    // issue-specific analyzers are not run by default:
    Analyzer_Issue_Sacc * analyzer_issue_sacc = new Analyzer_Issue_Sacc(vehicle, _data_sources);
    if (analyzer_issue_sacc != NULL) {
        configure_analyzer(config, analyzer_issue_sacc, false);
    } else {
        la_log(LOG_INFO, "Failed to create analyzer_issue_sacc");
    }
}


void Analyze::configure_analyzer(INIReader *config, Analyzer *analyzer, bool run_default)
{
    if ((!run_default || _use_names_to_run) &&
        !_names_to_run[analyzer->name()]) {
        return;
    }
    if (analyzer->configure(config)) {
        _analyzers.push_back(analyzer);
    } else {
        la_log(LOG_INFO, "Failed to configure (%s)", analyzer->name().c_str());
    }
    analyzer->set_pure_output(pure_output());
}

void results_json_add_version(Json::Value &root)
{
    const char *git_version = GIT_VERSION;
    if (git_version != NULL) {
        root["git_version"] = git_version;
    }
}

void Analyze::results_json_add_statistics(Json::Value &root)
{
    if (analyzer_ever_flew != NULL) {
        root["total-flight-time"] = analyzer_ever_flew->total_flight_time() / 1000000.0f;
        root["total-flight-time-units"] = "seconds";
    }
    if (analyzer_position_estimate_divergence != NULL) {
        root["total-distance-travelled"] = analyzer_position_estimate_divergence->total_distance_travelled();
        root["total-distance-travelled-units"] = "metres";
        double dfh = analyzer_position_estimate_divergence->maximum_distance_from_origin();;
        if (!is_equal(dfh, -1.0f)) {
            root["maximum-distance-from-origin"] = dfh;
            root["maximum-distance-from-origin-units"] = "metres";
        }
    }

    if (analyzer_altitude_estimate_divergence != NULL) {
        root["maximum-altitude-absolute"] = analyzer_altitude_estimate_divergence->maximum_altitude();
        root["maximum-altitude-absolute-units"] = "metres";
        root["maximum-altitude-relative"] = analyzer_altitude_estimate_divergence->maximum_altitude_relative();
        root["maximum-altitude-relative-units"] = "metres";
    }

    if (analyzer_velocity_estimate_divergence != NULL) {
        root["maximum-velocity"] = analyzer_velocity_estimate_divergence->maximum_velocity();
        root["maximum-velocity-units"] = "metres/second";
    }

    if (analyzer_gps_fix->first_timestamp_utc_ms() != 0) {
        root["first-timestamp-utc"] = analyzer_gps_fix->first_timestamp_utc_ms() * 0.001;
        root["first-timestamp-utc-units"] = "seconds";
    }

    {
        const float first_latitude = analyzer_gps_fix->first_latitude();
        root["first-gps-latitude"] = first_latitude;
        root["first-gps-latitude-units"] = "degrees";
        const float first_longitude = analyzer_gps_fix->first_longitude();
        root["first-gps-longitude"] = first_longitude;
        root["first-gps-longitude-units"] = "degrees";
    }
}

void Analyze::results_json(Json::Value &root)
{
    Json::Value tests;
    uint16_t total_evilness= 0;
    for (std::vector<Analyzer*>::iterator it = _analyzers.begin();
         it != _analyzers.end();
         ++it) {
        Analyzer *analyzer = *it;
        const std::string name = analyzer->name();
        if (tests[name].isNull()) {
            Json::Value test_info(Json::objectValue);
            test_info["description"] = analyzer->description();
            test_info["name"] = name;
            test_info["results"] = Json::Value(Json::arrayValue);
            tests[name] = test_info;
        }

        analyzer->results_json_results(tests[name]["results"]);
        tests[name]["status"] = analyzer->status_as_string();
        tests[name]["severity-score"] = tests[name]["severity-score"].asLargestUInt() + analyzer->severity_score();
        if (!pure_output()) {
            tests[name]["evilness"] = tests[name]["severity-score"];
            tests[name]["evilness-is-deprecated"] = "Use severity-score";
        }
        total_evilness += analyzer->severity_score();
    }
    
    root["severity-score"] = total_evilness;
    if (!pure_output()) {
        root["evilness"] = root["severity-score"];
        root["evilness-is-deprecated"] = "Use severity-score";
    }
    root["tests"] = tests;
    results_json_add_version(root);

    results_json_add_statistics(root);
}


namespace Json {
    class PlainTextWriter : public Json::Writer {
    public:
        PlainTextWriter() :
            depth(0)
            { }
        std::string write( const Value &root );
        void writeValue(std::string &document, const Value &value);
    private:
        uint16_t depth;
        void writeIndent(std::string &document);
    };

    std::string PlainTextWriter::write( const Value &root )
    {
        std::string document = "";
        depth = 0;
        writeValue(document, root);
        return document;
    }
    void PlainTextWriter::writeIndent(std::string &document)
    {
        // FIXME, surely!
        for (uint8_t i=0; i< depth; i++) {
            document += "    ";
        }
    }

    void PlainTextWriter::writeValue(std::string &document, const Value &value)
    {
        switch(value.type()) {
        case nullValue:
            break;
        case intValue:
            document += valueToString(value.asLargestInt());
            break;
        case uintValue:
            document += valueToString(value.asLargestUInt());
            break;
        case realValue:
            document += valueToString(value.asDouble());
            break;
        case stringValue:
            document += valueToQuotedString(value.asString().c_str());
            break;
        case booleanValue:
            document += value.asBool();
            break;
        case arrayValue: {
            for (uint8_t index = 0; index < value.size(); index++) {
                switch(value[index].type()) {
                case nullValue:
                case intValue:
                case uintValue:
                case realValue:
                case stringValue:
                case booleanValue:
                    writeIndent(document);
                    writeValue(document, value[index]);
                    document += "\n";
                    break;
                case arrayValue:
                case objectValue:
                    depth++;
                    writeValue(document, value[index]);
                    depth--;
                    break;
                }
            }
            break;
        }
        case objectValue: {
            Value::Members members(value.getMemberNames());
            for (Value::Members::iterator it = members.begin();
                 it != members.end(); ++it) {
                const std::string &name = *it;
                writeIndent(document);
                document += name;
                document += ":";
                switch(value[name].type()) {
                case nullValue:
                case intValue:
                case uintValue:
                case realValue:
                case stringValue:
                case booleanValue:
                    document += " ";
                    writeValue(document, value[name]);
                    document += "\n";
                    break;
                case arrayValue:
                case objectValue:
                    document += "\n";
                    depth++;
                    writeValue(document, value[name]);
                    depth--;
                }
            }
            break;
        }
        }
    }


    class BriefPlainTextWriter : public Json::Writer {
    public:
        BriefPlainTextWriter()
            { }
        std::string write( const Value &root );
    private:
    };

    std::string BriefPlainTextWriter::write( const Value &root )
    {
        std::string document = "";
        document += "Score=";
        document += valueToString(root["evilness"].asLargestUInt());
        if (root["tests"]["Crash Test"]["evilness"].asLargestUInt() != 0) {
            document += " Crash!";
        }

        if (root["tests"]["Ever Flew"]["results"][0]["status"] == std::string("PASS")) {
            document += " Flew";
        }
 
         if (root["tests"]["Vehicle Definition"]["results"][0]["status"] != std::string("PASS")) {
            document += " NoVehicle";
        }
      
// document += "\n";
        return document;
    }

}


namespace Json {
    class HTMLWriter : public Json::Writer {
    public:
        HTMLWriter() :
            depth(0)
            { }
        std::string write( const Value &root );
        void writeValue(std::string &document, const Value &value);
    private:
        uint16_t depth;
        void writeIndent(std::string &document);
    };

    std::string HTMLWriter::write( const Value &root )
    {
        std::string document = "";
        depth = 0;
        writeValue(document, root);
        return document;
    }
    void HTMLWriter::writeIndent(std::string &document)
    {
        // FIXME, surely!
        for (uint8_t i=0; i< depth; i++) {
            document += "    ";
        }
    }

    void HTMLWriter::writeValue(std::string &document, const Value &value)
    {
        switch(value.type()) {
        case nullValue:
            break;
        case intValue:
            document += valueToString(value.asLargestInt());
            break;
        case uintValue:
            document += valueToString(value.asLargestUInt());
            break;
        case realValue:
            document += valueToString(value.asDouble());
            break;
        case stringValue:
            document += valueToQuotedString(value.asString().c_str());
            break;
        case booleanValue:
            document += value.asBool();
            break;
        case arrayValue: {
            document += "<ol>";
            for (uint8_t index = 0; index < value.size(); index++) {
                switch(value[index].type()) {
                case nullValue:
                case intValue:
                case uintValue:
                case realValue:
                case stringValue:
                case booleanValue:
                    writeIndent(document);
                    document += "<li>";
                    writeValue(document, value[index]);
                    document += "</li>";
                    document += "\n";
                    break;
                case arrayValue:
                case objectValue:
                    depth++;
                    document += "<li>";
                    writeValue(document, value[index]);
                    depth--;
                    document += "</li>";
                    break;
                }
            }
            break;
        }
        case objectValue: {
            Value::Members members(value.getMemberNames());
            if (depth == 0) {
                document += "<dl class='la_results'>";
            } else {
                document += "<dl>";
            }
            for (Value::Members::iterator it = members.begin();
                 it != members.end(); ++it) {
                const std::string &name = *it;
                writeIndent(document);
                document += "<dt>";
                document += name;
                document += "</dt>\n";
                writeIndent(document);
                document += "<dd>";
                switch(value[name].type()) {
                case nullValue:
                case intValue:
                case uintValue:
                case realValue:
                case stringValue:
                case booleanValue:
                    writeValue(document, value[name]);
                    break;
                case arrayValue:
                case objectValue:
                    depth++;
                    writeValue(document, value[name]);
                    depth--;
                }
                document += "</dd>\n";
            }
            document += "</dl>\n";
            break;
        }
        }
    }
}

void Analyze::end_of_log(uint32_t packet_count, uint64_t bytes_dropped) {

    std::for_each(_analyzers.begin(),
                  _analyzers.end(),
                  [packet_count](Analyzer* c){c->end_of_log(packet_count); });

    Json::Value root;
    root["format-version"] = "0.1";
    root["timestamp"] = (Json::UInt64)start_time;
    root["duration"] = (Json::UInt64)(now() - start_time);

    results_json(root);

    root["packet_count"] = packet_count;
    root["packet-count"] = packet_count;
    root["bytes-dropped"] = (Json::UInt64)bytes_dropped;

    Json::Writer *writer;
    switch(_output_style) {
    case OUTPUT_JSON: {
        writer = new Json::StyledWriter();
        break;
    }
    case OUTPUT_PLAINTEXT:
        writer = new Json::PlainTextWriter();
        break;
    case OUTPUT_HTML:
        writer = new Json::HTMLWriter();
        break;
    case OUTPUT_BRIEF:
        writer = new Json::BriefPlainTextWriter();
        break;
    default:
        ::fprintf(stderr, "Writer not set for output style");
        abort();
    }
    std::string document = writer->write(root);
    fprintf(stdout, "%s", document.c_str());
}


void Analyze::evaluate_all() {
    std::for_each(_analyzers.begin(),
                  _analyzers.end(),
                  [](Analyzer* c){c->evaluate(); });
}



