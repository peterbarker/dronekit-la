#ifndef ANALYZER_GPS_FIX_H
#define ANALYZER_GPS_FIX_H

/*
 * analyzer_gps_fix
 *
 */

#include "analyzer.h"
#include "data_sources.h"


class Analyzer_GPS_Fix_Result : public Analyzer_Result_Period {
public:
    Analyzer_GPS_Fix_Result(std::string name) :
        _name(name)
              { };
    void set_satellites(uint8_t satellites) { _satellites = satellites; }
    uint8_t satellites() const { return _satellites; }
    void set_hdop(double hdop) { _hdop = hdop; }
    double hdop() const { return _hdop; }
    void set_sacc(double sacc) { _sacc = sacc; }
    double sacc() const { return _sacc; }
    const std::string name() { return _name; }
private:
    const std::string _name;
    uint8_t _satellites;
    double _hdop;
    double _sacc;
};

class Analyzer_GPS_FirstFixTime_Result : public Analyzer_Result_Event {
public:
    Analyzer_GPS_FirstFixTime_Result(std::string name) :
        _name(name)
        { };
    void set_time_taken(uint64_t time_taken) { _time_taken = time_taken; }

    const std::string name() { return _name; }
private:
    const std::string _name;
    uint64_t _time_taken;
};

class Analyzer_GPS_No_FirstFixTime_Result : public Analyzer_Result_Event {
public:
    Analyzer_GPS_No_FirstFixTime_Result()
        { }

private:

};


class Analyzer_GPS_Fix : public Analyzer {

public:
    Analyzer_GPS_Fix(AnalyzerVehicle::Base *&vehicle, Data_Sources &data_sources) :
    Analyzer(vehicle, data_sources)
    { }


    const std::string name() const override { return "GPS Fix"; }
    const std::string description() const override {
        return "The accuracy and precision of GPS messages can vary depending on many factors including weather, ionospheric disturbances and number of satellites visible.  This test will FAIL if the quality of the GPS information is poor.";
    }

    bool configure(INIReader *config) override;

    uint64_t first_timestamp_utc_ms() { return _first_timestamp_utc_ms;  }

    void set_first_latitude(float latitude) { _first_latitude = latitude; }
    float first_latitude() const { return _first_latitude; }
    void set_first_longitude(float longitude) { _first_longitude = longitude; }
    float first_longitude() const { return _first_longitude; }

private:

    void evaluate() override;
    void evaluate_gps(AnalyzerVehicle::GPSInfo *gpsinfo);

    std::map<const std::string, Analyzer_GPS_Fix_Result*> _result;
    void close_result(const std::string result_key,
                      Analyzer_GPS_Fix_Result *result);
    void open_result(AnalyzerVehicle::GPSInfo *gpsinfo);
    void update_result(AnalyzerVehicle::GPSInfo *gpsinfo);
    void close_results();
    void end_of_log(const uint32_t packet_count) override;

    uint8_t satellites_visible_threshold() const { return _satellites_min; }
    double hdop_threshold() const { return _hdop_min; }
    double sacc_threshold_warn() const { return _sacc_threshold_warn; }
    double sacc_threshold_fail() const { return _sacc_threshold_fail; }

    bool gpsinfo_bad(AnalyzerVehicle::GPSInfo *gpsinfo) const;

    void add_firstfixtime_result(AnalyzerVehicle::GPSInfo *gpsinfo,
                                 uint64_t time_taken);
    void add_no_firstfixtime_result();


    void set_first_timestamp_utc_ms(uint64_t timestamp_ms) { _first_timestamp_utc_ms = timestamp_ms;  }
    uint64_t _first_timestamp_utc_ms = 0;

    uint8_t _satellites_min = 5;
    double _hdop_min = 5.0f;

    class FirstFixInfo {
    public:
        bool first_3D_fix_found = false;
        uint64_t first_3D_fix_T = 0;
        bool non_3dfix_seen = false;
    };
    std::map<const std::string, FirstFixInfo*> _result_ff;
    void close_result_ff(const std::string result_key, FirstFixInfo *info);
    void close_results_ff();
    void add_no_firstfixtime_result(const std::string result_key,
                                    FirstFixInfo *info);

    float _sacc_threshold_warn = 1.0f;
    float _sacc_threshold_fail = 1.5f;

    float _first_latitude = 0.0;
    float _first_longitude = 0.0;
};

#endif


