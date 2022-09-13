// CHECKOUT_EXERCISE.CPP
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <set>
#include <stdlib.h>
#include <algorithm>

using namespace std;



// #### STRUCT AND CLASS DECLARATIONS ####



struct Item
{
    int code;
    string name;
    double price;
    string offer;

    Item(int item_code, string item_name,
        double item_price, string item_offer)
    {
        code = item_code;
        name = item_name;
        price = item_price;
        offer = item_offer;
    }
};

class Transaction
{
    /*
    Class for storing items when a new transaction occurs
    includes member functions to calculate subtotal and total

    hash tables relating product ID to product names are passed
    */
    vector <int> item_list;
    double subtotal;
    double total;
    vector <double> discounts;
public:
    void add_item(int new_item);
    void list_items(unordered_map<int, string> code_name_hash,
        unordered_map<int, double> code_price_hash);
    double sum_list(unordered_map<int, double> code_price_hash);
    void apply_offers(unordered_map<int, string> code_name_hash,
        unordered_map<int, double> code_price_hash,
        unordered_map<int, string> code_offer_hash);
};



//#### TRANSACTION CLASS MEMBER FUNCTIONS ####



void Transaction::add_item(int new_item)
{
    item_list.push_back(new_item);
}

void Transaction::list_items(unordered_map<int, string> code_name_hash,
    unordered_map<int, double> code_price_hash)
{

    vector <int>::iterator it;

    cout << fixed;
    cout << setprecision(2);
    cout << "ITEM LIST:" << endl;
    // iterate over item list
    for (it = item_list.begin(); it != item_list.end(); ++it)
    {
        cout << code_name_hash[*it] << "\t-\t$"
            << code_price_hash[*it] << endl;
    }

}


double Transaction::sum_list(unordered_map<int, double> code_price_hash)
{
    int i;
    double sum = 0.0;
    for (i = 0; i < item_list.size(); i++)
    {
        sum += code_price_hash[item_list[i]];
    }

    subtotal = sum;
    cout << fixed;
    cout << setprecision(2);
    cout << "SUBTOTAL: \t-\t$" << sum << endl;

    return sum;
}

