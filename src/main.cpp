#include <iostream>
#include <stdexcept>

#include <lyra/lyra.hpp>

#include "TEA.hpp"
#include "terminal_echo.hpp"

using namespace tea;

//------------------------------------------------------------------------------

enum class ProgramAction { encrypt, decrypt };

struct ProgramArgs {
    ProgramAction action;
    std::string source_file;
    std::string target_file;
    bool show_help{false};
};

auto get_program_args(int argc, char** argv) {
    const std::string option_encrypt{"encrypt"};
    const std::string option_decrypt{"decrypt"};
    ProgramArgs res{};
    std::string action;
    auto cli = lyra::cli_parser() | lyra::help(res.show_help);
    cli.add_argument(lyra::opt(action, "action")
                         .name("-a")
                         .name("--action")
                         .required()
                         .choices(option_encrypt, option_decrypt)
                         .help("Action to choose: `encrypt` a file or `decrypt` a file"));
    cli.add_argument(lyra::opt(res.source_file, "source_file")
                         .name("-s")
                         .name("--source")
                         .required()
                         .help("Source file"));
    cli.add_argument(lyra::opt(res.target_file, "target_file")
                         .name("-t")
                         .name("--target")
                         .required()
                         .help("Target file"));
    auto result = cli.parse({argc, argv});

    if (!result) {
        std::cerr << "Error in command line: " << result.message() << "\n";
        std::cerr << cli << "\n";
        exit(1);
    }

    if (res.show_help) {
        std::cout << cli << "\n";
        exit(0);
    }

    if (action == option_encrypt) {
        res.action = ProgramAction::encrypt;
    } else if (action == option_decrypt) {
        res.action = ProgramAction::decrypt;
    } else {
        std::cerr << "Wrong action chosen (use `encrypt` or `decrypt`)\n";
        std::cerr << cli << "\n";
        exit(1);
    }

    if (res.source_file == res.target_file) {
        std::cerr << "Source and Target file cannot have the same name\n";
        exit(1);
    }

    return res;
}

//------------------------------------------------------------------------------

int main(int argc, char** argv) {
    try {
        const auto args = get_program_args(argc, argv);

        std::ifstream ifs{args.source_file};
        std::ofstream ofs{args.target_file};
        if (!ifs || !ofs) {
            throw std::runtime_error{"bad filename"};
        }

        SetStdinEcho(false);
        std::string key;
        std::cout << "enter key: ";
        std::cin >> key;
        std::cout << "\n";
        SetStdinEcho(true);

        // proceed with the main action
        switch (args.action) {
        case ProgramAction::encrypt:
            encrypt_file(ifs, ofs, key);
            break;
        case ProgramAction::decrypt:
            decrypt_file(ifs, ofs, key);
            break;
        default:
            std::cerr << "Default action branch\n";
            return 1;
        }
        return 0;
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "something went wrong\n";
        return 2;
    }
}
