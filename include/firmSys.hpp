#ifndef FIRM_SYS_HPP
#define FIRM_SYS_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <list>
#include <set>
#include "firm.hpp"
#include "utils.hpp"
#include "template/stack_template.hpp"
#include "template/queue_template.hpp"
#include "template/linked_list_template.hpp"
#include "template/vector_template.hpp"
#include "template/matrix_template.hpp"

class IFirmSystem {
public:
    /* ------------------------------- initialization ------------------------------- */
    virtual void loadFirms(const std::string& filename) = 0;
    virtual void loadPatentsFromCSV(const std::string& filename) = 0;

    /* ------------------------------- operations ------------------------------- */
    virtual void addFirm(const std::string& firmID, const std::string& firmName) = 0;
    virtual void removeFirm(const std::string& firmID) = 0;
    virtual std::shared_ptr<IFirm> getFirm(const std::string& firmID) const = 0;
    virtual void addPatentFirm(const std::string& firmID, Patent& patent) = 0;
    virtual void removePatentFirm(const std::string& firmID, const std::string& patentID) = 0;
    virtual void transferPatent(const std::string& fromFirmID, const std::string& toFirmID, const std::string& patentID) = 0;
    virtual void searchPatentFirms(const std::string& keyword) = 0;

    /* --------------------------- patents application -------------------------- */
    virtual void loadApplicantsFromCSV(const std::string& filename) = 0;
    virtual void addApplicantToQueue(const Patent& patent) = 0;
    virtual void processApplicants() = 0;
    virtual bool changeApplicantInfo(Patent& patent) = 0;

    /* ------------------------------ sparse matrix ----------------------------- */
    virtual std::vector<std::vector<int>> toSparseMatrix() = 0;
    virtual int getCommonPatentCount(const std::string& firmID1, const std::string& firmID2) = 0;

    /* --------------------------------- display -------------------------------- */
    virtual void displayFirm(const std::string& firmID) const = 0;
    virtual void displayFirmInfo(const std::string& firmID) const = 0;
    virtual void displayFirms(int mode) const = 0;
    virtual void displayFirmsID() const = 0;
    virtual void displayFirmApplicants(const std::string& firmID) const = 0;
    virtual void displayFirmsAppl() const = 0;

    virtual ~IFirmSystem() = default;
    // 可以加查找；按id；按title-关键词、tf-idf
};

class BaseFirmSystem : public IFirmSystem {
public:
 
    void loadFirms(const std::string& filename) override {
        
        std::ifstream file(filename);
        std::string line, firmID, firmName;

        if (!file.is_open()) {
            std::cerr << "Error: Could not open FirmData.csv" << std::endl;
            file.close();
        }

        getline(file, line);
        
        while (getline(file, line)) {
            std::stringstream ss(line);
            getline(ss, firmID, ',');
            getline(ss, firmName, ',');

            firmName = cleanString(firmName);
            firmID = cleanString(firmID);

            addFirm(firmID, firmName);
        }

        file.close();
    }
    
    void loadPatentsFromCSV(const std::string& filename) override {
        std::ifstream file(filename);
        std::string line, patentID, grantdate, appldate, patent_title, country, firmID;

        if (!file.is_open()) {
            std::cerr << "Error: Could not open PatentData.csv" << std::endl;
            file.close();
        }

        getline(file, line);
        while (getline(file, line)) {
            std::stringstream ss(line);
            std::getline(ss, patentID, ',');
            // std::getline(ss, grantdate, ',');
            // std::getline(ss, appldate, ',');

            getline(ss, patent_title, ',');

            // alter for patent1.csv
            // getline(ss, country, ',');

            std::vector<std::string> firmIDs;
            
            if (line.find('"') != std::string::npos) {
                getline(ss, firmID, '"');
                getline(ss, firmID, '"');
                ss.ignore();
                std::stringstream firmIDStream(firmID);
                std::string singleFirmID;
                while (getline(firmIDStream, singleFirmID, ',')) {
                    firmIDs.push_back(cleanString(singleFirmID));
                }
            } else {
                getline(ss, firmID, ',');
                firmID = cleanString(firmID);
                std::cout << "firmID: " << firmID << std::endl;
                firmIDs.push_back(firmID);
            }

            for (const auto& firmID : firmIDs) {
                Patent p(patentID, grantdate, appldate, patent_title, country, firmID);
                addPatentFirm(firmID, p);
            }
        }

        file.close();
    }

