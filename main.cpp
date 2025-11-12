#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#pragma region Структуры
struct Date {
    int day;
    int month;
    int year;
};

struct Person_info {
    std::string name;
    std::string surname;
    Date birth_date;
};
#pragma endregion

#pragma region Функции
Date ParseDateIntoVector(const std::string &date) {
    Date result_date;

    result_date.day = stoi(date.substr(0, 2));
    result_date.month = stoi(date.substr(3, 2));
    result_date.year = stoi(date.substr(6));

    return result_date;
}


Person_info ParseLineIntoPersonInfo(const std::string &line, const std::string &separator) {
    Person_info person_info;
    std::vector<std::string> vector_of_strings;
    std::string::size_type start = 0;
    std::string::size_type end = line.find(separator);

    while (end != std::string::npos) {
        vector_of_strings.push_back(line.substr(start, end - start));
        start = end + 1;
        end = line.find(separator, start);
    }

    vector_of_strings.push_back(line.substr(start));

    person_info.surname = vector_of_strings[0];
    person_info.name = vector_of_strings[1];
    person_info.birth_date = ParseDateIntoVector(vector_of_strings[2]);

    return person_info;
}


std::vector<Person_info> ReadFileIntoVector(const std::string &file_path, const std::string &separator) {
    std::vector<Person_info> vector_of_person_info;
    std::ifstream file;

    file.open(file_path);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            vector_of_person_info.push_back(ParseLineIntoPersonInfo(line, separator));
        }
    }

    file.close();

    return vector_of_person_info;
}


void WriteInfoIntoJSONFile(const std::string& file_path, std::vector<Person_info>& vector) {
    std::ofstream file;

    file.open(file_path);

    file << "[" << std::endl;

    for (Person_info& element : vector) {
        file << "  {" << std::endl;

        file << R"(    "last_name": )" << R"(")" + element.surname + R"(",)" << std::endl;
        file << R"(    "first_name": )" << R"(")" + element.name + R"(",)" << std::endl;
        file << R"(    "date_of_birth": {)" << std::endl;
        file << R"(      "year": )" << element.birth_date.year << "," << std::endl;
        file << R"(      "month": )" << element.birth_date.month << "," << std::endl;
        file << R"(      "day": )" << element.birth_date.day << std::endl;
        file << "    }" << std::endl;

        if (&element != &vector[vector.size() - 1]) {
            file << "  }," << std::endl;
        }
        else {
            file << "  }" << std::endl;
        }

    }

    file << "]";

    file.close();
}
#pragma endregion

int main() {
    std::string file_path_to_read;
    std::string file_path_to_write;
    std::string separator;

    std::cout << "Type file path for read: ";
    std::cin >> file_path_to_read;

    std::cout << "Type separator: ";
    std::cin >> separator;

    std::cout << "Type file path for write: ";
    std::cin >> file_path_to_write;

    std::vector<Person_info> vector_of_person_info = ReadFileIntoVector(file_path_to_read, separator);

    WriteInfoIntoJSONFile(file_path_to_write, vector_of_person_info);
}
