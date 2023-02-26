#include <iostream>
#include <fstream>
#include <iomanip>


std::string output_file_extension(const std::string& input_filename) {
    const size_t last_dot_pos = input_filename.find_last_of('.');
    if (last_dot_pos == std::string::npos) {
        // no extension
        return input_filename + "_bytes.h";
    }
    else {
        return input_filename.substr(0, last_dot_pos) + "_bytes.h";
    }
}

std::string pure_filename(const std::string& input_filename) {
    const size_t last_slash_pos = input_filename.find_last_of("/\\");
    const size_t last_dot_pos = input_filename.find_last_of('.');
    if (last_dot_pos == std::string::npos) {
        // no extension
        if (last_slash_pos == std::string::npos) {
            return input_filename;
        }
        else {
            return input_filename.substr(last_slash_pos + 1);
        }
    }
    else {
        if (last_slash_pos == std::string::npos) {
            return input_filename.substr(0, last_dot_pos);
        }
        else {
            return input_filename.substr(last_slash_pos + 1, last_dot_pos - last_slash_pos - 1);
        }
    }
}

int main(int argc, char* argv[])
{
    std::string filename;
    if (argc > 1) {
        filename = argv[1];
    }
    else {
        std::cout << "Enter filename: ";
        std::cin >> filename;
    }

    std::ifstream input_file(filename, std::ios::binary);
    if (!input_file.is_open()) {
        std::cout << "unable to open file" << std::endl;
        return 1;
    }

    const std::string input_filename = argv[1];
    const std::string output_filename = output_file_extension(input_filename);
    const std::string array_name = pure_filename(input_filename);

    input_file.seekg(0, std::ios::end);
    size_t file_size = input_file.tellg();
    input_file.seekg(0, std::ios::beg);

    unsigned char* file_data = new unsigned char[file_size];

    input_file.read((char*)file_data, file_size);

    input_file.close();

    std::ofstream output_file(output_filename);

    if (!output_file.is_open()) {
        std::cout << "error creating output" << std::endl;
        delete[] file_data;
        return 1;
    }

    output_file << "unsigned char " << array_name << "[" << file_size << "] = {" << std::endl
        << "    ";

    for (size_t i = 0; i < file_size; i++) {
        if (i % 12 == 0 && i != 0) {
            output_file << std::endl << "    ";
        }
        output_file << "0x" << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << (unsigned int)file_data[i];
        if (i != file_size - 1) {
            output_file << ", ";
        }
    }

    output_file << std::endl << "};" << std::endl;

    output_file.close();

    delete[] file_data;

    std::cout << "success" << std::endl;

    return 0;
}