void Transaction::apply_offers(unordered_map<int, string> code_name_hash,
    unordered_map<int, double> code_price_hash,
    unordered_map<int, string> code_offer_hash)
{
    /*
    Applies special offers and calculates total
    offer codes :
    "3FOR90" - buy three pay 90 cents
    "10For6" - buy ten and pay $6
    "2For3" - buy two and pay $3
    */
    vector <int> three_for_ninty_list;
    vector <int> TenForSix;
    vector <int> TwoForThree;
    vector <int>::iterator it;
    int i;
    double total_discount = 0.00;

    cout << fixed;
    cout << setprecision(2);
    // extract special offer items to relevant lists
    for (it = item_list.begin(); it != item_list.end(); ++it)
    {
        if (code_offer_hash[*it] == "3FOR90")
        {
            three_for_ninty_list.push_back(*it);
        }
        else if (code_offer_hash[*it] == "10For6")
        {
            TenForSix.push_back(*it);
        }
        else if (code_offer_hash[*it] == "2For3")
        {
            TwoForThree.push_back(*it);
        }
    }

    // "3FOR90" - buy three pay 90 cents

    set <int> unique(three_for_ninty_list.begin(), three_for_ninty_list.end());

    // Loop over elements in unique  

    int n_items;

    for (auto element : unique)
    {
        n_items = count(three_for_ninty_list.begin(),
            three_for_ninty_list.end(), element);

        if (n_items >= 3)
        {
            cout << n_items << " x " << code_name_hash[element] << endl;
            int n_sets = n_items / 3; // should floor value

            int res = n_items % 3;
            n_items -= res;
            total_discount += n_sets * 0.30;
            cout << "DISCOUNT 3 FOR 90 cents on " << n_items << " " << code_name_hash[element] << " are $ "
                << -n_sets * total_discount << endl;

        }

    }

    //  "10For6" - buy ten and pay $6

    set <int> unique1(TenForSix.begin(), TenForSix.end());

    // Loop over elements in unique  
    double TenForSix_discount = 0.00;

    for (auto element : unique1)
    {
        n_items = count(TenForSix.begin(),
            TenForSix.end(), element);

        if (n_items >= 10)
        {
            cout << n_items << " x " << code_name_hash[element] << endl;
            int n_sets = n_items / 10; // should floor value

            int res = n_items % 10;
            n_items -= res;
            TenForSix_discount += n_sets * 1.5;
            cout << "DISCOUNT 10 for $6 on " << n_items << " " << code_name_hash[element] << " are $ "
                << -n_sets * TenForSix_discount << endl;
            total_discount += TenForSix_discount;
        }

    }

    // "2For3" - buy two and pay $3

    set <int> unique2(TwoForThree.begin(), TwoForThree.end());
    double TwoForThree_discount = 0.00;
    for (auto element : unique2)
    {
        n_items = count(TwoForThree.begin(),
            TwoForThree.end(), element);

        if (n_items >= 2)
        {
            cout << n_items << " x " << code_name_hash[element] << endl;
            int n_sets = n_items / 2; // should floor value

            int res = n_items % 2;
            n_items -= res;
            TwoForThree_discount += n_sets * 1.00;
            cout << "DISCOUNT 2 for $3 on " << n_items << " " << code_name_hash[element] << " are $ "
                << -n_sets * TwoForThree_discount << endl;
            total_discount += TwoForThree_discount;
        }

    }

    cout << "TOTAL DISCOUNT\t-\t$ " << -total_discount << endl;
    total = subtotal - total_discount;

    cout << "\nTOTAL\t-\t$ " << total << endl;
}



//#### FUNCTION DECLARATIONS ####



string parse_args(int argc, char* argv[]);
vector<Item> generate_item_list(const string& f_in);
unordered_map<int, string> init_code_name_hash(vector<Item> item_list);
unordered_map<int, double> init_code_price_hash(vector<Item> item_list);
unordered_map<int, string> init_code_offer_hash(vector<Item> item_list);
bool session_prompt(unordered_map<int, string> code_name_hash,
    unordered_map<int, double> code_price_hash,
    unordered_map<int, string> code_offer_hash);

int main(int argc, char* argv[])
{
    string price_index_file;
    vector<Item> item_index;
    unordered_map<int, string> code_name_hash;
    unordered_map<int, double> code_price_hash;
    unordered_map<int, string> code_offer_hash;
    bool session_flag = true;

    price_index_file = parse_args(argc, argv);

    cout << price_index_file << endl;

    // Read item list
    item_index = generate_item_list(price_index_file);

    // Generate hash tables
    code_name_hash = init_code_name_hash(item_index);
    code_price_hash = init_code_price_hash(item_index);
    code_offer_hash = init_code_offer_hash(item_index);

    // Start new transaction
    while (session_flag)
    {
        session_flag = session_prompt(code_name_hash, code_price_hash,
            code_offer_hash);
    }

    cout << "End of session..." << endl;

    return 0;
}



//#### FUNCTIONS ####


string parse_args(int argc, char* argv[])
{

    string f_in = "item_index.txt";

    return f_in;
}

vector<Item> generate_item_list(const string& f_path)
{
    /*
    Reads in item info from file
    File must contain a line for each item in format:
    item_code item_name price offer, separated by tabs

    Input:
    String containing path to item index file

    Output:
    Returns a vector of items corresponding to
    each entry in the file
    */

    vector<Item> item_list;

    ifstream f_index(f_path.c_str());

    string line;
    while (getline(f_index, line))
    {
        // Extract lines from file 
        vector<string> tokens;
        istringstream iss(line);
        string token;
        while (getline(iss, token, '\t'))
        {
            tokens.push_back(token);
        }

        // Create new item - assume formatting is correct
        int new_item_code = atoi(tokens[0].c_str());
        string new_item_name = tokens[1];
        double new_item_price = atof(tokens[2].c_str());
        string new_item_offer = tokens[3];

        Item new_item(new_item_code, new_item_name,
            new_item_price, new_item_offer);

        item_list.push_back(new_item);
    }

    return item_list;
}