    void displayFirm(const std::string& firmID) const override {
        auto firm = getFirm(firmID);
        if (firm) {
            displayLine(2);
            std::cout << "Firm ID: " << firm->getFirmID() << ", Firm Name: " << firm->getFirmName() << std::endl;
            std::cout << "Number of Patents: " << firm->getPatentCount() << std::endl;
            firm->displayPatents(50);
            std::cout << "-----------------------------------" << std::endl;
        }
    }

    void displayFirmInfo(const std::string& firmID) const override {
        auto firm = getFirm(firmID);
        if (firm) {
            displayLine(2);
            std::cout << "Firm ID: " << firm->getFirmID() << ", Firm Name: " << firm->getFirmName() << std::endl;
            std::cout << "Number of Patents: " << firm->getPatentCount() << std::endl;
            firm->displayPatents(3);
        }
    }
};

class FirmSystemVector : public BaseFirmSystem {
private:
    myVector<std::shared_ptr<IFirm>> fs;
    FirmType firmType;
public:
    FirmSystemVector(FirmType type) : firmType(type) {}

    void addFirm(const std::string& firmID, const std::string& firmName) override {
        std::shared_ptr<IFirm> firm;
        switch (firmType) {
            case FirmType::LinkedList:
                firm = std::make_shared<FirmLinkedList>(firmID, firmName);
                break;
            case FirmType::Vector:
                firm = std::make_shared<FirmVector>(firmID, firmName);
                break;
            case FirmType::Map:
                firm = std::make_shared<FirmMap>(firmID, firmName);
                break;
        }
        fs.push_back(firm);
    }

    void removeFirm(const std::string& firmID) override {
        auto it = std::find_if(fs.begin(), fs.end(), [&](const std::shared_ptr<IFirm>& f) {
            return f->getFirmID() == firmID;
        });
        if (it != fs.end()) {
            fs.remove_at(it - fs.begin());
            std::cout << "Firm removed successfully." << std::endl;
        } else {
            std::cerr << "Firm not found." << std::endl;
        }
    }

    void addPatentFirm(const std::string& firmID, Patent& patent) override {
        auto it = std::find_if(fs.begin(), fs.end(), [&](const std::shared_ptr<IFirm>& f) {
            return f->getFirmID() == firmID;
        });
        if (it != fs.end()) {
            (*it)->addPatent(patent);
        }
    }

    void removePatentFirm(const std::string& firmID, const std::string& patentID) override {
        auto it = std::find_if(fs.begin(), fs.end(), [&](const std::shared_ptr<IFirm>& f) {
            return f->getFirmID() == firmID;
        });
        if (it != fs.end()) {
            (*it)->removePatent(patentID);
        }
    }

    void transferPatent(const std::string& fromFirmID, const std::string& toFirmID, const std::string& patentID) override {
        std::shared_ptr<IFirm> fromFirm = nullptr;
        std::shared_ptr<IFirm> toFirm = nullptr;

        for (auto& f : fs) {
            if (f->getFirmID() == fromFirmID) {
                fromFirm = f;
            } else if (f->getFirmID() == toFirmID) {
                toFirm = f;
            }
        }

        if (fromFirm && toFirm) {
            Patent p = fromFirm->getPatent(patentID);
            toFirm->addPatent(p);
            fromFirm->removePatent(patentID);
        } else {
            std::cerr << "Error: One or both firms not found." << std::endl;
        }
    }

