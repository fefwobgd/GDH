#include "labels.hpp"

std::vector<labels::Label> labels::labels_top_left = {};
std::vector<labels::Label> labels::labels_top_right = {};
std::vector<labels::Label> labels::labels_bottom_left = {};
std::vector<labels::Label> labels::labels_bottom_right = {};
std::vector<labels::Label> labels::labels_bottom = {};
std::vector<labels::Label> labels::labels_top = {};

float labels::label_size = 0.4f;
int labels::label_opacity = 150;
float labels::label_padding = 5.f;

const char* labels::label_types[COUNT_LABELS] = {
    "Time (24h)",
    "Time (12h)",
    "Noclip Accuracy",
    "CPS Counter",
    "Death Counter",
    "Attempt Counter",
    "Session Time",
    "Custom Text"
};

// i couldnt figure out where to put these so here they are
int labels::attempts = 0;
float labels::attempt_time = 0;

std::string labels::get_label_string_repr(labels::Label const& label) {
    if (label.type == labels::LABEL_TIME12) {
	std::stringstream ss;
        SYSTEMTIME localTime;
        GetLocalTime(&localTime);

        int hour = localTime.wHour;
        std::string period = (hour < 12) ? "AM" : "PM";
        if (hour == 0) hour = 12;
        if (hour > 12) hour -= 12;
        
        ss << std::setfill('0') << std::setw(2) << hour << ":" 
        << std::setw(2) << localTime.wMinute << ":" << std::setw(2) << localTime.wSecond << " " << period;
        return ss.str();
	
    } else if (label.type == labels::LABEL_TIME24) {
	std::stringstream ss;
       SYSTEMTIME localTime;
       GetLocalTime(&localTime);
	
       ss << std::setfill('0') << std::setw(2) << localTime.wHour << ":" 
       << std::setw(2) << localTime.wMinute << ":" << std::setw(2) << localTime.wSecond;
       return ss.str();
	
    } else if (label.type == labels::LABEL_NOCLIP_ACCURACY) {
       std::stringstream formatedPercentage;
       formatedPercentage << std::fixed << std::setprecision(2) << noclip_accuracy.getPercentage();

       return formatedPercentage.str() + "%";

    } else if (label.type == labels::LABEL_CPS_COUNTER) {
       cps_counter.update();
       return std::to_string(cps_counter.cps) + "/" + std::to_string(cps_counter.highscore);
	
    } else if (label.type == labels::LABEL_DEATH_COUNTER) {
	return std::to_string(noclip_accuracy.deaths_full) + " Deaths";
        
    } else if (label.type == labels::LABEL_ATTEMPTS) {
	return "Attempt " + std::to_string(labels::attempts+1);
        
    } else if (label.type == labels::LABEL_ATTEMPT_TIME) {
        std::stringstream ss;
        int time = (int) labels::attempt_time;
        int minutes = time / 60;
        int hours = minutes / 60;
        ss << std::setfill('0') << std::setw(2) << hours % 60 << ":" 
        << std::setw(2) << minutes % 60 << ":" << std::setw(2) << time % 60;
	 return "Session: " + ss.str();
        
    } else if (label.type == labels::LABEL_CUSTOM_TEXT) {
	return label.text;
        
    }
    return (std::string) "unreachable";
}

std::string labels::get_labels(int position) {
    std::string label;
    std::vector<labels::Label> label_vector;
    
    if (position == 0) label_vector = labels_top_left;
    if (position == 1) label_vector = labels_top_right;
    if (position == 2) label_vector = labels_bottom_left;
    if (position == 3) label_vector = labels_bottom_right;
    if (position == 4) label_vector = labels_bottom;
    if (position == 5) label_vector = labels_top;
    
    for (auto& l : label_vector) {
	label += labels::get_label_string_repr(l);
	label += "\n";
    }
    
    return label;
}
