#include<SFML/Graphics.hpp>
#include<random>
#include<ctime>
#include<vector>
#include<tuple>
#include<iostream>

using namespace std;

class Tile {
public:
    sf::Texture tex;
    vector<int> sides;
    string name;
    sf::Sprite image;

    Tile() {}
    Tile(string fileName, vector<int> sideValues) {
        tex.loadFromFile("Tiles/" + fileName);
        image.setTexture(tex);
        sides = sideValues;
        name = fileName;
    }

    ~Tile() {
    }
};

class Space {
public:
    bool collapsed = false;
    vector<Tile*> possibilities;
    sf::Vector2f pos;
    int entropy;
    Tile tile;

    Space(sf::Vector2f coord, vector<Tile*> all_tiles) {
        possibilities = all_tiles;
        pos = coord;
        entropy = all_tiles.size();

    }

    ~Space() {}

    void draw(sf::RenderWindow& window) {
        if (collapsed) {
            tile.image.setPosition(pos);
            window.draw(tile.image);
        }
    }

    void collapse() {
        if (entropy != 0) {
            collapsed = true;
            int randomSlot = rand() % entropy;
            tile = *possibilities[randomSlot];
        }
    }
};

bool determine_possibilities(vector<vector<Space*>*> grid) {
    vector<Tile*> filter;
    bool done = true;

    for (int i = 0; i < grid.size(); i++) {
        int j = 0;
        for (auto& space : *grid[i]) {
            if (!space->collapsed) {
                filter = space->possibilities;
                done = false;
                if (j + 1 < grid[i]->size()) {
                    if ((*grid[i])[j + 1]->collapsed) {
                        for (auto& possibility : space->possibilities) {
                            if ((*grid[i])[j + 1]->tile.sides[3] != possibility->sides[1]) {
                                filter.erase(remove(filter.begin(), filter.end(), possibility));
                            }
                        }
                    }
                }
                if (j - 1 >= 0) {
                    if ((*grid[i])[j - 1]->collapsed) {
                        for (auto& possibility : space->possibilities) {
                            if ((*grid[i])[j - 1]->tile.sides[1] != possibility->sides[3]) {
                                filter.erase(remove(filter.begin(), filter.end(), possibility));
                            }
                        }
                    }
                }
                if (i + 1 < grid.size()) {
                    if ((*grid[i + 1])[j]->collapsed) {
                        for (auto& possibility : space->possibilities) {
                            if ((*grid[i + 1])[j]->tile.sides[0] != possibility->sides[2]) {
                                filter.erase(remove(filter.begin(), filter.end(), possibility));
                            }
                        }
                    }
                }
                if (i - 1 >= 0) {
                    if ((*grid[i - 1])[j]->collapsed) {
                        for (auto& possibility : space->possibilities) {
                            if ((*grid[i - 1])[j]->tile.sides[2] != possibility->sides[0]) {
                                filter.erase(remove(filter.begin(), filter.end(), possibility));
                            }
                        }
                    }
                }
            }
            space->possibilities = filter;
            space->entropy = space->possibilities.size();
            j++;
        }
    }
    return done;
}

void collapse(vector<vector<Space*>*> grid, int max_entropy) {
    int lowestEntropy = max_entropy;
    vector<Space*> lowestVector;

    for (int i = 0; i < grid.size(); i++) {
        for (auto& space : *grid[i]) {
            if (!space->collapsed) {
                if (space->entropy < lowestEntropy) {
                    lowestEntropy = space->entropy;
                }
            }
        }
    }
    for (int i = 0; i < grid.size(); i++) {
        for (auto& space : *grid[i]) {
            if (!space->collapsed) {
                if (space->entropy == lowestEntropy) {
                    lowestVector.push_back(space);
                }
            }
        }
    }
    if (lowestVector.size() != 0) {
        int randomSlot = rand() % lowestVector.size();
        lowestVector[randomSlot]->collapse();
    }
}


