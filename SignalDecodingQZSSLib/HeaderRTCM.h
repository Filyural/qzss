#pragma once

struct MessageType
{
    bool subframe_indicator;
    bool applicable_nav_mess_extension;
    bool correction_service_ID;
    unsigned short message_generation_facility_ID;
    unsigned int vendor_ID;

    void showInfo()
    {
        std::cout << "|\tSubframe_indicator:\t\t\t\t"
                  << (subframe_indicator ? "First data part of a subframe." : "Another data part of a subframe.") << std::endl;
        std::cout << "|\tApplicable navigation message extension:\t" << applicable_nav_mess_extension << std::endl;
        std::cout << "|\tCorrection Service ID:\t\t\t\t"
                  << (correction_service_ID ? "Clock/Ephemeris Corrections" : "Ionospheric Corrections") << std::endl;
        switch (message_generation_facility_ID)
        {
        case 0:
        case 1:
            std::cout << "|\tMessage generation facility ID:\t\t\tHitachi-Ota" << std::endl;
            break;
        case 2:
        case 3:
            std::cout << "|\tMessage generation facility ID:\t\t\tKobe" << std::endl;
            break;
        default:
            break;
        }

        std::cout << "|\tVendor ID:\t\t\t\t\t";
        switch (vendor_ID)
        {
        case 5:
            std::cout << "CLAS." << std ::endl;
            break;
        case 2:
            std::cout << "MADOCA-PPP." << std ::endl;
            break;
        case 3:
            std::cout << "QZNMA." << std ::endl;
            break;
        default:
            std::cout << "Reserved:\t" << vendor_ID << std ::endl;
            break;
        }
    }
};

class HeaderRTCM
{

private:
    Bits::BitContainer bits;
    unsigned int time_of_week;
    unsigned short correction_error_bits;
    unsigned short PRN;
    MessageType message_type;
    bool alert_flag;

public:
    HeaderRTCM(Bits::BitContainer message);

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
    void showInfo();
    // std::string getStringTOW();
    // std::string getStringCEB();
    // std::string getStringPRN();
    // std::string getStringMessageType();
    // std::string getStringAlertFlag();
};
