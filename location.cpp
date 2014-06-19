/* 
 * File:   location.cpp
 * Author: Dale Peters (dmp9@aber.ac.uk)
 * 
 * This file is used to define the methods in the Location class,
 * as declared in location.h.
 *
 * Created on 23 March 2014
 */

#include "location.h"
#include <sstream>

/**
 * Constructs a new instance of a location given a vector of string
 * values from the file.
 * 
 * @param data_from_file A vector containing the values about the
 * location in string form, as split from the data file.
 */
Location::Location(std::vector<std::string> data_from_file){
    // Extracts the data from the values passed in and sets the instance variables.
    std::stringstream(data_from_file[3].substr(0, 2)) >> this->lat_degrees;
    std::stringstream(data_from_file[5].substr(0, 3)) >> this->lng_degrees;
    std::stringstream(data_from_file[3].substr(2, 7)) >> this->lat_mins;
    std::stringstream(data_from_file[5].substr(3 ,7)) >> this->lng_mins;
    
    // Converts the latitude and longitude to the output form.
    this->lat_decimal = lat_degrees + lat_mins / 60;
    this->lng_decimal = -1 * (lng_degrees + lng_mins / 60);
}

/**
 * Returns the latitude of a location in decimal form.
 * 
 * @return The latitude of the location in decimal form.
 */
double Location::get_lat_decimal(){
    return lat_decimal;
}

/**
 * Returns the longitude of the location in decimal form.
 * 
 * @return The longitude of the location in decimal form.
 */
double Location::get_lng_decimal(){
    return lng_decimal;
}

/**
 * Sets the decimal latitude of the location.
 * 
 * @param lat_dec The latitude in decimal form to set.
 */
void Location::set_lat_decimal(double lat_dec){
    lat_decimal = lat_dec;
}

/**
 * Sets the decimal longitude of the location.
 * 
 * @param lng_dec The longitude in decimal form to set.
 */
void Location::set_lng_decimal(double lng_dec){
    lng_decimal = lng_dec;
}
