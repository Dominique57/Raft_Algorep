#pragma once

#include "utils/string.hh"

namespace Clock {

    enum SPEED_TYPE : unsigned int {
        HIGH = 0,
        MEDIUM = 1,
        LOW = 2,
    };

    static const char *speed_type_names[] = {
        "high",
        "medium",
        "low",
    };

    inline const char *getSpeedTypeName(const SPEED_TYPE& type) {
        auto typeIndex = static_cast<unsigned int>(type);

        if (typeIndex >= sizeof(speed_type_names) / sizeof(speed_type_names[0]))
            return "Unkown Name";

        return speed_type_names[typeIndex];
    }

    inline SPEED_TYPE getSpeedType(const std::string& type) {
        for (size_t i = 0; i < sizeof(speed_type_names) / sizeof(speed_type_names[0]); ++i)
            if (speed_type_names[i] == type)
                return static_cast<SPEED_TYPE>(i);
        return SPEED_TYPE::HIGH;
    }

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

        SPEED_TYPE speed;
    };

}
