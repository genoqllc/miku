#ifndef MIKU_FLAGS_HPP
#define MIKU_FLAGS_HPP

/// @brief Flags system so screens can selectively delcare what data they depend on. The App can then apply the correct data to the screen.
enum DependencyFlags
{
    /// @brief The screen needs no special data
    None = 0,
    /// @brief The screen needs real-time MIDI events
    MidiEvents = 1 << 0,
};

#endif
