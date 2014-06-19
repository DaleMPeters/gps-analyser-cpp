/* 
 * File:   fix_data.cpp
 * Author: Dale Peters (dmp9@aber.ac.uk)
 * 
 * This file is used to define the functions declared in the Fix_Data.h file,
 * the functions of the Fix_Data class.
 *
 * Created on 23 March 2014
 */

#include"fix_data.h"
#include<stdlib.h>
#include<string>
#include<sstream>

/**
 * Constructs a new instance of a Fix_Data object.
 * 
 * @param data_from_file A vector containing the values about the
 * fix in string form, as split from the data file.
 */
Fix_Data::Fix_Data(std::vector<std::string> data_from_file) {
    // Converts string to integer
    std::stringstream(data_from_file[1]) >> this->num_sentences_for_fix;
    int i;
    // Loop through the vector
    for (i = 7; i < data_from_file.size(); i += 4) {
        if (data_from_file[i].compare("") == 0) { // If the SNR value was an empty string
            snr_data.push_back(0); // Add 0
        } else {
            snr_data.push_back(atoi(data_from_file[i].c_str())); // Convert from C++ string to integer
        }
    }
}

/**
 * Retrieves the number of readings above 35 from the fix data. It does
 * this by looping through the vector of SNR data and incrementing a
 * counter if the SNR reading is above 35.
 * 
 * @return The number of SNR readings from the vector with a value above 35.
 */
int Fix_Data::get_num_good_readings() {
    int i;
    int num_good_readings = 0;
    for (std::vector<int>::iterator it = snr_data.begin(); it != snr_data.end(); ++it) {
        if ((*it) > 35) {
            num_good_readings++;
        }
    }
}

/**
 * Returns the total number of sentences for a group of fix data.
 *
 * @return The total number of sentences for the group of fix data.
 */
int Fix_Data::get_num_sentences_for_fix() {
    return num_sentences_for_fix;
}

