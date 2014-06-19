/* 
 * File:   gps_processor.cpp
 * Author: Dale Peters (dmp9@aber.ac.uk)
 * 
 * This file is used for the main routine of the program, containing
 * the file processing functions and the main function.
 *
 * Created on 23 March 2014, 12:31
 */

////////////// Files used from the standard library //////////////
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>

////////////// Files used from the Boost library //////////////
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

////////////// Included files of my own //////////////
#include "location.h"
#include "gps_processor.h"
#include "fix_data.h"

////////////// Common sentence types for increased maintainability //////////////
#define RMC "$GPRMC"
#define GSV "$GPGSV"

/**
 * The main function for executing the GPS processor.
 * @param argc The number of arguments provided at the command line.
 * @param argv The list of arguments provided at the command line.
 * @return 0 if the program executed successfully.
 */
int main(int argc, char** argv) {
    const char *file_1_name = "gps_1.dat";
    const char *file_2_name = "gps_2.dat";
    std::string cpp_1_name(file_1_name);
    std::string cpp_2_name(file_2_name);

    const char *output_file_name = "output.gpx";
    std::string cpp_output_name(output_file_name);

    std::vector<Location*> out_data = process_nmea_files(cpp_1_name, cpp_2_name);
    write_gpx_file(cpp_output_name, out_data);
    return 0;

}

/**
 * The main function for processing the two files in NMEA format.
 * @param file_name_1 The name of the first file to extract data from of type C++ string.
 * @param file_name_2 the name of the second file to extract data from for 
 * processing of type C++ string.
 * @return A vector of the most accurate GPS data from the two files.
 */
std::vector<Location*> process_nmea_files(std::string file_name_1, std::string file_name_2) {
    std::vector<Location*> output_locations; // Stores the accurate locations.
    std::ifstream in_file_1; // The stream for file 1
    std::ifstream in_file_2; // The stream for file 2
    in_file_1.open(file_name_1.c_str()); // } Convert the C++ string file name to
    in_file_2.open(file_name_2.c_str()); // } C style string for compatability with open function.
    
    // Boolean variables to represent state of good or bad fix for each stream.
    bool receiver_1_good_fix = true, receiver_2_good_fix = true;
    
    // Stores the offset between the two files
    double lat_offset = 0.0f, lng_offset = 0.0f;
    std::string next_line;
    while (in_file_1 >> next_line) {
        Location *rec_1_location = NULL, *rec_2_location = NULL;
        if (next_line.find(GSV, 0) != std::string::npos) {
            std::vector<Fix_Data> rec1_gsv_data;
            rec1_gsv_data.push_back(split_and_strip_sentence(next_line));
            // Read the next n lines, where n is the number of lines for the
            // fix group
            int i;
            for (i = 0; i < rec1_gsv_data[0].get_num_sentences_for_fix() - 1; i++) {
                in_file_1 >> next_line;
                rec1_gsv_data.push_back(split_and_strip_sentence(next_line));
            }
            // Update the fix value
            receiver_1_good_fix = is_good_fix(rec1_gsv_data);
        } else if (next_line.find(RMC, 0) != std::string::npos) { // If the line is of type RMC
            rec_1_location = new Location(split_and_strip_sentence(next_line));
            std::string next_line_2;
            do {
                // Read lines until it's of a type RMC or GSV
                in_file_2 >> next_line_2;
            } while (next_line_2.find(RMC, 0) == std::string::npos &&
                    next_line_2.find(GSV, 0) == std::string::npos);
            if (next_line_2.find(GSV, 0) != std::string::npos) {
                std::vector<Fix_Data> rec2_gsv_data;
                rec2_gsv_data.push_back(split_and_strip_sentence(next_line_2));
                int i;
                for (i = 0; i < rec2_gsv_data[0].get_num_sentences_for_fix() - 1; i++) {
                    in_file_2 >> next_line_2;
                    rec2_gsv_data.push_back(split_and_strip_sentence(next_line_2));
                }
                // Update fix
                receiver_2_good_fix = is_good_fix(rec2_gsv_data);
                do {
                    in_file_2 >> next_line_2;
                } while (next_line_2.find(RMC) == std::string::npos);
                rec_2_location = new Location(split_and_strip_sentence(next_line_2));
            } else if (next_line_2.find(RMC) != std::string::npos) {
                rec_2_location = new Location(split_and_strip_sentence(next_line_2));
            }
        }
        if (receiver_1_good_fix == true && receiver_2_good_fix == true
                && rec_1_location != NULL && rec_2_location != NULL) {
            // Update offset if both receivers have good fix
            lat_offset = rec_1_location->get_lat_decimal() - rec_2_location->get_lat_decimal();
            lng_offset = rec_1_location->get_lng_decimal() - rec_2_location->get_lng_decimal();
            // Add receiver 1's location
            output_locations.push_back(rec_1_location);
        } else if (!receiver_1_good_fix && rec_2_location != NULL) {
            Location *fixed_location = NULL;
            fixed_location = calculate_fixed_position(rec_2_location, lat_offset, lng_offset);
            output_locations.push_back(fixed_location);
        }
    }
    return output_locations;
}

