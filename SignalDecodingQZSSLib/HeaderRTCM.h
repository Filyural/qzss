#pragma once

struct MessageType
{
    bool subframe_indicator;
    bool applicable_nav_mess_extension;
    bool correction_service_ID;
    unsigned short message_generation_facility_ID;
    unsigned int vendor_ID;

    std::string getInfo()
    {
        std::string result;
        result += "|\tSubframe_indicator:\t\t\t\t" +
                  + subframe_indicator ? "First data part of a subframe.\n" : "Another data part of a subframe.\n";
        result += "|\tApplicable navigation message extension:\t" + std::to_string(applicable_nav_mess_extension) + "\n";
        result += "|\tCorrection Service ID:\t\t\t\t"
                  + correction_service_ID ? "Clock/Ephemeris Corrections\n" : "Ionospheric Corrections\n";
        switch (message_generation_facility_ID)
        {
        case 0:
        case 1:
            result += "|\tMessage generation facility ID:\t\t\tHitachi-Ota\n";
            break;
        case 2:
        case 3:
            result += "|\tMessage generation facility ID:\t\t\tKobe\n";
            break;
        default:
            break;
        }

        result += "|\tVendor ID:\t\t\t\t\t";
        switch (vendor_ID)
        {
        case 5:
            result += "CLAS.\n";
            break;
        case 2:
            result += "MADOCA-PPP.\n";
            break;
        case 3:
            result += "QZNMA.\n";
            break;
        default:
            result += "Reserved:\t" + std::to_string(vendor_ID) + "\n";
            break;
        }
        return result;
    }
};

class HeaderRTCM
{

private:
    BitContainer bits;
    unsigned int time_of_week;
    unsigned short correction_error_bits;
    unsigned short PRN;
    MessageType message_type;
    bool alert_flag;

public:
    HeaderRTCM(BitContainer message);

    unsigned int getTimeOfWeek()
    {
        return time_of_week;
    }
    unsigned short getCorrectionErrorBits()
    {
        return correction_error_bits;
    }
    unsigned short getPRN()
    {
        return PRN;
    }
    MessageType getMessageType()
    {
        return message_type;
    }
    bool getAlertFlag()
    {
        return alert_flag;
    }
    std::string getInfo();
    // std::string getStringTOW();
    // std::string getStringCEB();
    // std::string getStringPRN();
    // std::string getStringMessageType();
    // std::string getStringAlertFlag();
};
