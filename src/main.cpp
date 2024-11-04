#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <limits>
#include <map>
#include "firm.hpp"
#include "firmSys.hpp"
#include "./template/linked_list_template.hpp"
#include "./template/vector_template.hpp"

void printMenu() {
    std::cout << "\n=====================================" << std::endl;
    std::cout << "         PATENT SYSTEM MENU          " << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "          PATENT OPERATIONS          " << std::endl;
    std::cout << "1. Add Patent to Firm" << std::endl;
    std::cout << "2. Remove Patent from Firm" << std::endl;
    std::cout << "3. Transfer Patent between Firms" << std::endl;
    std::cout << "-------------------------------------" << std::endl;
    
    std::cout << "           FIRM OPERATIONS           " << std::endl;
    std::cout << "4. Display Firm Details" << std::endl;
    std::cout << "5. Display All Firms Info" << std::endl;
    std::cout << "6. Add Firm" << std::endl;
    std::cout << "7. Remove Firm" << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    std::cout << "                GRANT                " << std::endl;
    std::cout << "8. Start patent granting pipeline" << std::endl;
    std::cout << "9. Display firms' desired patents" << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    std::cout << "                 NEW                 " << std::endl;
    std::cout << "10. Search Patent Firms" << std::endl;
    std::cout << "11. Display Firms-Patents Matrix" << std::endl;
    std::cout << "12. Find Common Patents Between Firms (execute 11 first)" << std::endl;

    std::cout << "-------------------------------------" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << "Select an option: ";
}

int main() {
    FirmType firmType;
    int typeChoice;
    
    system("clear");

    std::cout << "Select the Firm(Patents) Data Structure to use:" << std::endl;
    std::cout << "1. LinkedList" << std::endl;
    std::cout << "2. Vector" << std::endl;
    std::cout << "3. Map" << std::endl;
    std::cout << "Enter choice: ";
    std::cin >> typeChoice;

    switch (typeChoice) {
        case 1:
            firmType = FirmType::LinkedList;
            break;
        case 2:
            firmType = FirmType::Vector;
            break;
        case 3:
            firmType = FirmType::Map;
            break;
        default:
            std::cerr << "Invalid choice. Defaulting to Map." << std::endl;
            firmType = FirmType::Map;
    }

    // std::shared_ptr<IFirmSystem> firmSystem;
    // std::cout << "Select the Firm System Data Structure to use:" << std::endl;
    // std::cout << "1. Vector" << std::endl;
    // std::cout << "2. Map" << std::endl;
    // std::cout << "Enter choice: ";
    // std::cin >> typeChoice;

    // switch (typeChoice) {
    //     case 1:
    //         firmSystem = std::make_shared<FirmSystemVector>(firmType);
    //         break;
    //     case 2:
    //         firmSystem = std::make_shared<FirmSystemMap>(firmType);
    //         break;
    //     default:
    //         std::cerr << "Invalid choice. Defaulting to Map." << std::endl;
    //         firmSystem = std::make_shared<FirmSystemMap>(firmType);
    // }

    std::shared_ptr<IFirmSystem> firmSystem = std::make_shared<FirmSystemMap>(firmType);
    system("clear");

    std::string filename="../data/FirmData.csv";
    // firmSystem->loadFirms(filename);
    filename="../data/FirmPatent.csv";
    firmSystem->loadPatentsFromCSV(filename);
    // filename="../data/patent2.csv";
    // firmSystem->loadApplicantsFromCSV(filename);

    system("clear");

    int choice;
    do {
        printMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: {
                system("clear");
                std::string firmID, patentID, grantdate, appldate, title, country;
                firmSystem->displayFirmsID();
                std::cout << "Enter Firm ID to add Patent to: ";
                std::cin >> firmID;
                if (!firmSystem->getFirm(firmID)) {
                    break;
                };
                std::cout << "Enter Patent ID: ";
                std::cin >> patentID;
                std::cout << "Enter Grant Date: ";
                std::cin >> grantdate;
                std::cout << "Enter Application Date: ";
                std::cin >> appldate;
                std::cout << "Enter Title: ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, title);
                std::cout << "Enter Country: ";
                std::cin >> country;
                Patent patent(patentID, grantdate, appldate, title, country, firmID);
                firmSystem->addPatentFirm(firmID, patent);
                break;
            }
            case 2: {
                system("clear");
                std::string firmID, patentID;
                firmSystem->displayFirmsID();
                std::cout << "Enter Firm ID to remove Patent from: ";
                std::cin >> firmID;
                if (!firmSystem->getFirm(firmID)) {
                    break;
                };
                std::cout << "Enter Patent ID: ";
                std::cin >> patentID;
                firmSystem->removePatentFirm(firmID, patentID);
                std::cout << "Patent deleted successfully." << std::endl;
                break;
            }
            case 3: {
                system("clear");
                firmSystem->displayFirmsID();
                std::string fromFirmID, toFirmID, patentID;
                std::cout << "Enter Source Firm ID: ";
                std::cin >> fromFirmID;
                if (!firmSystem->getFirm(fromFirmID)) {
                    break;
                };
                std::cout << "Enter Destination Firm ID: ";
                std::cin >> toFirmID;
                if (!firmSystem->getFirm(toFirmID)) {
                    break;
                };
                std::cout << "Enter Patent ID to transfer: ";
                std::cin >> patentID;
                firmSystem->transferPatent(fromFirmID, toFirmID, patentID);
                std::cout << "Patent transfered successfully." << std::endl;
                break;
            }
            case 4: {
                system("clear");
                firmSystem->displayFirmsID();
                std::string firmID;
                std::cout << "Enter Firm ID to display: ";
                std::cin >> firmID;
                firmSystem->displayFirm(firmID);
                break;
            }
            case 5: {
                system("clear");
                firmSystem->displayFirms(1);
                break;
            }
            case 6: {
                system("clear");
                std::string firmID, firmName;
                std::cout << "Enter Firm ID: ";
                std::cin >> firmID;
                std::cout << "Enter Firm Name: ";
                std::cin >> firmName;
                firmSystem->addFirm(firmID, firmName);
                break;
            }
            case 7: {
                system("clear");
                firmSystem->displayFirmsID();
                std::string firmID;
                std::cout << "Enter Firm ID to remove: ";
                std::cin >> firmID;
                firmSystem->removeFirm(firmID);
                break;
            }
            case 8: {
                system("clear");
                firmSystem->processApplicants();
                // 这里应该支持暂停
                break;
            }
            case 9: {
                system("clear");
                firmSystem->displayFirmsAppl();
                break;
            }
            case 10: {
                system("clear");
                std::string keyword;
                std::cout << "Enter keyword to search: ";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, keyword);
                keyword = cleanString(keyword);
                firmSystem->searchPatentFirms(keyword);
                break;
            }
            case 11: {
                system("clear");
                firmSystem->toSparseMatrix();
                break;
            }
            case 12: {
                system("clear");
                firmSystem->displayFirmsID();
                std::string firmID1, firmID2;
                std::cout << "Firm ID 1: ";
                std::cin >> firmID1;
                std::cout << "Firm ID 2: ";
                std::cin >> firmID2;
                firmSystem->getCommonPatentCount(firmID1, firmID2);
                break;
            }
            case 0: {
                std::cout << "Exiting..." << std::endl;
                break;
            }
            default: {
                system("clear");
                std::cerr << "Invalid choice. Please try again." << std::endl;
                break;
            }
        }
    } while (choice != 0);

    return 0;
}