/**
 * Produces a GPX file of the most accurate GPS data retrieved from the
 * NMEA files from a vector of the most accurate information.
 *
 * @param output_file_name The name of the file to output to.
 * @param output_data A vector containing the data to output to the file.
 */
void write_gpx_file(std::string output_file_name, std::vector<Location*> output_data) {
    std::ofstream output_file_handle; // Output file stream
    output_file_handle.open(output_file_name.c_str()); // Convert C++ style string to C for compatability with open function
    
    // Print header
    output_file_handle << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n";
    output_file_handle << "<gpx xmlns=\"http://www.topografix.com/GPX/1/1\" xmlns:"
            << "gpxx=\"http://www.garmin.com/xmlschemas/GpxExtensions/v3\" "
            << "xmlns:gpxtpx=\"http://www.garmin.com/xmlschemas/TrackPointExtension/v1\" "
            << "creator=\"Oregon 400t\" version=\"1.1\" "
            << "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
            << "xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1"
            << " http://www.topografix.com/GPX/1/1/gpx.xsd "
            << "http://www.garmin.com/xmlschemas/GpxExtensions/v3 "
            << "http://www.garmin.com/xmlschemas/GpxExtensionsv3.xsd "
            << "http://www.garmin.com/xmlschemas/TrackPointExtension/v1 "
            << "http://www.garmin.com/xmlschemas/TrackPointExtensionv1.xsd\">\n";
    output_file_handle << "\t<metadata>\n";
    output_file_handle << "\t\t<link href=\"http://www.garmin.com\">\n";
    output_file_handle << "\t\t\t<text>Garmin International</text>\n";
    output_file_handle << "\t\t</link>\n";
    output_file_handle << "\t</metadata>\n";
    output_file_handle << "\t<trk>\n";
    output_file_handle << "\t\t<name>CS22510 C++ GPX Output File</name>\n";
    output_file_handle << "\t\t<trkseg>\n";
    
    // Iterate through the vector of accurate locations
    for (std::vector<Location*>::iterator it = output_data.begin(); it != output_data.end(); ++it) {
        double lat_decimal = 0.0f;
        double lng_decimal = 0.0f;
        lat_decimal = (*it)->get_lat_decimal();
        lng_decimal = (*it)->get_lng_decimal();
        // Print each latitude and longitude out to the file
        output_file_handle << "\t\t\t<trkpt lat=\"" << lat_decimal
                << "\" lon=\"" << lng_decimal << "\">\n";
        output_file_handle << "\t\t\t</trkpt>\n";
    }
    output_file_handle << "\t\t</trkseg>\n";
    output_file_handle << "\t</trk>\n";
    output_file_handle << "</gpx>";
    output_file_handle.close();
}

/**
 * Takes a C++ string and strips the checksum (last three characters)
 * from it and separates the string in to a vector of strings.
 * 
 * @param sentence The sentence to strip and split.
 * @return A vector containing the values contained by the NMEA sentence,
 * each value being in the form of a C++ string.
 */
std::vector<std::string> split_and_strip_sentence(std::string sentence) {
    std::string stripped;
    stripped = sentence.substr(0, sentence.size() - 3);
    std::vector<std::string> values;
    boost::split(values, stripped, boost::is_any_of(","), boost::token_compress_on);
    return values;
}

/**
 * Determines from a vector of fix data whether the fix from the NMEA file
 * is of a good fix.
 * 
 * @param gsv_data The GSV data that determines the quality of the
 * fix.
 * @return true if the fix quality is good.
 */
bool is_good_fix(std::vector<Fix_Data> gsv_data) {
    int num_good_readings = 0, i = 0;
    // Loop through the vector and count the number of SNR values above 35
    for (std::vector<Fix_Data>::iterator it = gsv_data.begin(); it != gsv_data.end(); ++it) {
        num_good_readings += (*it).get_num_good_readings();
    }
    return num_good_readings >= 3;
}

/**
 * Calculates a reliable position from an offset by adding on the given
 * offset values to the latitude and longitude.
 * 
 * @param unfixed_location The location before the offset is added to it.
 * @param lat_offset The offset to add on to the latitude.
 * @param lng_offset The offset to add on to the longitude.
 * @return The reliable position with the offset added on to the latitude
 * and longitude.
 */
Location *calculate_fixed_position(Location *unfixed_location, double lat_offset,
        double lng_offset) {
    Location *fixed_location;
    double unfixed_lat_dec = unfixed_location->get_lat_decimal();
    double unfixed_lng_dec = unfixed_location->get_lng_decimal();
    fixed_location->set_lat_decimal(unfixed_lat_dec + lat_offset);
    fixed_location->set_lng_decimal(unfixed_lng_dec + lng_offset);
    return fixed_location;
}