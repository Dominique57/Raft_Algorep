#pragma once

#include "utils/string.hh"

namespace Clock {

    /**
     * Enum for the type of speed.
     */
    enum SPEED_TYPE : unsigned int {
        HIGH = 0,
        MEDIUM = 1,
        LOW = 2,
    };

    /**
     * To get the corresponding SPEED name in string form.
     */
    static const char *speed_type_names[] = {
        "high",
        "medium",
        "low",
    };

    /**
     * @brief Get the SPEED_TYPE in string format.
     * @param type              The type of the SPEED in SPEED_TYPE enum
     * @return A char buffer containing the SPEED type.
     */
    inline const char *getSpeedTypeName(const SPEED_TYPE& type) {
        auto typeIndex = static_cast<unsigned int>(type);

        if (typeIndex >= sizeof(speed_type_names) / sizeof(speed_type_names[0]))
            return "Unknown Name";

        return speed_type_names[typeIndex];
    }

    /**
     * @brief Get the SPEED_TYPE in enum format
     * @param type              The name of the SPEED_TYPE
     * @return The SPEED_TYPE enum value of the SPEED_TYPE.
     */
    inline SPEED_TYPE getSpeedType(const std::string& type) {
        for (size_t i = 0; i < sizeof(speed_type_names) / sizeof(speed_type_names[0]); ++i)
            if (speed_type_names[i] == type)
                return static_cast<SPEED_TYPE>(i);
        return SPEED_TYPE::HIGH;
    }

    /// Class to handle different speeds of our processes.
    class Clock {
    public:
        /**
         * @brief Constructor
         * @param speed         The given speed
         */
        Clock(const SPEED_TYPE& speed);

        /**
         * @brief Sleep the process
         */
        void wait() const;

        /// Our current speed type
        SPEED_TYPE speed;
    };

}
