#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Sonido.h"
#include <vector>

class Menu
{
public:
    enum class GameState
    {
        MainMenu,
        Game,
        Controls,
        Exit
    };

    friend class Juego;  // Permite que la clase Juego acceda a los miembros privados de Menu.

    // Constructor y Destructor
    Menu();
    ~Menu();

    // Métodos públicos
    void set_values();           
    void run_menu();             
    void loop_events();          
    void draw_all();             
    void showControls();         

    // Método de acceso para obtener el estado del juego
    GameState get_state() const { return state; }

private:
    // Miembros privados
    Sounds SonidosM;
    sf::RenderWindow window;                
    sf::RenderWindow controlsWindow;        
    sf::Font font;                          
    sf::Texture image;                      
    sf::Texture imagecontrolBackground;     
    sf::Sprite bg;                          
    sf::Sprite controlsBackground;      
    sf::RectangleShape winclose;
        
    size_t currentFrame;
    sf::Clock animationClock;
    std::vector<sf::Texture> backgroundTextures;       

    sf::Text titulotext;
    std::vector<sf::Text> texts;            
    std::vector<std::string> options;       
    std::vector<sf::Vector2f> coords;       
    std::vector<int> sizes;                 
    int pos;                                
    bool pressed;                           
    bool theselect;                         
    bool isPlaying;                         
    bool seleccionJugar;                    

    sf::Vector2i pos_mouse;                 
    sf::Vector2f mouse_coord;               

    GameState state;
};

#endif