vector<vector<Space*>*> makeGrid(sf::RenderWindow& window, vector<Tile*> all_tiles) {
    vector<vector<Space*>*> grid;
    for (int i = 0; i < window.getSize().y / 100; i++) {
        grid.push_back(new vector<Space*>);
            for (int j = 0; j < window.getSize().x / 100; j++) {
                grid[i]->push_back(new Space(sf::Vector2f(j * 100, i * 100), all_tiles));
            }
    }
    return grid;
}


int main() {

    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Wave Function Collapse", sf::Style::None);
    window.setPosition(sf::Vector2i(460, 20));
    sf::Event event;

    sf::Color bgColor = sf::Color(8, 6, 12);
    
    bool running = true;
    bool done = false;

    vector<Tile*> TileImages{ new Tile("Tile0.png", vector<int>{0, 0, 0, 0}),
                            new Tile("Tile1.png", vector<int>{1, 1, 0, 1}),
                            new Tile("Tile2.png", vector<int>{1, 1, 1, 0}),
                            new Tile("Tile3.png", vector<int>{0, 1, 1, 1}),
                            new Tile("Tile4.png", vector<int>{1, 0, 1, 1}),
                            new Tile("Tile5.png", vector<int>{0, 1, 0, 1}),
                            new Tile("Tile6.png", vector<int>{1, 0, 1, 0}),
                            new Tile("Tile7.png", vector<int>{1, 1, 0, 0}),
                            new Tile("Tile8.png", vector<int>{0, 1, 1, 0}),
                            new Tile("Tile9.png", vector<int>{0, 0, 1, 1}),
                            new Tile("Tile10.png", vector<int>{1, 0, 0, 1}),
                            new Tile("Tile15.png", vector<int>{1, 1, 1, 1}) };

    /*vector<Tile*> TileImages{ new Tile("Tile0.png", vector<int>{0, 0, 0, 0}),
                            new Tile("Tile1.png", vector<int>{1, 1, 0, 1}),
                            new Tile("Tile2.png", vector<int>{1, 1, 1, 0}),
                            new Tile("Tile3.png", vector<int>{0, 1, 1, 1}),
                            new Tile("Tile4.png", vector<int>{1, 0, 1, 1}),
                            new Tile("Tile5.png", vector<int>{0, 1, 0, 1}),
                            new Tile("Tile6.png", vector<int>{1, 0, 1, 0}), 
                            new Tile("Tile7.png", vector<int>{1, 1, 0, 0}), 
                            new Tile("Tile8.png", vector<int>{0, 1, 1, 0}), 
                            new Tile("Tile9.png", vector<int>{0, 0, 1, 1}), 
                            new Tile("Tile10.png", vector<int>{1, 0, 0, 1}), 
                            new Tile("Tile11.png", vector<int>{1, 0, 0, 0}), 
                            new Tile("Tile12.png", vector<int>{0, 1, 0, 0}), 
                            new Tile("Tile13.png", vector<int>{0, 0, 1, 0}), 
                            new Tile("Tile14.png", vector<int>{0, 0, 0, 1}),
                            new Tile("Tile15.png", vector<int>{1, 1, 1, 1}), };*/

    vector<vector<Space*>*> grid = makeGrid(ref(window), TileImages);

    while (window.isOpen()) {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) { 
                for (int i = 0; i < grid.size(); i++) {
                    for (auto& space : *grid[i]) {
                        space->~Space();
                    }
                }
                grid = makeGrid(ref(window), TileImages);
                done = false;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
                window.close();
            }

        }

        window.clear(bgColor);

        if (!done) {
            done = determine_possibilities(grid);
            collapse(grid, TileImages.size());
        }

        for (int i = 0; i < grid.size(); i++) {
            for (auto& space : *grid[i]) {
                space->draw(ref(window));
            }
        }

        window.display();
    }
}