    std::shared_ptr<IFirm> getFirm(const std::string& firmID) const override {
        auto it = std::find_if(fs.begin(), fs.end(), [&](const std::shared_ptr<IFirm>& f) {
            return f->getFirmID() == firmID;
        });
        if (it != fs.end()) {
            return *it;
        }
        std::cerr << "Firm not found." << std::endl;
        return nullptr;
    }

    void displayFirmsID() const override {
        if (fs.size() == 0) {
            std::cout << "No firms available." << std::endl;
            return;
        }

        for (const auto& firm : fs) {
            std::cout << firm->getFirmName() << "  "<< firm->getFirmID() << std::endl;
        }
        std::cout << std::endl;
    }

    void displayFirms(int mode) const override {
        if (fs.size() == 0) {
            std::cout << "No firms available." << std::endl;
            return;
        }
        switch (mode)
        {
        case 1: // detail mode
            for (const auto& firm : fs) {
                displayFirm(firm->getFirmID());
            }
            break;

        case 2: // brief mode
            for (const auto& firm : fs) {
                displayFirmInfo(firm->getFirmID());
            }
            break;
        default:
            break;
        }
    }
};

class FirmSystemMap : public BaseFirmSystem {
private:
    std::map<std::string, std::shared_ptr<IFirm>> fs;
    FirmType firmType;
    LinkedListQueue<Patent> applQueue;
    LinkedListStack<std::string> allHistory; // 这个历史记录需要思考，是不是应该保存操作？
    LinkedListStack<Patent> historyStack;
    LinkedListQueue<Patent> rejQueue;
    LinkedListStack<std::string> rejHistory;
    std::map<std::string, std::vector<std::string>> firmApplCount;

    std::map<std::string, int> firmIndex;
    std::map<std::string, int> patentIndex;
    std::vector<std::vector<int>> commonPatentMatrix;

public:
    FirmSystemMap(FirmType type) 
        : firmType(type), 
          applQueue(), 
          allHistory(), 
          rejQueue(), 
          rejHistory(), 
          firmApplCount() {}

    void addFirm(const std::string& firmID, const std::string& firmName) override {
        std::shared_ptr<IFirm> firm;
        switch (firmType) {
            case FirmType::LinkedList:
                firm = std::make_shared<FirmLinkedList>(firmID, firmName);
                break;
            case FirmType::Vector:
                firm = std::make_shared<FirmVector>(firmID, firmName);
                break;
            case FirmType::Map:
                firm = std::make_shared<FirmMap>(firmID, firmName);
                break;
        }
        fs[firmID] = firm;
        firmApplCount[firmID] = {};
    }

    void removeFirm(const std::string& firmID) override {
        auto it = fs.find(firmID);
        if (it != fs.end()) {
            fs.erase(it);
            std::cout << "Firm removed successfully." << std::endl;
        } else {
            std::cerr << "Firm not found." << std::endl;
        }
    }

    void addPatentFirm(const std::string& firmID, Patent& patent) override {
        auto it = fs.find(firmID);
        if (it != fs.end()) {
            it->second->addPatent(patent);
            std::cout << "Patent | " << patent.getPatentID() << " | added successfully." << std::endl;
        } else {
            std::cerr << "Firm not found." << std::endl;
            std::cerr << "Add new firm to Firm System, new FirmID: " << firmID << std::endl;
            addFirm(firmID, "New Firm");
            addPatentFirm(firmID, patent); // 只会执行一次前面的if
        }
    }

    void removePatentFirm(const std::string& firmID, const std::string& patentID) override {
        auto it = fs.find(firmID);
        if (it != fs.end()) {
            it->second->removePatent(patentID);
        } else {
            std::cerr << "Firm not found." << std::endl;
        }
    }

