#pragma once

namespace bpc_prp_opencv_lib {

    class Detection {

    public:

        enum class DetectionType {
            kWorker,
            kZombie,
            kNone,
        };

        Detection(DetectionType type, float distance, float direction)
        : type_{type}
        , distance_{distance}
        , direction_{direction} {

        }

        DetectionType getType() const {return type_;}
        std::string getTypeAsString() const {
            switch(type_) {
                case DetectionType::kWorker: return std::string("Worker");
                case DetectionType::kZombie: return std::string("Zombie");
                case DetectionType::kNone: return std::string("None");
            }
        }
        float getDistance() const {return distance_;}
        float getDirection() const { return direction_;}

    private:

        DetectionType type_;
        float distance_;
        float direction_;

    };

}