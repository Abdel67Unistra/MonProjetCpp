#include "ColorTerminal.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>

#ifdef _WIN32
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif

// Définition des constantes statiques
const std::string ColorTerminal::RED = "\033[31m";
const std::string ColorTerminal::GREEN = "\033[32m";
const std::string ColorTerminal::YELLOW = "\033[33m";
const std::string ColorTerminal::BLUE = "\033[34m";
const std::string ColorTerminal::MAGENTA = "\033[35m";
const std::string ColorTerminal::CYAN = "\033[36m";
const std::string ColorTerminal::WHITE = "\033[37m";
const std::string ColorTerminal::BLACK = "\033[30m";

const std::string ColorTerminal::BG_RED = "\033[41m";
const std::string ColorTerminal::BG_GREEN = "\033[42m";
const std::string ColorTerminal::BG_YELLOW = "\033[43m";
const std::string ColorTerminal::BG_BLUE = "\033[44m";
const std::string ColorTerminal::BG_MAGENTA = "\033[45m";
const std::string ColorTerminal::BG_CYAN = "\033[46m";
const std::string ColorTerminal::BG_WHITE = "\033[47m";
const std::string ColorTerminal::BG_BLACK = "\033[40m";

const std::string ColorTerminal::BOLD = "\033[1m";
const std::string ColorTerminal::UNDERLINE = "\033[4m";
const std::string ColorTerminal::BLINK = "\033[5m";
const std::string ColorTerminal::REVERSE = "\033[7m";

const std::string ColorTerminal::RESET = "\033[0m";

bool ColorTerminal::supportsColor() {
    #ifdef _WIN32
        // Sur Windows, vérifier si ANSICON ou ConEmu est utilisé
        const char* term = std::getenv("TERM");
        const char* ansicon = std::getenv("ANSICON");
        const char* conemu = std::getenv("ConEmuANSI");
        
        return (term && std::string(term) != "dumb") || 
               ansicon || 
               conemu || 
               std::getenv("CMDER_ROOT");
    #else
        // Sur Unix/Linux/MacOS, vérifier si le terminal n'est pas "dumb"
        const char* term = std::getenv("TERM");
        return isatty(fileno(stdout)) && term && std::string(term) != "dumb";
    #endif
}

std::string ColorTerminal::colorize(const std::string& text, const std::string& color) {
    if (!supportsColor()) {
        return text;
    }
    
    std::stringstream ss;
    ss << color << text << RESET;
    return ss.str();
}

std::string ColorTerminal::stylize(const std::string& text, const std::string& style) {
    if (!supportsColor()) {
        return text;
    }
    
    std::stringstream ss;
    ss << style << text << RESET;
    return ss.str();
}

void ColorTerminal::info(const std::string& message) {
    std::cout << colorize(message, BLUE) << std::endl;
}

void ColorTerminal::success(const std::string& message) {
    std::cout << colorize(message, GREEN) << std::endl;
}

void ColorTerminal::warning(const std::string& message) {
    std::cout << colorize(message, YELLOW) << std::endl;
}

void ColorTerminal::error(const std::string& message) {
    std::cerr << colorize(message, RED) << std::endl;
}

void ColorTerminal::title(const std::string& title) {
    std::cout << stylize(colorize(title, CYAN), BOLD) << std::endl;
}

void ColorTerminal::separator(int width, char character) {
    std::string line(width, character);
    std::cout << colorize(line, BLUE) << std::endl;
} 