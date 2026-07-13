#ifdef CXX26_DEBUG

#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <map>
#include <expected>
#include <optional>
#include <memory>
#include <variant>
#include <meta>
#include <type_traits>
#include <cstdint>

enum class ERRORCODE : std::uint16_t{
    SUCCESS              = 0x000,
    ERROR                = 0xfff,
    FILE_DONT_EXIST      = 0x001,
    COULDNT_OPEN_FILE    = 0x002,
    INVALID_READ         = 0x003,
    NOT_FOUND_KEY        = 0x004
};

/*
    As you can see, this is a debug function that uses reflection and metaprogramming
    to work, in the first time, it doesn't work well, and how this is and recent
    technology I couldn't make this work, I sent to AI and it said it was a compiler
    error, and then I made a bug report in the bugzilla, and it wasn't an error,
    that was embarrasing.

    But I don't use AI anymore, and now I want to understand by the documentation.
    so let's me try to explain what is happening here:

    we recept a std::expected (its fragile, we need to check if it really is the expected object),
    and then extract the error (if not have, return SUCCESS) in the auto error = raw_error.error_or
    (ERRORCODE::SUCCESS), then we entered in a template for, and by what I understand is
    a for in compile time, and this for is a for each: defining in the compiling the 
    enumerator a static array, this is, a array in the compile time as said in https://en.cppreference.com/cpp/header/meta
    "promotes compile-time string to static storage, returning a pointer to the first character of the static string".
    And then we get the meta info from the array positions, passing by the for each,
    with it we transform the current member in a object in the programming with [::],
    this is, in other words, get the info of the variable (what is in the compiler plan)
    and turns it into a data in our program:

    float X; -> is a variable.
    ------------------------
    for the compiler this is something like:
    size: 8 bytes
    name: X
    address: 0x00
    
    and in the template for we are getting those meta informations from the array (remember we 
    tranformed the enum in a array with the define_static_array?) so in this point
    we have those info as a other plan, with the [::] we turn it into a info inside the 
    program (its veryyyyy cooll) and then compare its value (because getting this variable 
    inside the program with the [::] still is its value) with our target error,
    if it matches, we get  the name of the variable inside the array (before enum)
    and returning it from the function, the std::meta::identifier_of, by your time is 
    a std::string_view, so we need to return as it.

    thanks to Marek Polacek who answered my bug report.
*/

// this just returns if fail or success
template<typename u_type>
std::string_view GetError(const u_type& raw_error) {
    ERRORCODE error;
    error = raw_error.error_or(ERRORCODE::SUCCESS);

    template for (constexpr auto member : define_static_array(std::meta::enumerators_of(^^ERRORCODE))) {
        if ([:member:] == error) {
            return std::string_view(std::meta::identifier_of(member));
        }
    }

    return std::string_view("UNKNOWN_ERROR");
}

// this returns the success value if there's no error
// DOESN'T WORK YET
#ifdef WORKING_FUNCTIONS
    template<typename u_type>
    std::variant<std::string_view, float, std::string, int, bool, double> IsError(const u_type& raw_error) {
        ERRORCODE error;
        error = raw_error.error_or(ERRORCODE::SUCCESS);
        if (error == ERRORCODE::SUCCESS) {
            // caso seja sucesso:
            auto val = raw_error.value();
            return ;

        } else {    // se for erro.

            template for (constexpr auto member : define_static_array(std::meta::enumerators_of(^^ERRORCODE))) {
                if ([:member:] == error) {
                    return std::string_view(std::meta::identifier_of(member));
                }
            }
    
            return std::string_view("UNKNOWN_ERROR");
        }
    }
#endif

#endif //CXX26_DEBUG
