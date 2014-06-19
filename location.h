/* 
 * File:   location.h
 * Author: Dale Peters (dmp9@aber.ac.uk)
 *
 * This file contains the definition and function declarations of
 * the Location class.
 * 
 * Created on 23 March 2014, 15:44
 */

#ifndef LOCATION_H
#define	LOCATION_H
#include <string>
#include <vector>

class Location {
private:
    /**
     * The latitude of the location in degrees.
     */
    int lat_degrees;
    /** 
     * The latitude of the location in minutes.
     */
    double lat_mins;
    
    /**
     * The longitude of the location in degrees.
     */
    int lng_degrees;
    
    /**
     * The longitude of the location in minutes.
     */
    double lng_mins;
    
    /**
     * The latitude of the location in decimal form.
     */
    double lat_decimal;
    
    /**
     * The longitude of the location in decimal form.
     */
    double lng_decimal;

public:
    /**
     * The constructor for the location.
     */
    Location(std::vector<std::string> data_from_file);

    /**
     * Getter for the location's latitude.
     */
    double get_lat_decimal();

    /**
     * Getter for the location's longitude.
     */
    double get_lng_decimal();
    
    /**
     * Setter for the location's latitude.
     */
    void set_lat_decimal(double lat_dec);
    
    /**
     * Setter for the location's longitude.
     */
    void set_lng_decimal(double lng_dec);
};

#endif	/* LOCATION_H */

