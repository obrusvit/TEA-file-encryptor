#include "TEA.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <iostream>

const std::string asset_path = ASSET_PATH;

std::string trim_trailing_whitespaces(const std::string& s) {
    size_t last = s.find_last_not_of(' ');
    return s.substr(0, (last + 1));
}

TEST_CASE("Test encrypt") {
    const std::string obj = asset_path + "/test1.txt";
    const std::string obj_res = asset_path + "/test1_result.txt";
    const std::string obj_desired = asset_path + "/test1_encrypted_key123.txt";
    {
        std::ifstream ifs{obj};
        REQUIRE(ifs.is_open());
        std::ofstream ofs{obj_res};
        REQUIRE(ofs.is_open());
        std::string key{"key123"};
        tea::encrypt_file(ifs, ofs, key);
    }

    std::ifstream ifs_result{obj_res};
    std::ifstream ifs_desired{obj_desired};
    REQUIRE(ifs_result.is_open());
    REQUIRE(ifs_desired.is_open());

    std::string line_result;
    std::string line_desired;
    std::getline(ifs_result, line_result);
    std::getline(ifs_desired, line_desired);

    REQUIRE(trim_trailing_whitespaces(line_result) == trim_trailing_whitespaces(line_desired));

    std::remove(obj_res.data());
}

TEST_CASE("Test decrypt") {
    // Example from the book: "Programming Principles and Practice using C++", chapter 25.5.6
    const std::string obj = asset_path + "/test2.txt";
    const std::string obj_res = asset_path + "/test2_result.txt";
    const std::string obj_desired = asset_path + "/test2_decrypted_bs.txt";
    {
        std::ifstream ifs{obj};
        REQUIRE(ifs.is_open());
        std::ofstream ofs{obj_res};
        REQUIRE(ofs.is_open());
        std::string key{"bs"};
        tea::decrypt_file(ifs, ofs, key);
    }

    std::ifstream ifs_result{obj_res};
    std::ifstream ifs_desired{obj_desired};
    REQUIRE(ifs_result.is_open());
    REQUIRE(ifs_desired.is_open());

    std::string line_result;
    std::string line_desired;
    std::getline(ifs_result, line_result);
    std::getline(ifs_desired, line_desired);

    REQUIRE(trim_trailing_whitespaces(line_result) == trim_trailing_whitespaces(line_desired));

    std::remove(obj_res.data());
}
