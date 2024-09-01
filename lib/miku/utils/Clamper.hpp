#ifndef CLAMPER_H
#define CLAMPER_H

#include "daisysp.h"

class Clamper
{
public:
    /// @brief Given a potentiometer reading (from 0 to 1), return the index of a set of items in a list that reading would have selected.
    /// @param potentiometerReading The reading from the pot.
    /// @param itemCount The total number of items in the list you want to select from.
    /// @return The index that was selected by the pot reading.
    static unsigned short ReadingToIndex(float potentiometerReading, int itemCount)
    {
        return (unsigned short)daisysp::fclamp((potentiometerReading * 100.0) / (100 / itemCount), 0, itemCount - 1);
    }
};

#endif