#ifndef FIRM_HPP
#define FIRM_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <list>
#include "patent.hpp"
#include "template/linked_list_template.hpp"
#include "template/vector_template.hpp"

class IFirm {
public:
    virtual std::string getFirmID() const = 0;
    virtual std::string getFirmName() const = 0;
    virtual int getPatentCount() const = 0;
    virtual void displayPatents(int num) const = 0;
    virtual void addPatent(Patent& patent) = 0;
    virtual void removePatent(const std::string& patentID) = 0;
    virtual const Patent getPatent(const std::string& patentID) const = 0;
    virtual ~IFirm() {}
};

enum class FirmType {
    LinkedList,
    Vector,
    UnorderedMap
};

class BaseFirm : public IFirm {
// 这里可以抽象出来，减少重复代码
};

class FirmLinkedList : public IFirm {
private:
    std::string firmID;
    std::string firmName;
    int patentCount;
    SinglyLinkedList<Patent> patents;
    // SinglyLinkedList<Patent> patentsToBeProcessed; // 这个是不是应该放到firmSystem里面？

public:
    FirmLinkedList() : patentCount(0) {}

    FirmLinkedList(std::string firmID, std::string firmName) : firmID(firmID), firmName(firmName), patentCount(0) {}

    std::string getFirmID() const override { return firmID; }
    std::string getFirmName() const override { return firmName; }
    int getPatentCount() const override { return patentCount; }
    
    void displayPatents(int num) const override {
        displayTitle();
        auto current = patents.getHead();
        int i = 0;
        while (current != nullptr && i < num) {
            current->data.display();
            current = current->next;
            i++;
        }
        displayDots();
    }

    void addPatent(Patent& patent) override {
        patent.setFirmID(firmID);
        patents.insert(patent);
        // 这里其实应该加一个检测重复，如果重复的话，就update
        patentCount++;
    }

    void removePatent(const std::string& patentID) override {
        Patent tempPatent(patentID, "", "", "", "", "");
        try {
            patents.remove(tempPatent);
            patentCount--;
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    const Patent getPatent(const std::string& patentID) const override {
        Patent tempPatent(patentID, "", "", "", "", "");
        return patents.find_and_return(tempPatent);
    }

    ~FirmLinkedList() = default;
};

class FirmVector : public IFirm {
private:
    std::string firmID;
    std::string firmName;
    int patentCount;
    myVector<Patent> patents;

public:
    FirmVector() : patentCount(0) {}

    FirmVector(std::string firmID, std::string firmName) : firmID(firmID), firmName(firmName), patentCount(0) {}

    std::string getFirmID() const override { return firmID; }
    std::string getFirmName() const override { return firmName; }
    int getPatentCount() const override { return patentCount; }

    void displayPatents(int num) const override {
        displayTitle();
        for (size_t i = 0; i < patents.size() && i < num; ++i) {
            patents[i].display();
        }
        displayDots();
    }

    void addPatent(Patent& patent) override {
        patent.setFirmID(firmID);
        patents.push_back(patent);
        patentCount++;
    }

    void removePatent(const std::string& patentID) override {
        for (size_t i = 0; i < patents.size(); ++i) {
            if (patents[i].getPatentID() == patentID) {
                patents.remove_at(i);
                patentCount--;
                return;
            }
        }
        std::cerr << "Error: Patent not found." << std::endl;
    }

    const Patent getPatent(const std::string& patentID) const override {
        for (size_t i = 0; i < patents.size(); ++i) {
            if (patents[i].getPatentID() == patentID) {
                return patents[i];
            }
        }
        throw std::invalid_argument("Patent not found");
    }

    ~FirmVector() = default;
};

class FirmUnorderedMap : public IFirm {
private:
    std::string firmID;
    std::string firmName;
    int patentCount;
    std::unordered_map<std::string, Patent> patents;

public:
    FirmUnorderedMap() : patentCount(0) {}

    FirmUnorderedMap(std::string firmID, std::string firmName) : firmID(firmID), firmName(firmName), patentCount(0) {}

    std::string getFirmID() const override { return firmID; }
    std::string getFirmName() const override { return firmName; }
    int getPatentCount() const override { return patentCount; }

    void displayPatents(int num) const override {
        displayTitle();
        int i = 0;
        for (const auto& pair : patents) {
            if (i >= num) {
                break;
            }
            pair.second.display();
        }
        displayDots();
    }

    void addPatent(Patent& patent) override {
        patent.setFirmID(firmID);
        patents[patent.getPatentID()] = patent;
        patentCount++;
    }

    void removePatent(const std::string& patentID) override {
        auto it = patents.find(patentID);
        if (it != patents.end()) {
            patents.erase(it);
            patentCount--;
        } else {
            std::cerr << "Error: Patent not found." << std::endl;
        }
    }

    const Patent getPatent(const std::string& patentID) const override {
        auto it = patents.find(patentID);
        if (it != patents.end()) {
            return it->second;
        }
        throw std::invalid_argument("Patent not found");
    }

    ~FirmUnorderedMap() = default;
};

#endif