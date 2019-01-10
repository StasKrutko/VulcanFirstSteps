#pragma once

#include <vector>
#include <fstream>

using namespace std;

vector<char> read_file(const string& file_name)
{
    ifstream file(file_name, ios::ate | ios::binary);

    if (!file.is_open())
    {
        throw runtime_error("Failed to open file!");
    }

    size_t file_size = static_cast<size_t>(file.tellg());
    vector<char> file_buffer(file_size);

    file.seekg(0);
    file.read(file_buffer.data(), file_size);
    file.close();
    return file_buffer;
}
