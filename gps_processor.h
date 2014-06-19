/* 
 * File:   gps_processor.h
 * Author: Dale Peters (dmp9@aber.ac.uk)
 *
 * Created on 23 March 2014, 16:00
 */

#ifndef GPS_PROCESSOR_H
#define	GPS_PROCESSOR_H
#include "fix_data.h"
#include <vector>

void write_gpx_file(char *output_file_name, std::vector<Location>);

std::vector<Location*> process_nmea_files(std::string file_name_1, std::string file_name_2);

std::vector<Fix_Data> process_fix_data(std::ifstream file_reader, std::string prev_line);

void write_gpx_file(std::string output_file_name, std::vector<Location*> output_data);

std::vector<std::string> split_and_strip_sentence(std::string sentence);

bool is_good_fix(std::vector<Fix_Data> gsv_data);

Location *calculate_fixed_position(Location *unfixed_location, double lat_offset,
        double lng_offset);

#endif	/* GPS_PROCESSOR_H */

