#include <liberror/ErrorOr.hpp>
#include <libpreprocessor/Preprocessor.hpp>
#include <cxxopts.hpp>

#include <print>
#include <span>
#include <fstream>
#include <filesystem>
#include <ranges>
#include <unordered_map>

liberror::ErrorOr<std::unordered_map<std::string, std::string>> setup_environment_variables(cxxopts::ParseResult const& parsedOptions)
{
    std::unordered_map<std::string, std::string> result {};

    auto const variables = parsedOptions["envvars"].as<std::vector<std::string>>();

    if (parsedOptions["envvars"].has_default()) return result;
    if (variables.size() % 2 != 0) return liberror::make_error(PREFIX_ERROR": variables should given in the following way: -e KEY1,VALUE1,KEY2,VALUE2");

    for (auto const& variable : variables | std::views::chunk(2))
    {
        auto const name  = variable.front();
        auto const value = variable.back();
        result.emplace(std::format("ENV:{}",name), value);
    }

    return result;
}

liberror::ErrorOr<void> preprocess_files(auto verbose, auto preserve, auto const& files, libpreprocessor::PreprocessorContext const& context)
{
    namespace fs = std::filesystem;

    for (fs::path const& file : files)
    {
        if (verbose) { std::println(PREFIX_INFO": processing: {}", file.string()); }

        if (fs::is_empty(file))
        {
            if (verbose) { std::println(PREFIX_INFO": skipping empty file: {}", file.string()); }
            continue;
        }

        if (!fs::is_regular_file(file))
        {
            if (verbose) { std::println(PREFIX_INFO": skipping non-regular file: {}", file.string()); }
            continue;
        }

        auto const result = TRY(libpreprocessor::preprocess(file, context));

        if (preserve)
        {
            auto const preservedName = std::format("{}\\{}.preserve{}", file.parent_path().string(), file.stem().string(), file.extension().string());
            fs::copy(file, preservedName, fs::copy_options::overwrite_existing);
            if (verbose) { std::println(PREFIX_INFO": file {} preserved as {}", file.string(), preservedName); }
        }

        std::ofstream { file, std::ios::trunc } << result;

        if (verbose) { std::println(PREFIX_INFO": done!"); }
    }

    return {};
}

liberror::ErrorOr<int> program_main(std::span<char const*> arguments)
{
    cxxopts::Options options { "kukui" };

    options.add_options()
        ("f,file", "file(s) to preprocess", cxxopts::value<std::vector<std::string>>()->default_value({}))
        ("e,envvars", "environment variables", cxxopts::value<std::vector<std::string>>()->default_value({}))
        ("p,preserve", "preserve the preprocessed file", cxxopts::value<bool>()->default_value("true"))
        ("h,help", "")
        ("v,verbose", "", cxxopts::value<bool>()->default_value("true"));

    auto const parsedOptions = options.parse(int(arguments.size()), arguments.data());

    if (parsedOptions.arguments().empty()) return liberror::make_error("{}", options.help());
    if (parsedOptions.count("file") == 0) return liberror::make_error("{}", options.help());
    if (parsedOptions.count("help")) return liberror::make_error("{}", options.help());

    namespace fs = std::filesystem;

    auto const verbose = parsedOptions["verbose"].as<bool>();
    auto const preserve = parsedOptions["preserve"].as<bool>();
    auto const files =
        parsedOptions["file"].as<std::vector<std::string>>()
            | std::views::transform([] (auto&& file) { return fs::path(file); });

    TRY(preprocess_files(verbose, preserve, files,
            libpreprocessor::PreprocessorContext {
                .localVariables = {},
                .environmentVariables = TRY(setup_environment_variables(parsedOptions))
            }));


    return EXIT_SUCCESS;
}

int main(int argumentCount, char const** argumentValues)
{
    auto const result = program_main({ argumentValues, size_t(argumentCount) });

    if (result.has_error())
    {
        std::println("{}", result.error().message());
        return EXIT_FAILURE;
    }

    return result.value();
}
