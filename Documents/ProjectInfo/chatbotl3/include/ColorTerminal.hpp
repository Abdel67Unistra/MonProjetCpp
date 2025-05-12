#ifndef COLOR_TERMINAL_HPP
#define COLOR_TERMINAL_HPP

#include <string>
#include <iostream>

/**
 * @class ColorTerminal
 * @brief Utilitaire pour ajouter des couleurs et du style au terminal
 * 
 * Cette classe fournit des méthodes statiques pour colorer et styliser
 * le texte dans un terminal compatible ANSI.
 */
class ColorTerminal {
public:
    // Codes de couleur pour le texte
    static const std::string RED;
    static const std::string GREEN;
    static const std::string YELLOW;
    static const std::string BLUE;
    static const std::string MAGENTA;
    static const std::string CYAN;
    static const std::string WHITE;
    static const std::string BLACK;
    
    // Codes de couleur pour l'arrière-plan
    static const std::string BG_RED;
    static const std::string BG_GREEN;
    static const std::string BG_YELLOW;
    static const std::string BG_BLUE;
    static const std::string BG_MAGENTA;
    static const std::string BG_CYAN;
    static const std::string BG_WHITE;
    static const std::string BG_BLACK;
    
    // Styles de texte
    static const std::string BOLD;
    static const std::string UNDERLINE;
    static const std::string BLINK;
    static const std::string REVERSE;
    
    // Réinitialisation
    static const std::string RESET;
    
    /**
     * @brief Vérifie si le terminal supporte les couleurs ANSI
     * @return true si les couleurs sont supportées, false sinon
     */
    static bool supportsColor();
    
    /**
     * @brief Colore un texte avec la couleur spécifiée
     * @param text Le texte à colorer
     * @param color Le code couleur à appliquer
     * @return Le texte coloré
     */
    static std::string colorize(const std::string& text, const std::string& color);
    
    /**
     * @brief Applique un style au texte
     * @param text Le texte à styliser
     * @param style Le code de style à appliquer
     * @return Le texte stylisé
     */
    static std::string stylize(const std::string& text, const std::string& style);
    
    /**
     * @brief Affiche un message d'information en bleu
     * @param message Le message à afficher
     */
    static void info(const std::string& message);
    
    /**
     * @brief Affiche un message de succès en vert
     * @param message Le message à afficher
     */
    static void success(const std::string& message);
    
    /**
     * @brief Affiche un message d'avertissement en jaune
     * @param message Le message à afficher
     */
    static void warning(const std::string& message);
    
    /**
     * @brief Affiche un message d'erreur en rouge
     * @param message Le message à afficher
     */
    static void error(const std::string& message);
    
    /**
     * @brief Affiche un titre en cyan et gras
     * @param title Le titre à afficher
     */
    static void title(const std::string& title);
    
    /**
     * @brief Affiche un séparateur horizontal
     * @param width Largeur du séparateur (défaut: 80)
     * @param character Caractère utilisé pour le séparateur (défaut: -)
     */
    static void separator(int width = 80, char character = '-');
};

#endif // COLOR_TERMINAL_HPP 