    void transferPatent(const std::string& fromFirmID, const std::string& toFirmID, const std::string& patentID) override {
        auto fromIt = fs.find(fromFirmID);
        auto toIt = fs.find(toFirmID);

        if (fromIt != fs.end() && toIt != fs.end()) {
            Patent p = fromIt->second->getPatent(patentID);
            toIt->second->addPatent(p);
            fromIt->second->removePatent(patentID);
        } else {
            std::cerr << "Error: One or both firms not found." << std::endl;
        }
    }

    std::shared_ptr<IFirm> getFirm(const std::string& firmID) const override {
        auto it = fs.find(firmID);
        if (it != fs.end()) {
            return it->second;
        }
        std::cerr << "Firm not found." << std::endl;
        return nullptr;
    }


    void loadApplicantsFromCSV(const std::string& filename) override {
        std::ifstream file(filename);
        std::string line, patentID, appldate, patent_title, firmID, result, grantdate, country;

        if (!file.is_open()) {
            std::cerr << "Error: Could not open PatentData.csv" << std::endl;
            file.close();
        }

        getline(file, line);

        while (getline(file, line)) {
            std::stringstream ss(line);
            std::getline(ss, patentID, ',');
            std::getline(ss, appldate, ',');

            if (line.find('"') != std::string::npos) {
                getline(ss, patent_title, '"');
                getline(ss, patent_title, '"');
                ss.ignore();
            } else {
                getline(ss, patent_title, ',');
            }

            getline(ss, firmID, ',');
            getline(ss, result, ',');

            // 这里应该只是暂时的，后面会有判断标准，到时候还是一个一个push进applQueue，然后对applQueue进行排序，，

            firmID = cleanString(firmID);
            
            Patent p(patentID, grantdate, appldate, patent_title, country, firmID);
            p.setStatus(cleanString(result) == "y", 0);
            firmApplCount[firmID].push_back(patentID);
            addApplicantToQueue(p);
        }
        file.close();
    }

    void addApplicantToQueue(const Patent& patent) override {
        applQueue.push(patent);
    }

    void sortApplicants() {
        std::vector<Patent> tempVec = applQueue.toVector();
        std::sort(tempVec.begin(), tempVec.end(), [](const Patent& a, const Patent& b) {
            return a.getAppldate() < b.getAppldate();
        });
        applQueue.clear();
        for (const auto& patent : tempVec) {
            applQueue.push(patent);
        }
    }

    void processApplicants() override {
        // 如何加上撤销操作？
        sortApplicants();
        // applQueue.display();
        while (!applQueue.isEmpty()) {
            Patent p = applQueue.pop();
            // pushToHistoryStack(p);
            bool yes = p.getStatus().first;
            int times = p.getStatus().second;
            // std::cout << "Processing: " << p.getPatentID() << "status: " << yes << times << std::endl;
            if (yes) {
                allHistory.push(p.getPatentID());
                p.setGrantdate("20241012");
                addPatentFirm(p.getFirmID(), p);

                // 维护 firmApplCount
                auto& applList = firmApplCount[p.getFirmID()];
                applList.erase(std::remove(applList.begin(), applList.end(), p.getPatentID()), applList.end());
            }
            else {
                if (times < 1) {
                    allHistory.push(p.getPatentID());
                    rejHistory.push(p.getPatentID());
                    p.setStatus(false, times + 1);
                    rejQueue.push(p);
                }
                else {
                    displayFirms(2);
                    if (!changeApplicantInfo(p)) {
                        addApplicantToQueue(p);
                        break;
                    }
                    yes = p.getStatus().first;
                    if (yes) {
                        allHistory.push(p.getPatentID());
                        p.setGrantdate("20241012");
                        addPatentFirm(p.getFirmID(), p);

                        // 维护 firmApplCount
                        auto& applList = firmApplCount[p.getFirmID()];
                        applList.erase(std::remove(applList.begin(), applList.end(), p.getPatentID()), applList.end());
                    } else {
                        allHistory.push(p.getPatentID());
                        rejHistory.push(p.getPatentID());
                        rejQueue.push(p);
                    }
                }
            }

            if (applQueue.size() <= 3 && !rejQueue.isEmpty()) {
                addApplicantToQueue(rejQueue.pop());
            }
        }
    }
    
