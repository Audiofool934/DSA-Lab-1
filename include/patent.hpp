#ifndef PATENT_HPP
#define PATENT_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <list>
#include "template/linked_list_template.hpp"
#include "template/vector_template.hpp"

class Patent {
private:
    std::string patentID;
    std::string grantdate;
    std::string appldate;
    std::string title;
    std::string country;
    std::string firmID;
    std::pair<bool, int> status;

public:
    Patent() {}

    Patent(
        std::string patentID, std::string grantdate, std::string appldate, std::string title, std::string country, std::string firmID
    ) : patentID(patentID), grantdate(grantdate), appldate(appldate), title(title), country(country), firmID(firmID) {}

    std::string getPatentID() const { return patentID; }
    std::string getGrantdate() const { return grantdate; }
    std::string getAppldate() const { return appldate; }
    std::string getTitle() const { return title; }
    std::string getCountry() const { return country; }
    std::string getFirmID() const { return firmID; }
    std::pair<bool, int> getStatus() const { return status; }

    void setStatus(bool status, int value) { this->status = std::make_pair(status, value); }
    void setFirmID(const std::string& firmID) { this->firmID = firmID; }
    void setGrantdate(const std::string& grantdate) { this->grantdate = grantdate; }

    void display() const {
        std::cout << std::left
                << std::setw(10) << patentID
                << std::setw(13) << grantdate
                << std::setw(13) << appldate
                << std::setw(30) << (title.size() > 30 ? title.substr(0, 25) + "..." : title)
                << std::setw(10) << country
                << std::setw(10) << firmID
                << std::endl;
    }

    bool operator==(const Patent& other) const {
        return patentID == other.getPatentID();
    }

    bool operator!=(const Patent& other) const {
        return !(*this == other);
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Patent& patent);

    ~Patent() = default;
};

std::ostream& operator<<(std::ostream& os, const Patent& patent) {
    os << "PatentID: " << patent.getPatentID();
    return os;
}

void displayTitle() {
    std::cout << std::left
            << std::setw(10) << "PatentID"
            << std::setw(13) << "Grant Date"
            << std::setw(13) << "Appl Date"
            << std::setw(30) << "Title"
            << std::setw(10) << "Country"
            << std::setw(10) << "FirmID"
            << std::endl;

    std::cout << std::string(80, '-') << std::endl;
}

void displayDots() {
    std::cout << std::left
            << std::setw(10) << "..."
            << std::setw(13) << "..."
            << std::setw(13) << "..."
            << std::setw(30) << "..."
            << std::setw(10) << "..."
            << std::setw(10) << "..."
            << std::endl;

    std::cout << std::string(80, '-') << std::endl;
}

#endif