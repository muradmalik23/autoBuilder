#pragma  once

inline double clamp(double value, double min, double max){
    if (value > max){
        return max;
    } else if (value < min){
        return min;
    } else {
        return value;
    }
}

inline bool inRange(double value, double min, double max){
    if (min>max){//handle looped input 
        return (value >= min || value <= max);
    } else {
        return (clamp(value, min, max) == value);
    }
}