/* 
 * File:   fix_data.h
 * Author: Dale Peters (dmp9@aber.ac.uk)
 * 
 * This file contains the definition of the Fix_Data class.
 *
 * Created on 23 March 2014, 12:32
 */

#ifndef FIX_DATA_H
#define	FIX_DATA_H

#include <vector>
#include <string>

/**
 * Used to represent fix data obtained from GSV sentences.
 */
class Fix_Data {
private:
    /**
     * The total number of sentences for a particular group of GSV
     * sentences.
     */
    int num_sentences_for_fix;
    /**
     * The values of the SNR readings obtained from the GSV sentences.
     */
    std::vector<int> snr_data;

public:
    /**
     * Constructor for the Fix_Data class.
     */
    Fix_Data(std::vector<std::string>);
    
    /**
     * Getter method for the number of sentences for a fix group. 
     */
    int get_num_sentences_for_fix();
    
    /**
     * Getter method for the number of SNR values of a good standard.
     */
    int get_num_good_readings();

};

#endif	/* FIX_DATA_H */

