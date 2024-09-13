#ifndef CLAMPER_H
#define CLAMPER_H

#include "daisysp.h"

/// @brief A utility class for clamping a potentiometer reading to a set of items in a list.
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

    /// @brief Given a pot reading from 0 to 65535, return the index of a set of items in a list that reading would have selected.
    /// @param potentiometerReading The reading from the pot.
    /// @param itemCount The total number of items in the list you want to select from.
    /// @return The index that was selected by the pot reading.
    static uint16_t ReadingToIndex(uint16_t potentiometerReading, int itemCount)
    {
        float rawSelection = (potentiometerReading / 65535.0f * itemCount);
        return clamp((int)rawSelection, 0, itemCount - 1);
    }
private:
    static uint16_t clamp(uint16_t value, uint16_t min, uint16_t max)
    {
        return (value < min) ? min : (value > max) ? max : value;
    }
};

#endif