    void pushToHistoryStack(const Patent& patent) {
        if (historyStack.size() >= 3) {
            historyStack.pop();
        }
        historyStack.push(patent);
    }
    
    bool changeApplicantInfo(Patent& patent) override {
        std::string status;
        std::cout << std::endl;
        displayLine(2);
        displayTitle();
        patent.display();
        // std::cout << "Failed to pass the application. You need to change the apply info: " << std::endl;
        // std::string firmID, patentID, grantdate, appldate, title, country;
        // std::cout << "Update Firm ID: ";
        // std::cin >> firmID;
        // std::cout << "Update Patent ID: ";
        // std::cin >> patentID;
        // std::cout << "Update Grant Date: ";
        // std::cin >> grantdate;
        // std::cout << "Updaet Application Date: ";
        // std::cin >> appldate;
        // std::cout << "Update Title: ";
        // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // std::getline(std::cin, title);
        // std::cout << "Update Country: ";
        // std::cin >> country;
        // Patent patent(patentID, grantdate, appldate, title, country, firmID);
        std::cout << "set patent status y/n" << std::endl;
        std::cout << "enter '0' to exit" << std::endl;
        // std::cout << "enter 'z' to undo edit" << std::endl;
        std::cout << "input: ";
        std::cin >> status;
        if (status == "0") {
            system("clear");
            return false;
        }
        // if (status == "z") {
        //     applQueue.push(historyStack.top());
        // }
        patent.setStatus(cleanString(status) == "y", patent.getStatus().second);
        system("clear");
        return true;
    }

    void searchPatentFirms(const std::string& keyword) override {
        bool found = false;
        for (const auto& firm : fs) {
            std::vector<Patent> results = firm.second->searchPatent(keyword);
            if (results.size() > 0) {
                found = true;
                displayLine(2);
                std::cout << "Firm ID: " << firm.second->getFirmID() << std::endl;
                std::cout << "Number of Patents: " << results.size() << std::endl;
                displayTitle();
                for (const auto& patent : results) {
                    patent.display();
                }
            }
        }
        if (!found) {
            std::cout << "No patents found for the keyword: " << keyword << std::endl;
        }
    }

    std::vector<std::vector<int>> toSparseMatrix() override {
        std::vector<std::string> firmIDs;
        std::vector<std::string> patentIDs;

        std::set<std::string> uniquePatentIDs;
        for (const auto& pair : fs) {
            firmIDs.push_back(pair.first);
            const auto& patents = pair.second->getPatentIDs();
            for (const auto& patentID : patents) {
                if (uniquePatentIDs.insert(patentID).second) {
                    patentIDs.push_back(patentID);
                }
            }
        }

        std::sort(patentIDs.begin(), patentIDs.end());

        // firm index mapping
        for (size_t i = 0; i < firmIDs.size(); ++i) {
            firmIndex[firmIDs[i]] = i;
        }

        // patent index mapping
        for (size_t i = 0; i < patentIDs.size(); ++i) {
            patentIndex[patentIDs[i]] = i;
        }

        std::vector<std::vector<int>> sparseMatrix(firmIDs.size(), std::vector<int>(patentIDs.size(), 0));
        for (const auto& pair : fs) {
            const auto& firmID = pair.first;
            const auto& patents = pair.second->getPatentIDs();
            for (const auto& patentID : patents) {
                sparseMatrix[firmIndex[firmID]][patentIndex[patentID]] = 1;
            }
        }

        // 下面只是展示效果，实际都是用 OrthList<int> 来处理

        // displatMatrix(patentIDs, firmIDs, sparseMatrix);

        OrthList<int> olMatrix(sparseMatrix);

        displayLine(2);
        olMatrix.printSparseMatrix();
        displayLine(2);

        displayLine(2);
        olMatrix.printMatrix();
        displayLine(2);
        

        OrthList<int> olMatrixT = olMatrix.transpose();
        
        displayLine(2);
        olMatrixT.printSparseMatrix();
        displayLine(2);

        displayLine(2);
        olMatrixT.printMatrix();
        displayLine(2);

        std::vector<std::vector<int>> transformedMatrix = olMatrix.transformTo2DArray();

        OrthList<int> result = olMatrix.multiplyMatrix(olMatrixT);

        displayLine(2);
        result.printMatrix();
        displayLine(2);

        commonPatentMatrix = result.transformTo2DArray();

        // displatMatrix(patentIDs, firmIDs, commonPatentMatrix);

        return sparseMatrix;
    }