// Hash table functions
unordered_map<int, string> init_code_name_hash(vector<Item> item_list)
{
    /*
    Generates a hash table relating item code to item name

    Input:
    item_list - vector of Items

    Output
    code_name_hash - unordered_map of integer item codes to item names
    */

    unordered_map<int, string> code_name_hash;
    int i;

    for (i = 0; i < item_list.size(); i++)
    {
        code_name_hash[item_list[i].code] = item_list[i].name;
    }

    return code_name_hash;
}

unordered_map<int, double> init_code_price_hash(vector<Item> item_list)
{
    /*
    Generates a hash table relating item code to item price

    Input:
    item_list - vector of Items

    Output
    code_price_hash - unordered_map of integer item codes to item prices
    */

    unordered_map<int, double> code_price_hash;
    int i;

    for (i = 0; i < item_list.size(); i++)
    {
        code_price_hash[item_list[i].code] = item_list[i].price;
    }

    return code_price_hash;
}


unordered_map<int, string> init_code_offer_hash(vector<Item> item_list)
{
    /*
    Generates a hash table relating item code to item offer

    Input:
    item_list - vector of Items

    Output
    code_offer_hash - unordered_map of integer item codes to offers
    */

    unordered_map<int, string> code_offer_hash;
    int i;

    for (i = 0; i < item_list.size(); i++)
    {
        code_offer_hash[item_list[i].code] = item_list[i].offer;
    }

    return code_offer_hash;
}


bool session_prompt(unordered_map<int, string> code_name_hash,
    unordered_map<int, double> code_price_hash,
    unordered_map<int, string> code_offer_hash)
{
    /*
    User prompt for new transaction
    Item code is entered for each new item
    'e' to exit
    Subtotal (pre-offer sum total of items) is displayed, then final total
    with discounts applied is displayed
    */


    // Start transaction
    Transaction new_transaction;
    bool break_flag = false;
    string item_code;
    bool invalid_item_flag = true;
    cout << "Item Code\tItem List" << endl
        << "1.\t\tApple" << endl
        << "2.\t\tBananas" << endl
        << "3.\t\tCoconuts" << endl
        << "4.\t\tDonuts" << endl;
    while (!break_flag)
    {
        cout << "Enter item code or \'e\' to finish" << endl;

        cin >> item_code;
        if (item_code == "e")
        {
            break_flag = true;
        }
        else
        {
            // check item code is in hash table
            auto t = code_name_hash.find(atoi(item_code.c_str()));
            if (t == code_name_hash.end())
            {
                cout << "ERROR: Item code invalid. Try again..." << endl;
            }
            else //continue to add item
            {
                new_transaction.add_item(atoi(item_code.c_str()));
                new_transaction.list_items(code_name_hash, code_price_hash);
            }
        }
    }

    // Display subtotal
    new_transaction.sum_list(code_price_hash);

    // Apply offers
    new_transaction.apply_offers(code_name_hash,
        code_price_hash,
        code_offer_hash);

    // End of transaction    
    bool session_flag;
    string value;
    break_flag = false;

    while (!break_flag)
    {
        cout << "Start new transaction? Y/N" << endl;
        cin >> value;
        if (value == "Y")
        {
            session_flag = true;
            break_flag = true;
        }
        else if (value == "N")
        {
            session_flag = false;
            break_flag = true;
        }
        else
        {
            cout << "Please enter Y or N." << endl;
        }

    }
    return session_flag;
}