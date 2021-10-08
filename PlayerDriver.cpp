#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <regex>
#include <string>

#include "Orders.h"
#include "Territory.h"
#include "Player.h"
#include "Cards.h"
#include "Continent.h"
#include "Map.h"

using namespace std;

int main()
{
    Map* newMap = Map::mapCreater("LOTR2.map"); // bigeurope.map, LOTR2.map
    if (!newMap->validate()) {
        cout << "ERROR: the map we are loading didn't pass the validate process." << "\n";
        return 0;
    }
    newMap->displayLink();


    cout << "DEBUG: Create a player Object." << "\n";
    vector<vector<Territory*>> currentMapGraph = (newMap->getMapGraph());
    Player player1(2, "Comp345", &currentMapGraph);

    

    cout << "DEBUG: Adding terrtories ownership player" << "\n";
    player1.addTerrtories(currentMapGraph[1][0]);
    player1.printPlayerTerrtories();
    cout << "DEBUG: adding 1 more terrtories ownership player" << "\n";
    player1.addTerrtories(currentMapGraph[3][0]);
    player1.printPlayerTerrtories();
    cout << "DEBUG: adding 3 more terrtories ownership player" << "\n";
    player1.addTerrtories(currentMapGraph[2][0]);
    player1.addTerrtories(currentMapGraph[5][0]);
    player1.addTerrtories(currentMapGraph[10][0]);
    player1.printPlayerTerrtories();


    cout << "DEBUG: Create some orders and add them to player's orderlist." << "\n";
    player1.issueOrder(0, currentMapGraph[5][0], 10, currentMapGraph[3][0]);
    player1.issueOrder(0, currentMapGraph[6][0], 10, currentMapGraph[3][0]);
    player1.issueOrder(1, currentMapGraph[2][0], 5, currentMapGraph[3][0]);
    (*player1.getOrderList()).displayAll();

    cout << "DEBUG: Create some more orders and add them to player's orderlist." << "\n";
    player1.issueOrder(5, currentMapGraph[17][0], 12345, currentMapGraph[17][0]);
    player1.issueOrder(3, currentMapGraph[11][0], 12345, currentMapGraph[17][0]);
    //this will be an invaild order, since terrtories on currentMapGraph[11][0] is not owned by current player. leave for testing of validate() method
    player1.issueOrder(2, currentMapGraph[17][0], 12345, currentMapGraph[17][0]);
    player1.issueOrder(1, currentMapGraph[1][0], 5, currentMapGraph[2][0]);
    player1.issueOrder(1, currentMapGraph[2][0], 15, currentMapGraph[3][0]);
    (*player1.getOrderList()).displayAll();

    cout << "DEBUG: Testing the 'validate'method: " << "\n";
    bool valid = player1.getOrderList()->popFirst()->validate();
    cout << "POP the first order and  -- is it validate? :" << valid <<"\n";
    cout << "The first order is poped. checking the order list: " << "\n";
    (*player1.getOrderList()).displayAll();
    valid = player1.getOrderList()->popFirst()->validate();
    cout << "POP the 2nd order(currently the first) and  -- is it validate? :" << valid << "\n";
    cout << "the result of the order above should be 0(false), since currentMapGraph[6][0] is not controled by the current player." << valid << "\n";

    cout << "POP and exeute the last order checking the execute() method :" <<  "\n";
    cout << "result is(should be error ):" << player1.getOrderList()->popLast()->execute() << "\n";


    cout << "get and exeute the 2nd last order(current the last order) checking the execute() method :" << "\n";
    currentMapGraph[2][0]->setArmyNumber(30);
    cout << "result is(should be successful since it meet 3 requirement ):" << player1.getOrderList()->getLast()->execute() << "\n";


    delete newMap;
    newMap = nullptr;

    cout << "Done!" << "\n";
    return 0;
}