    int getCommonPatentCount(const std::string& firmID1, const std::string& firmID2) override {

        int idx_1 = firmIndex[firmID1];
        int idx_2 = firmIndex[firmID2];
        // std::cout << idx_1 << " " << idx_2 << std::endl;

        int commonPatentCount = commonPatentMatrix[idx_1][idx_2];
        std::cout << "Common Patent Count: " << commonPatentCount << std::endl << std::endl;
        return commonPatentCount;
    }

    void displayMatrix(std::vector<std::string> &patentIDs, std::vector<std::string> &firmIDs, std::vector<std::vector<int>> &sparseMatrix)
    {
        // Display matrix
        std::cout << "Firms-Patents Matrix:" << std::endl;
        std::cout << "      ";
        for (size_t j = 1; j < patentIDs.size() + 1; ++j)
        {
            std::cout << (j / 10);
        }
        std::cout << std::endl;
        std::cout << "      ";
        for (size_t j = 1; j < patentIDs.size() + 1; ++j)
        {
            std::cout << (j % 10);
        }
        std::cout << std::endl << std::endl;

        for (size_t i = 0; i < firmIDs.size(); ++i)
        {
            std::cout << firmIDs[i] << "  ";
            for (size_t j = 0; j < patentIDs.size(); ++j)
            {
                std::cout << sparseMatrix[i][j] << "";
            }
            std::cout << std::endl;
        }
    }

    void displayFirmsID() const override {
        if (fs.empty()) {
            std::cout << "No firms available." << std::endl;
            return;
        }
        for (const auto& pair : fs) {
            std::cout << pair.second->getFirmName() << ": " << pair.first << std::endl;
        }
        std::cout << std::endl;
    }

    void displayFirms(int mode) const override {
        if (fs.empty()) {
            std::cout << "No firms available." << std::endl;
            return;
        }
        switch (mode)
        {
        case 1: // detail mode
            for (const auto& pair : fs) {
                displayFirm(pair.first);
            }
            break;

        case 2: // brief mode
            for (const auto& pair : fs) {
                displayFirmInfo(pair.first);
            }
            break;
        default:
            break;
        }
    }

    void displayFirmApplicants(const std::string& firmID) const override {
        auto it = fs.find(firmID);
        if (it != fs.end()) {
            displayLine(2);
            std::cout << "Firm ID: " << it->second->getFirmID() << ", Firm Name: " << it->second->getFirmName() << std::endl;
            std::cout << "Number of Patents: " << it->second->getPatentCount() << std::endl;
            // it->second->displayPatents();
            // std::cout << "-----------------------------------" << std::endl;
            auto it = firmApplCount.find(firmID);
            if (it != firmApplCount.end()) {
                std::cout << "Number of Applicants: " << it->second.size() << std::endl;
            } else {
                std::cerr << "Firm not found in firmApplCount." << std::endl;
            }
        } else {
            std::cerr << "Firm not found." << std::endl;
        }
    }

    void displayFirmsAppl() const override{
        for (const auto& pair : firmApplCount) {
            displayFirmApplicants(pair.first);
        }
    }

};

#endif