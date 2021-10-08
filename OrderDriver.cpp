#include <iostream>
#include <cstring>
#include <vector>
#include <string>

#include "Orders.h"
#include "Territory.h"

using namespace std;

int mainXXXXXXXXXXXXX()
{
    OrderList oList1(105);
    Territory t2("Canada", 1, 2, 3, 4);
    Territory t3("US", 5, 6, 7, 8);
    Territory t4("England", 9,10,11,12);

    Orders* o1 = new DeployOrder(101,5, &t2);
    Orders* o2 = new DeployOrder(101, 30, &t3);
    Orders* o3 = new AdvanceOrder(101, 10, &t3, &t2,true);
    Orders* o4 = new BombOrder(101, &t4,true);
    Orders* o5 = new BlockadeOrder(101, &t2);
    Orders* o6 = new AirliftOrder(101, 7, &t3, &t2);
    Orders* o7 = new NegotiateOrder(101,&t2);

    oList1.put(o1);
    oList1.put(o2);
    oList1.put(o3);
    cout << "DEBUG: Create 7 orders, move first three inside the list." << "\n";
    oList1.displayAll();
    oList1.put(o4);
    oList1.put(o5);
    oList1.remove(*(o2->getOrderID()));
    oList1.put(o6);
    oList1.put(o7);
    cout << "DEBUG: move 4 new order inside the list. remove the order with id:2 " << "\n";

    oList1.displayAll();

    oList1.move(3, 3);
    cout << "DEBUG: move order 3 , 3 place down " << "\n";
    oList1.displayAll();

    oList1.move(5, 4);
    cout << "DEBUG: move order 5 , 4 place down, which should result in nothing, since it's out of range " << "\n";
    oList1.displayAll();

    oList1.move(3, -3);
    cout << "DEBUG: move order 3 , 3 place up" << "\n";
    oList1.displayAll();
    oList1.move(7, -4);
    cout << "DEBUG: move order 7 , 4 place up, which should result in nothing, since it's out of range " << "\n";
    oList1.displayAll();

    oList1.remove(*(o1->getOrderID()));
    oList1.remove(*(o4->getOrderID()));
    oList1.remove(*(o3->getOrderID()));

    cout << "DEBUG: remove order with id: 1,3,4 from order list " << "\n";
    oList1.displayAll();

    cout << "DEBUG: execute first order " << "\n";
    cout << oList1.getFirst()->execute() << "\n";

    cout << "DEBUG: execute last order " << "\n";
    cout << oList1.getLast()->execute() << "\n";

    oList1.displayAll();

    cout << "DEBUG: pop the first order and execute it" << "\n";
    cout << oList1.popFirst()->execute() << "\n";
    oList1.displayAll();
    cout << "DEBUG: pop the last order and execute it" << "\n";
    cout << oList1.popLast()->execute() << "\n";
    oList1.displayAll();

    cout << "Done!" << "\n";
    return 0;
}